// nat 穿透只能基于 udp，不能用 tcp，所以 udp 需要在同一个端口进行收发，p2p 通信基础。

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

#define PORT_A          50001
#define PORT_B          50002
#define SERVER_HOST     "::1"
#define BUF_SIZE        40

#define PTS_PASS  0
#define PTS_FAIL  -1

static void run_peer_a(void);
static void run_peer_b(void);

static void errMsg(const char *msg)
{
    printf("Test FAILED. Error at %s\n", msg);
    exit(PTS_FAIL);
}

int main(int argc, char **argv)
{
    int status;
    int ret = PTS_PASS;
   
    if (fork() == 0) {
        run_peer_a();
        return 0;
    }
   
    sleep(1);

    if (fork() == 0) {
        run_peer_b();
        return 0;
    }
   
    while (waitpid(-1, &status, 0) > 0) {
        if (status != 0)
            ret = PTS_FAIL;
        continue;
    }
   
    printf("Test %s\n", ret == PTS_PASS ? "PASSED" : "FAILED");
    return ret;
}


static void run_peer_a(void)
{
    struct sockaddr_in myaddr, peer_addr;
    int sfd, j;
    ssize_t num_bytes;
    socklen_t len;
    char cliaddr[INET6_ADDRSTRLEN];
    char buf[BUF_SIZE] = {'\0'};

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd ==  -1)
        errMsg("[peer a] socket()");

    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(PORT_A);

    if (bind(sfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1) {
        errMsg("[peer a] bind()");
    }

    len = sizeof(struct sockaddr_in);
    num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &peer_addr, &len);
    if (num_bytes == -1)
        errMsg("[peer a] recvfrom()");        

    printf("[peer a] recvfrom() received message: %s\n", buf);
    if (strcmp("b->a", buf) != 0) {
        errMsg("[peer a] received wrong message.");
    }

    if (sendto(sfd, "a->b", sizeof("a->b"), 0,
            (struct sockaddr *) &peer_addr, sizeof(struct sockaddr_in)) != sizeof("a->b")) {
        errMsg("[peer a] sendto()");        
    }
}


static void run_peer_b(void)
{
    struct sockaddr_in myaddr, peer_addr;
    int sfd, j;
    ssize_t num_bytes;
    socklen_t len;
    char cliaddr[INET6_ADDRSTRLEN];
    char buf[BUF_SIZE] = {'\0'};

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd ==  -1)
        errMsg("[peer b] socket()");

    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(PORT_B);

    memset(&peer_addr, 0, sizeof(struct sockaddr_in));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    peer_addr.sin_port = htons(PORT_A);

    if (bind(sfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1) {
        errMsg("[peer b] bind()");
    }

    if (sendto(sfd, "b->a", sizeof("b->a"), 0,
            (struct sockaddr *) &peer_addr, sizeof(struct sockaddr_in)) != sizeof("b->a")) {
        errMsg("[peer b] sendto()");        
    }

    // after sendto A, A got ip:port of B. If A told C about B's ip:port
    // C can send msg to B though B's ip:port   
    printf("[peer b] listen on %d\n", PORT_B);
    len = sizeof(struct sockaddr_in);
    for (;;) {
        num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &peer_addr, &len);
        if (num_bytes == -1) {
            printf("[peer b] recvfrom() failed\n");
            continue;
        }      

        printf("[peer b] recvfrom() %s:%d, msg '%s'\n", 
            inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port), buf);
    }
}
