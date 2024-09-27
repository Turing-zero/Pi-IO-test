#include "mcp2515.h"

MCP2515::MCP2515(const char* ifname) {
    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("Socket");
        throw std::runtime_error("Failed to create CAN socket");
    }

    // 将套接字设置为非阻塞
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    strcpy(ifr.ifr_name, ifname);
    ioctl(sock, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
}

MCP2515::~MCP2515() {
    close(sock);
}

bool MCP2515::begin() {
    // Configure the CAN interface here if needed
    return true; // Return true for success
}

bool MCP2515::sendMessage(uint32_t id, const uint8_t* data, size_t length) {
    if (length > 8) {
        std::cerr << "Data length exceeds 8 bytes" << std::endl;
        return false;
    }

    can_frame frame;
    frame.can_id = id | CAN_EFF_FLAG;
    frame.can_dlc = length;
    std::memcpy(frame.data, data, length);

    if (write(sock, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Write");
        return false;
    }

    return true;
}

bool MCP2515::receiveMessage(can_frame& frame) {
    int nbytes = read(sock, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("Read");
        return false;
    }
    return true;
}

void MCP2515::clearCanReceiveBuffer() {
    int bytes;
    can_frame frame_temp;
    while ((bytes = read(sock, &frame_temp, sizeof(can_frame))) > 0) {
        // 读取所有缓存的消息
    }
    if (bytes < 0 && errno != EAGAIN) {
        std::cerr << "Error clearing receive buffer: " << strerror(errno) << std::endl;
    }
}

bool VESC::send_vesc_rpm(uint8_t id, int rpm) {
    uint8_t data[8] = {0};
    uint32_t id_send = id + 0x300;
    data[0] = ((uint32_t)rpm >> 24) & 0xFF;
    data[1] = ((uint32_t)rpm >> 16) & 0xFF;
    data[2] = ((uint32_t)rpm >> 8) & 0xFF;
    data[3] = (uint32_t)rpm & 0xFF;
    return sendMessage(id_send, data, 8);
}