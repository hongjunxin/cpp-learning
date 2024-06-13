#include <netlink/netlink.h>
#include <netlink/route/link.h>
#include <stdio.h>
#include <net/if.h>

/*

安装 libnl 库
apt-get install libnl-3-dev libnl-genl-3-dev libnl-route-3-dev

头文件在 /usr/include/libnl3 中，所以要将该路径追加到 vscode 配置中

g++ --std=c++20 netinfo.cpp -I/usr/include/libnl3 -lnl-3 -lnl-genl-3 -lnl-route-3

*/

void print_link_flags(struct rtnl_link *link) {
    unsigned int flags = rtnl_link_get_flags(link);    
    if (flags & IFF_UP) printf("Interface is up\n");
    if (flags & IFF_BROADCAST) printf("Broadcast is enabled\n");
    if (flags & IFF_DEBUG) printf("Debugging is enabled\n");
    if (flags & IFF_LOOPBACK) printf("Interface is a loopback interface\n");
    if (flags & IFF_POINTOPOINT) printf("Interface is a point-to-point link\n");
    if (flags & IFF_NOTRAILERS) printf("Avoid trailers\n");
    if (flags & IFF_RUNNING) printf("Resources are allocated\n");
    if (flags & IFF_NOARP) printf("No ARP protocol\n");
    if (flags & IFF_PROMISC) printf("Promiscuous mode is enabled\n");
    if (flags & IFF_ALLMULTI) printf("Receive all multicast packets\n");
    if (flags & IFF_MASTER) printf("Master of a load balancing bundle\n");
    if (flags & IFF_SLAVE) printf("Slave of a load balancing bundle\n");
    if (flags & IFF_MULTICAST) printf("Supports multicast\n");
    if (flags & IFF_PORTSEL) printf("Can set media type\n");
    if (flags & IFF_AUTOMEDIA) printf("Auto media selection active\n");
    if (flags & IFF_DYNAMIC) printf("Interface addresses can change\n");
}

void print_link_kind(struct rtnl_link *link) {
    const char *kind = rtnl_link_get_type(link);
    if (kind) {
        printf("Interface kind: %s\n", kind);
    } else {
        printf("Interface kind: Unknown\n");
    }
}

int main(int argc, char **argv)
{
    struct nl_sock *sock = nl_socket_alloc();
    if (nl_connect(sock, NETLINK_ROUTE) != 0) {
        perror("nl_connect");
        return 1;
    }

    const char *ifname = "eth0";
    if (argc == 2) {
        ifname = argv[1];
    }    

    struct rtnl_link *link;
    if (rtnl_link_get_kernel(sock, 0, ifname, &link) != 0) {
        perror("rtnl_link_get_kernel");
        nl_socket_free(sock);
        return 1;
    }
    
    print_link_flags(link);
    print_link_kind(link);

    nl_socket_free(sock);
    return 0;
}

/*
输出：
=== bond master
Interface is up
Broadcast is enabled
Resources are allocated
Master of a load balancing bundle
Supports multicast
Interface kind: bond

=== bond slave
Interface is up
Broadcast is enabled
Resources are allocated
Slave of a load balancing bundle
Supports multicast
Interface kind: Unknown

flag 中 IFF_RUNNING，网卡有 IP 时，该标志位才会置 1
*/
