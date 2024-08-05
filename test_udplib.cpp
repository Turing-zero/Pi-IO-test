#include "rpi_lib.h"

#define ip "192.168.31.207"
#define port 5005
int main(){
    udp_module udp(ip,port);
    udp.open_udp();
    char buffer[MAX_UDP_BUFFER_SIZE];
    while(true){
        int n = udp.recv_pkg(buffer);
        if (n < 0) {
            perror("Receive failed");
        }
        buffer[n] = '\0';
        std::cout << "Client: " << buffer << std::endl;
        char *message = "Hello from Raspberry Pi";
        udp.send_pkg(message);
    }
    return 0;
}