#include <acl/libacl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

/*
root@amdpcjelly:~/code/cpp-learning/linux/file# getfacl file.tmp 
# file: file.tmp
# owner: root
# group: root
user::rwx
user:jaxon:rwx                  #effective:r--
group::r-x                      #effective:r--
group:hong:--x                  #effective:---
mask::r--
other::r--

- ACL MASK 唯独不影响 user obj
- chmod 对于 acl 而言
    - 如果修改了 user 或 other，则会改动 acl 的 user obj，或 other
    - 如果修改了 group，是否对该文件执行过 setfacl 操作，即文件是否具有最小的 ACL 权限：
        - 是。会改动 acl 的 group obj
        - 否。改动的是 acl 的 mask，而不是 acl 的 group obj 和 group
- ACL 的优先级要高于对文件组权限的传统操作
- 新建文件具有最小的 access ACL 权限，等同于文件的传统权限
- setfacl 可以单独修改 ACL MASK 的值

setfacl
- 是否首次执行 setfacl：
    - 是。acl 会产生 ACL MASK 项，并且值等于文件传统权限中的 group
    - 否。增加 user 或 group 时，入参权限设置会影响 ACL MASK，ACL MASK 又会影响到文件传统权限中的 group
      比如，setfacl -m u:jaxon:w file.txt，ACL MASK 会追加 w，并且 file.txt 传统权限的 group 也会追加 w
      setfacl -x u:jaxon file.txt，删除 user 后，ACL MASK 和传统权限的 group 都会移除 w

ACL 分类
- 访问型 ACL，用于文件、目录
- 默认型 ACL，只用于目录。setfacl getfacl 加上 -d 参数
    - 访问目录时，默认型 ACL 并不参与判定所授予的权限。相反，默认型 ACL 的存在与否
      决定了在目录下所创建文件或子目录的 ACL 和权限
    - 新建目录具有最小的 access ACL entry，但是没有 default ACL entry。所以执行 acl_set_file(ACL_TYPE_DEFAULT, acl) 时，
      要求入参 acl 设置了 ACL_USER_OBJ,ACL_GROUP_OBJ,ACL_OTHER 这三个 entry

默认型 ACL，为什么称为默认型
- 因为在该目录下（没有层级限制）新增的文件和子目录会把目录的 ACL 沿用为自己的 ACL
- tmp 目录下已有的 a.file 文件，在 tmp 设置了默认 ACL 后，a.file 不会继承该 ACL，只有后续新增的文件才会继承

目录 UGO 规则
- ls 需要具备 r 权限
- cd 需要具备 x 权限
- 在目录下创建文件需要 wx 权限
*/

void handle_error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int set_acl_entry_permset(acl_entry_t &entry, int perm)
{
    acl_perm_t acl_perm = 0;
    acl_permset_t permset;

    if (acl_get_permset(entry, &permset) != 0) {
        handle_error("acl_get_permset");
        return -1;
    }    

    if (acl_clear_perms(permset) != 0) {
        handle_error("acl_clear_perms");
        return -1;
    }

    if ((perm & 4) != 0) {        
        acl_perm |= ACL_READ;
    }
    if ((perm & 2) != 0) {
        acl_perm |= ACL_WRITE;
    }
    if ((perm & 1) != 0) {
        acl_perm |= ACL_EXECUTE;
    }
    if (acl_add_perm(permset, acl_perm) != 0) {
        handle_error("acl_add_perm");
        return -1;
    }  

    if (acl_set_permset(entry, permset) != 0) {
        handle_error("acl_set_permset");
        return -1;
    }    
    return 0;
}

// @param acl_tag ACL_USER_OBJ, ACL_GROUP_OBJ, ACL_OTHER
int create_ugo_acl_entry(acl_t &acl, acl_tag_t acl_tag, int perm)
{
    acl_entry_t entry;
    acl_permset_t permset;
    
    if (acl_create_entry(&acl, &entry) != 0) {
        handle_error("acl_create_entry");
        return -1;
    }
    if (acl_set_tag_type(entry, acl_tag) != 0) {
        handle_error("acl_set_tag_type");
    }
    if (set_acl_entry_permset(entry, perm) != 0) {
        return -1;
    }

    return 0;
}

