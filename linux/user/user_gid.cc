#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <vector>

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

std::vector<gid_t> get_user_groups(uid_t uid) {
    struct passwd *pwd = getpwuid(uid);
    if (pwd == NULL) {
        handle_error("getpwuid");
    }

    const char *username = pwd->pw_name;
    printf("username %s\n", username);
    gid_t gid = pwd->pw_gid;

    int ngroups = 0;
    //获取用户的所有组 GID，包括主组和附加组
    getgrouplist(username, gid, nullptr, &ngroups);

    std::vector<gid_t> all_groups(ngroups);
    if (getgrouplist(username, gid, all_groups.data(), &ngroups) == -1) {
        handle_error("getgrouplist");
    }
    return all_groups;
}

int main() {
    uid_t uid = 1004;  // 替换为实际的 UID

    std::vector<gid_t> groups = get_user_groups(uid);

    std::cout << "GID list for UID " << uid << ": ";
    for (gid_t gid : groups) {
        std::cout << gid << " ";
    }
    std::cout << std::endl;

    return 0;
}
