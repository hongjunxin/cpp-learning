#include <acl/libacl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
- 新建文件具有最小的 ACL 权限，等同于文件的传统权限
- setfacl 可以单独修改 ACL MASK 的值

setfacl
- 是否首次执行 setfacl：
    - 是。acl 会产生 ACL MASK 项，并且值等于文件传统权限中的 group
    - 否。增加 user 或 group 时，入参权限设置会影响 ACL MASK，ACL MASK 又会影响到文件传统权限中的 group
      比如，setfacl -m u:jaxon:w file.txt，ACL MASK 会追加 w，并且 file.txt 传统权限的 group 也会追加 w
      setfacl -x u:jaxon file.txt，删除 user 后，ACL MASK 和传统权限的 group 都会移除 w

ACL 分类
- 访问型 ACL，用于文件
- 默认型 ACL，用于目录。setfacl getfacl 加上 -d 参数

目录的 ACL，为什么称为默认型
- 因为在该目录下（没有层级限制）新增的文件和子目录会把目录的 ACL 沿用为自己的 ACL
- tmp 目录下已有的 a.file 文件，在 tmp 设置了默认 ACL 后，a.file 不会继承该 ACL，只有后续新增的文件才会继承
*/

void handle_error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <permission>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];
    const char* permission = argv[2];

    // NOTE: acl api 调用顺序很重要，否则会出错

    // - ACL_TYPE_ACCESS 用于访问型 ACL
    // - ACL_TYPE_DEFAULT 用于默认型 ACL
    acl_t acl = acl_get_file(filename, ACL_TYPE_ACCESS);
    if (acl == NULL) {
        handle_error("acl_get_file");
    }

    acl_entry_t entry;    
    if (acl_create_entry(&acl, &entry) != 0) {
        handle_error("acl_create_entry");
    }

    uid_t uid = 1002;
    // - acl_set_tag_type 必须在 acl_set_qualifier 之前
    // - acl_set_qualifier 只对 ACL_USER 和 ACL_GROUP 有意义
    if (acl_set_tag_type(entry, ACL_USER) != 0) {
        handle_error("acl_set_tag_type");
    }
    if (acl_set_qualifier(entry, (void*) &uid) != 0) {
        handle_error("acl_set_qualifier");
    }    

    acl_permset_t permset;
    if (acl_get_permset(entry, &permset) != 0) {
        handle_error("acl_get_permset");
    }

    if (strcmp(permission, "r") == 0) {
        if (acl_add_perm(permset, ACL_READ) != 0) {
            handle_error("acl_add_perm");
        }
    } else if (strcmp(permission, "w") == 0) {
        if (acl_add_perm(permset, ACL_WRITE) != 0) {
            handle_error("acl_add_perm");
        }
    } else if (strcmp(permission, "x") == 0) {
        if (acl_add_perm(permset, ACL_EXECUTE) != 0) {
            handle_error("acl_add_perm");
        }
    } else {
        fprintf(stderr, "Invalid permission specified. Please use 'r', 'w', or 'x'.\n");
        exit(EXIT_FAILURE);
    }

    if (acl_set_permset(entry, permset) != 0) {
        handle_error("acl_set_permset");
    }

    // 计算并设置掩码
    if (acl_calc_mask(&acl) != 0) {
        handle_error("acl_calc_mask");
    }

    // 如果文件只有最小的 ACL 权限，会导致 acl_set_file 失败。即 ls -alh，rwx 的属性标志后面没有 + 号
    if (acl_set_file(filename, ACL_TYPE_ACCESS, acl) != 0) {
        handle_error("acl_set_file");
    }

    acl_free(acl);
    printf("ACL permission '%s' set successfully for file '%s'\n", permission, filename);
    
    return 0;
}

