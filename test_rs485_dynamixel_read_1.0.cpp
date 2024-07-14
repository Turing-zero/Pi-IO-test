/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2015 Intel Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Example usage: Prints "Hello Mraa!" recursively. Press Ctrl+C to exit
 *
 */

/* standard headers */
#include <signal.h>
#include <rpi_lib.h>
namespace{
    static unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
}
volatile sig_atomic_t flag = 1;
// char TX_PACKET[15]={}

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
    rs485_module *rs485 = new rs485_module(baudrate,UART_DELAY,UART4);
    rs485->open_rs485();

    int index = 0;
    bool send_once=true;
    int count = 0;
    float pingpong_delay_sum = 0; // ms
    float pingpong_delay_ave = 0; // ms
    int pingpong_delay_count = 0;
    int64_t timestamp_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while (flag) {
        // Ping
        int64_t timestamp_ping = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (timestamp_ping - timestamp_start > 1000000 * 60) {
            flag = false;
            std::cout << "test for 1min: finished." << std::endl;
        }

        // std::cout << timestamp_ping << std::endl;
        /* send data through uart */
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
        // char tx_buf[14] = {0xFF,0xFF,0xFD,0x00,0x01,0x07,0x00,0x02,0x84,0x00,0x04,0x00};
        // char tx_buf[25] = {0xFF,0xFF,0xFD,0x00,0x01,0x03,0x00,0x01,0x19,0x4E};
        // tx_buf[0] = 0xab;
        // tx_buf[1] = index & 0xff;
        // tx_buf[2] = (index >> 8) & 0xff;
        // unsigned char* tx=(unsigned char*)tx_buf;
        // for(int i =0;i<14;i++){
        //     std::cout<<int(tx[i])<<" ";
        // }
        //  std::cout<<std::endl;
        // unsigned short CRC_result = update_crc(0,(unsigned char*)tx_buf,12);
        // tx_buf[12] = CRC_result&0xff;
        // tx_buf[13] = CRC_result>>8&0xff;
        // std::cout<<int(tx_buf[12])<<" "<<int(tx_buf[13])<<std::endl;
        // tx_buf[3] = timestamp_ping & 0xff;
        // tx_buf[4] = (timestamp_ping >> 8) & 0xff;
        // tx_buf[5] = (timestamp_ping >> 16) & 0xff;
        // tx_buf[6] = (timestamp_ping >> 24) & 0xff;
        char tx_buf[8]={0xFF,0xFF,0x01,0x04,0x02,0x2B,0x01,0xCC}; //protocal 1.0
        // char tx_buf[25] = {0xff};
        // tx_buf[0] = 0xab;
        // tx_buf[1] = index & 0xff;
        // tx_buf[2] = 0xff;//(index >> 8) & 0xff;
        // for(int i=3;i<15;++i)tx_buf[i]=0xA5;
        // if(send_once){
            rs485->send_485packet(tx_buf,8);
            // for(int i=0;i<255555;i++){}
            // if(count>10){
            //    send_once=false;    
            // }
            // count++;
            // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // }
        // std::cout << baudrate<<std::endl;
        // Pong
        char s[256] = "";
        int len = rs485->recv_485packet(s,25);
        // Check ping-pong
        // if (s[0] == 0xFF) {
            // std::cout << "data available: " << (int)(s[0]) << " " << (int)(s[1]) << " " << (int)(s[2]) << " " << index << std::endl;
            // int index_pong = s[1] | (s[2] << 8);
            // if (index == index_pong) {
            //     int64_t timestamp_pong = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            //     int pingpong_delay = (timestamp_pong - timestamp_ping) / 1000.0;
            //     pingpong_delay_sum += pingpong_delay;
            //     pingpong_delay_count++;
            //     // Output
            //     if (pingpong_delay_count % 100 == 0) {
            //         pingpong_delay_ave = pingpong_delay_sum / pingpong_delay_count;
            //         std::cout << "pingpong delay: " << pingpong_delay_ave << "ms" << std::endl;
            //     }
            // }
            std::cout << "recv str: ";
            for(int i =0;i<len;++i){
                std::cout<< int(s[i])<<" " ;
            }
            std::cout<<std::endl;
            // std::cout << "index: " << int(index) << std::endl;
            // std::cout << "indexpong: " << int(index_pong) << std::endl;
        // }

        index++;
        std::this_thread::sleep_for(std::chrono::milliseconds(send_delay));
    }
    //! [Interesting]

    return EXIT_SUCCESS;
}

namespace{
/* MX28 Servo CRC16 function (for Protocol 2.0)*/
static unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
	unsigned short i, j;
 
	unsigned short crc_table[256] = {
		0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
		0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
		0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
		0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
		0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
		0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
		0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
		0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
		0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
		0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
		0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
		0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
		0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
		0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
		0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
		0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
		0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
		0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
		0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
		0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
		0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
		0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
		0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
		0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
		0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
		0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
		0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
		0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
		0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
		0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
		0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
		0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
	};
 
	for(j = 0; j < data_blk_size; j++)
	{
		i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
 
	return crc_accum;
}
}
 