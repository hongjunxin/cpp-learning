#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct ifreq ifr;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == -1) {
        perror("socket");
        return 1;
    }

    const char *ifname = "eth0";
    if (argc == 2) {
        ifname = argv[1];
    }

    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == -1) {
        perror("ioctl");
        close(sock);
        return 1;
    }

    close(sock);

    printf("Interface flags: 0x%x\n", ifr.ifr_flags);
    if (ifr.ifr_flags & IFF_UP) printf("Interface is up\n");
    if (ifr.ifr_flags & IFF_BROADCAST) printf("Broadcast is enabled\n");
    if (ifr.ifr_flags & IFF_DEBUG) printf("Debugging is enabled\n");
    if (ifr.ifr_flags & IFF_LOOPBACK) printf("Interface is a loopback interface\n");
    if (ifr.ifr_flags & IFF_POINTOPOINT) printf("Interface is a point-to-point link\n");
    if (ifr.ifr_flags & IFF_NOTRAILERS) printf("Avoid trailers\n");
    if (ifr.ifr_flags & IFF_RUNNING) printf("Resources are allocated\n");
    if (ifr.ifr_flags & IFF_NOARP) printf("No ARP protocol\n");
    if (ifr.ifr_flags & IFF_PROMISC) printf("Promiscuous mode is enabled\n");
    if (ifr.ifr_flags & IFF_ALLMULTI) printf("Receive all multicast packets\n");
    if (ifr.ifr_flags & IFF_MASTER) printf("Master of a load balancing bundle\n");
    if (ifr.ifr_flags & IFF_SLAVE) printf("Slave of a load balancing bundle\n");
    if (ifr.ifr_flags & IFF_MULTICAST) printf("Supports multicast\n");
    if (ifr.ifr_flags & IFF_PORTSEL) printf("Can set media type\n");
    if (ifr.ifr_flags & IFF_AUTOMEDIA) printf("Auto media selection active\n");
    if (ifr.ifr_flags & IFF_DYNAMIC) printf("Interface addresses can change\n");

    return 0;
}
