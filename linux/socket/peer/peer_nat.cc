// nat 穿透一般基于 udp，不能用 tcp，所以 udp 需要在同一个端口进行收发，p2p 通信基础。

// 测试步骤：

// 机器 A 部署在公网，ip *.*.*.22
// $ ./peer.out
// [server] listen on 19001
// [server] recvfrom() *.*.*.249:9235, msg 'b->a', echo back

// 机器 B 部署在内网，公网 ip *.*.*.249
// ./peer.out -t client -p 19000 -s *.*.*.22 -q 19001
// [client] send to *.*.*.22:19001, msg 'b->a'
// [client] listen on 19000
// [client] recv from *.*.*.22:19001, msg 'b->a'
// [client] recv from *.*.*.22:19001, msg 'b->a' (PS：case 2 中 A 发送的消息)

// 从 A 的 log 中看出，此时 B 的内网端口 19000 映射的公网端口为 9235

// 此时
// case 1：
// 机器 A 使用另一端口向机器 B 的公网端口 9235 发送消息，测试结果 B 无法收到。
// case 2：
// 机器 A 停止 ./peer.out 程序，然后执行
// ./peer.out -t client -p 19001 -s *.*.*.249 -q 9235
// 也就是用相同的端口 19001 向机器 B 的公网端口 9235 发送消息，测试结果 B 能接收到

// case 1、2 测试结果说明，机器 B 的 NAT 模型是 端口限制锥型

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE        40

#define PTS_PASS  0
#define PTS_FAIL  -1

#define EP_CLIENT  0
#define EP_SERVER  1

static void run_udp_server(void);
static void run_udp_client(void);

static int endpoint_type = EP_SERVER;
static char server_ip[16] = {'\0'};
static int client_port = 19000;
static int server_port = 19001;

static void errMsg(const char *msg)
{
    printf("Test FAILED. Error at %s\n", msg);
    exit(PTS_FAIL);
}

void parse_arg(int argc, char * argv[])
{
    int ch;

    while ((ch = getopt(argc, argv, "t:s:p:q:")) != -1)
    {
        switch (ch) {
        case 't':
            if (strcmp("client", optarg) == 0) {
                endpoint_type = EP_CLIENT;
            } else {
                endpoint_type = EP_SERVER;
            }
            break;
        case 's':
            strcpy(server_ip, optarg);
            break;
        case 'p':
            client_port = atoi(optarg);
            break;
        case 'q':
            server_port = atoi(optarg);
            break;

        case '?':
            printf("Unknown option: %c\n\n",(char)optopt);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    int status;
    int ret = PTS_PASS;

    parse_arg(argc, argv);

    if (endpoint_type == EP_CLIENT) {
        run_udp_client();
    } else {
        run_udp_server();
    }

    return ret;
}

static void run_udp_server(void)
{
    struct sockaddr_in myaddr, peer_addr;
    int sfd, j;
    ssize_t num_bytes;
    socklen_t len;
    char buf[BUF_SIZE] = {'\0'};

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd ==  -1)
        errMsg("[server] socket()");

    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(server_port);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1) {
        errMsg("[server] bind()");
    }

    printf("[server] listen on %d\n", server_port);
    len = sizeof(struct sockaddr_in);
    for (;;) {
        num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &peer_addr, &len);
        if (num_bytes == -1) {
            printf("[server] recvfrom() failed\n");
            continue;
        }      

        printf("[server] recvfrom() %s:%d, msg '%s', echo back\n", 
            inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port), buf);

        // echo
        if (sendto(sfd, buf, num_bytes, 0,
                (struct sockaddr *) &peer_addr, sizeof(struct sockaddr_in)) != num_bytes) {
            errMsg("[server] sendto()");        
        }
    }
}

static void run_udp_client(void)
{
    struct sockaddr_in myaddr, peer_addr;
    int sfd, j;
    ssize_t num_bytes;
    socklen_t len;
    char buf[BUF_SIZE] = {'\0'};

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd ==  -1)
        errMsg("[peer] socket()");

    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(client_port);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(&peer_addr, 0, sizeof(struct sockaddr_in));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &peer_addr.sin_addr);

    if (bind(sfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1) {
        errMsg("[client] bind()");
    }

    if (sendto(sfd, "b->a", sizeof("b->a"), 0,
            (struct sockaddr *) &peer_addr, sizeof(struct sockaddr_in)) != sizeof("b->a")) {
        errMsg("[client] sendto()");        
    }
    printf("[client] send to %s:%d, msg '%s'\n",
        inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port), "b->a");

    // after sendto A, A got ip:port of B. If A told C about B's ip:port
    // C can send msg to B though B's ip:port   
    printf("[client] listen on %d\n", client_port);
    len = sizeof(struct sockaddr_in);
    for (;;) {
        num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &peer_addr, &len);
        if (num_bytes == -1) {
            printf("[client] recvfrom() failed\n");
            continue;
        }      

        printf("[client] recv from %s:%d, msg '%s'\n",
            inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port), buf);
    }
}
