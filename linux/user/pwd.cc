#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

/*

pwd.h 是一个C标准库头文件，用于处理用户密码信息。它通常用于UNIX和类UNIX操作系统，
用于获取和管理用户密码文件（通常是 /etc/passwd）中的用户信息。以下是一些 pwd.h 头文件的主要功能：

- 获取用户信息：pwd.h 头文件提供了一些函数和结构体，允许您从密码文件中获取用户的基本信息，
  如用户名、用户ID、主目录、登录Shell等。这对于需要了解当前用户或其他用户的信息的程序非常有用。

- 访问密码文件：通过 getpwent、getpwnam 和 getpwuid 等函数（非线程安全），可以迭代密码文件的记录或根据
  用户名或用户ID来获取特定用户的信息。其中带 _r 后缀的函数是线程安全的。

- 密码加密：pwd.h 中包含 crypt 函数，用于加密密码。这对于验证用户密码或创建新用户帐户时加密密码是必要的。

- 管理用户密码文件：pwd.h 还提供了一些函数，允许您将用户信息写入密码文件，或者从密码文件中删除用户信息。

*/

void print_pw(struct passwd *passwd)
{
    std::cout << "pw_dir " << passwd->pw_dir
        << ", pw_gecos " << passwd->pw_gecos
        << ", pw_gid " << passwd->pw_gid
		<< ", pw_uid "  << passwd->pw_uid
        << ", pw_name " << passwd->pw_name
        << ", pw_password " << passwd->pw_passwd
		<< ", pw_shell " << passwd->pw_shell
        << std::endl;
}

int main(int argc, char **argv)
{
    struct passwd pwbuf;
    struct passwd *pw;
    char buf[1024];

    if (getpwnam_r("anna", &pwbuf, buf, sizeof(buf), &pw) == 0 && pw != NULL) {
		print_pw(&pwbuf);
    } else {
        perror("getpwnam_r");
    }

    if (getpwuid_r(1000, &pwbuf, buf, sizeof(buf), &pw) == 0 && pw != NULL) {
		print_pw(&pwbuf);
    } else {
        perror("getpwuid_r");
    }

    return 0;
}