// @param acl_tag ACL_USER or ACL_GROUP
// @param id uid or gid
int modify_default_acl_entry(const char *filename, acl_tag_t acl_tag, uint id, int perm)
{
    bool found_user_obj_entry = false;
    bool found_group_obj_entry = false;
    bool found_other_entry = false;
    bool found_id_entry = false;
    int entry_id = ACL_FIRST_ENTRY;
    uint *idp;
    acl_tag_t tag;
    acl_entry_t entry, ugo_entry;
    acl_t acl;

    acl = acl_get_file(filename, ACL_TYPE_DEFAULT);
    if (!acl) {
        printf("acl_get_file '%s' error, %s\n", filename, strerror(errno));
        return -1;
    }

    while (acl_get_entry(acl, entry_id, &entry) == 1) {
        if (acl_get_tag_type(entry, &tag) != 0) {
            printf("acl_get_tag_type error, %s\n", strerror(errno));
            goto err;
        }
        if (tag == acl_tag && !found_id_entry) {
            idp = (uint*) acl_get_qualifier(entry);
            if (*idp == id) {
                found_id_entry = true;
                break;
            }            
        }
        entry_id = ACL_NEXT_ENTRY;
    }

    entry_id = ACL_FIRST_ENTRY;
    while (acl_get_entry(acl, entry_id, &ugo_entry) == 1) {
        if (acl_get_tag_type(ugo_entry, &tag) != 0) {
            printf("acl_get_tag_type error, %s\n", strerror(errno));
            goto err;
        }
        if (tag == ACL_USER_OBJ && !found_user_obj_entry) {
            found_user_obj_entry = true;
        } else if (tag == ACL_GROUP_OBJ && !found_group_obj_entry) {
            found_group_obj_entry = true;
        } else if (tag == ACL_OTHER && !found_other_entry) {
            found_other_entry = true;
        }
        entry_id = ACL_NEXT_ENTRY;
    }   

    if (!found_user_obj_entry || !found_group_obj_entry || !found_other_entry) {
        struct stat statbuf;
        int perm = 0;
        if (stat(filename, &statbuf) != 0) {
            printf("stat '%s' error, %s\n", filename, strerror(errno));
            goto err;
        }  
        if (!found_user_obj_entry) {
            perm = 0;
            if (statbuf.st_mode & S_IRUSR) perm |= 4;
            if (statbuf.st_mode & S_IWUSR) perm |= 2;
            if (statbuf.st_mode & S_IXUSR) perm |= 1;
            if (create_ugo_acl_entry(acl, ACL_USER_OBJ, perm) != 0) {
                goto err;
            }
        }
        if (!found_user_obj_entry) {
            perm = 0;
            if (statbuf.st_mode & S_IRGRP) perm |= 4;
            if (statbuf.st_mode & S_IWGRP) perm |= 2;
            if (statbuf.st_mode & S_IXGRP) perm |= 1;
            if (create_ugo_acl_entry(acl, ACL_GROUP_OBJ, perm) != 0) {
                goto err;
            }
        }
        if (!found_user_obj_entry) {
            perm = 0;
            if (statbuf.st_mode & S_IROTH) perm |= 4;
            if (statbuf.st_mode & S_IWOTH) perm |= 2;
            if (statbuf.st_mode & S_IXOTH) perm |= 1;
            if (create_ugo_acl_entry(acl, ACL_OTHER, perm) != 0) {
                goto err;
            }
        }        
    }

    if (!found_id_entry) {    
        if (acl_create_entry(&acl, &entry) != 0) {
            printf("acl_create_entry error, %s\n", strerror(errno));
            goto err;
        }           
        if (acl_set_tag_type(entry, acl_tag) != 0) {
            printf("acl_set_tag_type error, %s\n", strerror(errno));
            goto err;
        }
        if (acl_set_qualifier(entry, (void*) &id) != 0) {
            printf("acl_set_qualifier error, %s\n", strerror(errno));
            goto err;
        }  
    }

    if (set_acl_entry_permset(entry, perm) != 0) {
        goto err;
    }    

    if (acl_calc_mask(&acl) != 0) {
        printf("acl_calc_mask error, %s\n", strerror(errno));
        goto err;
    }

    if (acl_set_file(filename, ACL_TYPE_DEFAULT, acl) != 0) {
        printf("acl_set_file error, %s\n", strerror(errno));
        goto err;
    }

    acl_free(acl);
    printf("ACL_DEFAULT permission '%d' set successfully for file '%s'\n", perm, filename);    
    return 0;    

err:
    acl_free(acl);
    printf("ACL_DEFAULT permission '%d' set failed for file '%s'\n", perm, filename);   
    return -1;
}

