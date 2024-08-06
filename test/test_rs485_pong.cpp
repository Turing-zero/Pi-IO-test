#include <map>
#include <signal.h>
#include "rpi_lib.h"
#define TX_RX_SWIO 40

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    int baudrate = 115200;
    int UART_DELAY = 400;
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        UART_DELAY = std::stoi(argv[2]);
        std::cout << "baudrate: " << baudrate << std::endl;
        std::cout << "uart_delay: " << UART_DELAY << std::endl;
    }
    rs485_module rs485(baudrate,UART_DELAY,UART5,TX_RX_SWIO);
    rs485.open_rs485();

    struct COMM_STATUS {
        int rx_count = 0;
        int tx_count = 0;

        int rx_index_start = 0;
        int rx_index_end = 0;
        int success_rate = 0;
    } comm_status;

    while (flag) {
        char s[256] = "";
        int len=rs485.recv_485packet(s,25);
        if (s[0] == 0xff) {
            std::cout << "data: ";
            for(int i=0;i<len;++i)std::cout <<int(s[i])<<" ";
            std::cout << std::endl;
            char tx_buf[25] = {0xff};
            tx_buf[0] = 0xbc;
            tx_buf[1] = s[1];
            tx_buf[2] = s[2];
            for(int i=3;i<25;++i)tx_buf[i]=s[i];

            rs485.send_485packet(tx_buf,len);
            if (comm_status.rx_count == 0) {
                comm_status.rx_index_start = s[1] | (s[2] << 8);
            } else {
                comm_status.rx_index_end = s[1] | (s[2] << 8);
            }
            comm_status.rx_count++;
            int rx_real_count = comm_status.rx_index_end - comm_status.rx_index_start + 1;
            comm_status.success_rate = comm_status.rx_count * 100 / rx_real_count;
            if (comm_status.rx_count % 100 == 0) {
                std::cout << "uart hit rate: " << (comm_status.success_rate) << std::endl;
            }
            std::cout << "recv str[1]: " << int(s[1]) << std::endl;
            std::cout << "recv str[2]: " << int(s[2]) << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    //! [Interesting]

    return EXIT_SUCCESS;
}
