#include <acl/libacl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void set_acl(acl_t *acl, const char *permission, const char *filename);

void handle_error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/*
    acl 覆盖测试。先保存两个 acl 句柄，再顺序对这两个 acl 进行 entry 修改并回写操作。
    测试结果：getfacl 得到的结果是最后写入的 acl
*/

/*
log：
===>
user::rw-
user:jaxon:r--
user:sim:-w-
group::r--
mask::rw-
other::r--
<===
ACL permission 'r' set successfully for file 'file.tmp'
===>
user::rw-
user:jaxon:-w-
user:sim:-w-
group::r--
mask::rw-
other::r--
<===
ACL permission 'w' set successfully for file 'file.tmp'

# getfacl file.tmp 
user::rw-
user:jaxon:-w-
user:sim:-w-
group::r--
mask::rw-
other::r--
*/

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];

    acl_t acl1 = acl_get_file(filename, ACL_TYPE_ACCESS);
    if (acl1 == NULL) {
        handle_error("acl_get_file acl1");
    }

    acl_t acl2 = acl_get_file(filename, ACL_TYPE_ACCESS);
    if (acl2 == NULL) {
        handle_error("acl_get_file acl2");
    }    

    set_acl(&acl1, "r", filename);
    set_acl(&acl2, "w", filename);

    return 0;
}

static void set_acl(acl_t *acl, const char *permission, const char *filename)
{
    acl_entry_t entry;    
    if (acl_create_entry(acl, &entry) != 0) {
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
    if (acl_calc_mask(acl) != 0) {
        handle_error("acl_calc_mask");
    }

    char *acl_str;
    acl_str = acl_to_text(*acl, nullptr);
    printf("===>\n%s<===\n", acl_str);
    acl_free(acl_str);

    if (acl_set_file(filename, ACL_TYPE_ACCESS, *acl) != 0) {
        handle_error("acl_set_file");
    }

    acl_free(*acl);
    printf("ACL permission '%s' set successfully for file '%s'\n", permission, filename);    
}