// @param acl_tag ACL_USER or ACL_GROUP
// @param id uid or gid
int modify_access_acl_entry(const char *filename, acl_tag_t acl_tag, uint id, int perm)
{
    bool found = false;
    int entry_id = ACL_FIRST_ENTRY;
    acl_tag_t tag_type;
    acl_entry_t entry;
    uint *idp;
    acl_t acl;
    
    acl = acl_get_file(filename, ACL_TYPE_ACCESS);
    if (!acl) {
        printf("acl_get_file '%s' error, %s\n", filename, strerror(errno));
        return -1;
    }

    while (acl_get_entry(acl, entry_id, &entry) == 1) {
        if (acl_get_tag_type(entry, &tag_type) != 0) {
            printf("acl_get_tag_type error, %s\n", strerror(errno));
            goto err;
        }        
        if (tag_type == acl_tag) {
            idp = (uint*) acl_get_qualifier(entry);
            if (*idp == id) {
                found = true;
                break;
            }            
        }
        entry_id = ACL_NEXT_ENTRY;
    }

    if (!found) {    
        if (acl_create_entry(&acl, &entry) != 0) {
            printf("acl_create_entry error, %s\n", strerror(errno));
            goto err;
        }   
        // - acl_set_tag_type 必须在 acl_set_qualifier 之前
        // - acl_set_qualifier 只对 ACL_USER 和 ACL_GROUP 有意义             
        if (acl_set_tag_type(entry, acl_tag) != 0) {
            printf("acl_set_tag_type error, %s\n", strerror(errno));
            goto err;
        }
        if (acl_set_qualifier(entry, (void*) &id) != 0) {
            printf("acl_set_qualifier error, %s\n", strerror(errno));
            goto err;
        }  
    }

    if (set_acl_entry_permset(entry, perm) != -0) {
        goto err;
    }

    // 计算并设置掩码
    if (acl_calc_mask(&acl) != 0) {
        handle_error("acl_calc_mask");
    }

    if (acl_set_file(filename, ACL_TYPE_ACCESS, acl) != 0) {
        handle_error("acl_set_file");
    }

    acl_free(acl);
    printf("ACL_ACCESS permission '%d' set successfully for file '%s'\n", perm, filename);    
    return 0;

err:
    acl_free(acl);
    printf("ACL_ACCESS permission '%d' set failed for file '%s'\n", perm, filename);    
    return -1;
}

// @param acl_tag ACL_USER or ACL_GROUP
// @param id uid or gid
// @param recusive whether set sub directory and file if filename is directory
int modify_acl_entry(const char *filename, acl_tag_t acl_tag, uint id, int perm, bool recusive)
{
    int ret = 0;
    char path[PATH_MAX];
    struct stat statbuf;

    if (stat(filename, &statbuf) != 0) {
        handle_error("stat");
        return -1;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        if (recusive) {
            DIR *dirp = nullptr;
            struct dirent *entry;
            if (!(dirp = opendir(filename))) {
                return -1;
            }
            while (entry = readdir(dirp)) {
                if (entry->d_name[0] == '.') {
                    if (entry->d_name[1] == '\0' ||
                            (entry->d_name[1] == '.' && entry->d_name[2] == '\0')) {
                        continue;
                    }
                }            
                if (entry->d_type == DT_DIR) {      
                    snprintf(path, sizeof(path), "%s/%s", filename, entry->d_name);
                    ret = modify_acl_entry(path, acl_tag, id, perm, recusive);
                } else if (entry->d_type == DT_REG) {            
                    snprintf(path, sizeof(path), "%s/%s", filename, entry->d_name); 
                    ret = modify_access_acl_entry(path, acl_tag, id, perm);
                }
                if (ret != 0) {
                    break;
                }
            }
            closedir(dirp);
        }
        // acl access entry 的改动可能会修改到 UGO 的 group 权限，所以先执行 default acl entry 的修改
        ret = modify_default_acl_entry(filename, acl_tag, id, perm);        
        if (ret != 0) {
            return ret;
        }
        ret = modify_access_acl_entry(filename, acl_tag, id, perm);
    } else {
        ret = modify_access_acl_entry(filename, acl_tag, id, perm);
    }

    return ret;
}

int main(int argc, char *argv[])
{
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <uid|gid> <type[u|g]> <filename> <permission> <recusive[0|1]>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uint id = atoi(argv[1]);

    acl_tag_t tag_type;
    if (strcmp(argv[2], "u") == 0) {
        tag_type = ACL_USER;
    } else {
        tag_type = ACL_GROUP;
    }

    const char* filename = argv[3];
    int permission = atoi(argv[4]);
    bool recusive = false;
    if (argc == 6) {
        recusive = atoi(argv[5]);
    }

    modify_acl_entry(filename, tag_type, id, permission, recusive);
    
    return 0;
}
