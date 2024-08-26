#include <signal.h>
#include <lib_io.h>
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

    // uart param
    int baudrate = 115200;
    int send_delay = 10;
    int UART_DELAY = 400;
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        send_delay = std::stoi(argv[2]);
        UART_DELAY = std::stoi(argv[3]);
        std::cout << "baudrate: " << baudrate << std::endl;
        std::cout << "send_delay: " << send_delay << std::endl;
        std::cout << "uart_delay: " << UART_DELAY << std::endl;
    }
    rs485_module rs485(baudrate,UART_DELAY,UART4,TX_RX_SWIO);
    rs485.open_rs485();

    int index = 0;
    bool send_once=true;
    int count = 0;

    while (flag) {
        char tx_buf[8]={(char)0xFF,(char)0xFF,(char)0x01,(char)0x04,(char)0x02,(char)0x2B,(char)0x01,(char)0xCC}; //protocal 1.0

        rs485.send_485packet(tx_buf,8);

        // Pong
        char s[256] = "";
        int len = rs485.recv_485packet(s,25);
            std::cout << "recv str: ";
            for(int i =0;i<len;++i){
                std::cout<< int(s[i])<<" " ;
            }
            std::cout<<std::endl;

        index++;
        std::this_thread::sleep_for(std::chrono::milliseconds(send_delay));
    }
    //! [Interesting]

    return EXIT_SUCCESS;
}

 