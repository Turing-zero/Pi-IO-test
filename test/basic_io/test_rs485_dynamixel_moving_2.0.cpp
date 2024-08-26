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
    rs485_module rs485(baudrate,UART_DELAY,UART2,TX_RX_SWIO);
    rs485.open_rs485();

    int index = 0;
    bool send_once=true;
    int wait_count=0;
    while (flag) {

        char tx_buf[] = {(char)0xFF,(char)0xFF,(char)0xFD,(char)0x00,(char)0x01,(char)0x07,(char)0x00,(char)0x02,(char)0x84,(char)0x00,(char)0x04,(char)0x00};//read data for dynamixel
        // char tx_buf[] = {0xFF,0xFF,0xFD,0x00,0x01,0x03,0x00,0x08};//reboot
        // char tx_buf[] = {0xFF,0xFF,0xFD,0x00,0x02,0x09,0x00,0x03,0x40,0x00,0x01,0x00,0x00,0x00};//reg-write
        // char tx_buf_d[] = {0xFF,0xFF,0xFD,0x00,0x02,0x09,0x00,0x04,0x74,0x00,0x00,0x00,0x00,0x00};//reg-write
        // char tx_buf_a[] = {0xFF,0xFF,0xFD,0x00,0x02,0x03,0x00,0x05};//action

        int before_crc_bits = sizeof(tx_buf);
        unsigned short CRC_result = rs485.update_crc(0,(unsigned char*)tx_buf,before_crc_bits);
        tx_buf[before_crc_bits] = CRC_result&0xff;
        tx_buf[before_crc_bits+1] = CRC_result>>8&0xff;

        // std::cout << "recv str1: ";
        // for(int i =0;i<before_crc_bits+2;++i){
        //     std::cout<< int(tx_buf[i])<<" " ;
        // }
        // std::cout<<std::endl;

        rs485.send_485packet(tx_buf,before_crc_bits+2);
        // rs485->send_485packet(tx_buf,10);
        char s[256] = "";
        int len= rs485.recv_485packet(s,25);
        std::cout<<len<<std::endl;
        std::cout << "recv str1: ";
        for(int i =0;i<len;++i){
            std::cout<< int(s[i])<<" " ;
        }
        std::cout<<std::endl;

        // int before_crc_bits_d = sizeof(tx_buf_d);
        // unsigned short CRC_result_d = rs485.update_crc(0,(unsigned char*)tx_buf_d,before_crc_bits_d);
        // tx_buf_d[before_crc_bits_d] = CRC_result_d&0xff;
        // tx_buf_d[before_crc_bits_d+1] = CRC_result_d>>8&0xff;

        // rs485.send_485packet(tx_buf_d,before_crc_bits_d+2);

        // int before_crc_bits_a = sizeof(tx_buf_a);
        // unsigned short CRC_result_a = rs485.update_crc(0,(unsigned char*)tx_buf_a,before_crc_bits_a);
        // tx_buf_a[before_crc_bits_a] = CRC_result_a&0xff;
        // tx_buf_a[before_crc_bits_a+1] = CRC_result_a>>8&0xff;

        // rs485.send_485packet(tx_buf_a,8+2);

        // char s1[256] = "";
        // len= rs485.recv_485packet(s1,25);
        // std::cout << "recv str2: ";
        // for(int i =0;i<len;++i){
        //     std::cout<< int(s1[i])<<" " ;
        // }
        // std::cout<<std::endl;

        index++;
        std::this_thread::sleep_for(std::chrono::milliseconds(send_delay));
    }
    //! [Interesting]

    return EXIT_SUCCESS;
}

 