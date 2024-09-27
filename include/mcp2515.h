#ifndef __MCP2515_H__
#define __MCP2515_H__

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>

class MCP2515 {
public:
    MCP2515(const char* ifname);
    ~MCP2515();
    bool begin();
    bool sendMessage(uint32_t id, const uint8_t* data, size_t length);
    bool receiveMessage(can_frame& frame);
    void clearCanReceiveBuffer();

private:
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;
};

class VESC: public MCP2515 {
public:
    using MCP2515::MCP2515;
    ~VESC() {};
    bool send_vesc_rpm(uint8_t id, int rpm);
};

#endif // MCP2515_H