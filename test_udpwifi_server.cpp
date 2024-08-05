#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5005
#define IP "192.168.31.207"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len = sizeof(client_addr);

    // 创建UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 配置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);

    // 绑定socket到指定端口
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }

    std::cout << "UDP server is up and listening on port " << PORT << std::endl;

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);

        // 接收数据
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_len);
        if (n < 0) {
            perror("Receive failed");
            break;
        }

        buffer[n] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        // 发送回复
        const char *message = "Hello from Raspberry Pi";
        sendto(sockfd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&client_addr, client_addr_len);
    }

    close(sockfd);
    return 0;
}