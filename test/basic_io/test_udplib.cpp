#include "lib_io.h"
#include "fstream"
#include "string"

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
        // char *message = "Hello from Raspberry Pi";
        char message[25];
        message[0]=0xab;
        message[1]=0x01;
        message[2]=0x02;
        udp.send_pkg(message);
    }
    return 0;
}