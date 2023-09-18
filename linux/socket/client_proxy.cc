#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PROXY_IP "192.168.10.11" // 代理服务器的IP地址
#define PROXY_PORT 7890            // 代理服务器的端口号

int main()
{
    int clientSocket;
    struct sockaddr_in proxyAddr;
    char httpRequest[1024];

    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // 设置代理服务器地址
    proxyAddr.sin_family = AF_INET;
    proxyAddr.sin_port = htons(PROXY_PORT);
    proxyAddr.sin_addr.s_addr = inet_addr(PROXY_IP);

    // 连接到代理服务器
    if (connect(clientSocket, (struct sockaddr*)&proxyAddr, sizeof(proxyAddr)) == -1) {
        perror("Failed to connect to proxy server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    char *url = "https://www.google.com/index.m3u8";
    char *dst_host = "www.google.com";

    // 构建HTTP请求
    // GET 后面的 path，要加上完整的协议和 host/port，即 http://dst-host/paths
    snprintf(httpRequest, sizeof(httpRequest),
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Wget/1.20.3 (linux-gnu)\r\n"
        "Accept: */*\r\n"
        "Connection: Keep-Alive\r\n\r\n", 
        url,
        dst_host);

    // 发送HTTP请求
    if (send(clientSocket, httpRequest, strlen(httpRequest), 0) == -1) {
        perror("Failed to send HTTP request");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // 接收并打印响应
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    // 关闭套接字
    close(clientSocket);

    return 0;
}
