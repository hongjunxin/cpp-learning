#define _XOPEN_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

// from /etc/shadow
// eg: anna:$y$j9T$UGoxjqE/1ylvhX4ropgoB1$a.2o4TNHSsX3f1fTPE3ImptSwn8tJaeY.gWMpKEiZ.4:19545:0:99999:7:::
char passwd_hash[] = "$y$j9T$UGoxjqE/1ylvhX4ropgoB1$a.2o4TNHSsX3f1fTPE3ImptSwn8tJaeY.gWMpKEiZ.4";

// password of user anna
const char *password = "123456";

int main(int argc, char *argv[])
{
    char *p = strrchr(passwd_hash, '$');
    if (!p)
        return false;

    *p = '\0';

    // crypt not thread safe, should add lock
    printf("passwd_hash trim: %s\n", passwd_hash);
    char *encrypted_password = crypt(password, passwd_hash);

    *p = '$';

    if (strcmp(encrypted_password, passwd_hash) == 0)
        printf("check ok\n");
    else
        printf("check failed\n");
}
