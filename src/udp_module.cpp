#include "rpi_lib.h"

udp_module::udp_module(char* ip,int port){
    _ip=ip;
    _port=port;
    client_addr_len = sizeof(client_addr);
}

void udp_module::open_udp(){
    // 创建UDP socket
    if ((_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
    }

    // 配置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(_ip);
    server_addr.sin_port = htons(_port);

    // 绑定socket到指定端口
    if (bind(_sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(_sockfd);
    }

    std::cout << "UDP server is up and listening on port " << _ip << ":" << _port << std::endl;
}

void udp_module::close_udp(){
    close(_sockfd);
}

void udp_module::send_pkg(char *message){
    sendto(_sockfd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&client_addr, client_addr_len);
}

int udp_module::recv_pkg(char *recv_msg){
    memset(recv_msg,0,MAX_UDP_BUFFER_SIZE);
    int n = recvfrom(_sockfd, (char *)recv_msg, MAX_UDP_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_len);
    return n;
}