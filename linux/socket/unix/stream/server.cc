/* us_xfr_sv.c */

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define SV_SOCKET_PATH "/tmp/uslibsrv.sock"
#define BUF_SIZE 512
#define BACKLOG 5

static void errExit(const char *msg)
{
    printf("%s error! %s\n", msg, strerror(errno));
    exit(-1);
}

int main(int argc, char **argv)
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];
    
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    if (remove(SV_SOCKET_PATH) == -1 && errno != ENOENT) {
        errExit("remove SV_SOCKET_PATH");
    }
        
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCKET_PATH, sizeof(addr.sun_path) - 1);
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        errExit("bind");
    }
        
    if (listen(sfd, BACKLOG) == -1) {
        errExit("listen");
    }

    for (;;) {
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            errExit("accept");
        }
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            printf("== receive request ==\n%.*s", (int)numRead, buf);
            // do echo
            if (write(cfd, buf, numRead) != numRead) {
                errExit("partial write");
            }
        }
        if (numRead == -1) {
            errExit("read");
        }
        if (close(cfd) == -1) {
            errExit("close");
        }
    }
}
