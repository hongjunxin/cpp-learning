#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

int main()
{
    struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct if_nameindex *if_nidxs = if_nameindex();
    if (if_nidxs == NULL) {
        perror("if_nameindex");
        return 1;
    }

    for (struct if_nameindex *if_nidx = if_nidxs; if_nidx->if_index != 0; if_nidx++) {
        if (!if_nidx->if_name) {
            continue;
        }
        memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_name, if_nidx->if_name, IFNAMSIZ-1);

        if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
            perror("ioctl");
            return 1;
        }

        // 过滤了 ifconfig down 的接口
        if (ifr.ifr_flags & IFF_UP) {
            printf("Interface: %s\n", ifr.ifr_name);
        }
    }

    if_freenameindex(if_nidxs);
    ::close(fd);

    return 0;
}
