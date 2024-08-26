#include "st7735.hpp"

int main(){
    upm::ST7735 st7735(0,0,80*100000,18,22);
    st7735.fillScreen(ST7735_BLUE);
    st7735.refresh();
    

    st7735.setTextWrap(0x0);
    st7735.setCursor(20, 100);
    st7735.setTextColor(ST7735_RED, ST7735_CYAN);
    st7735.setTextSize(1);
    st7735.print("Hello World!");
    st7735.setCursor(2, 50);
    st7735.setTextColor(ST7735_RED, ST7735_YELLOW);
    st7735.setTextSize(2);
    st7735.print("BIG");
    st7735.refresh();
    st7735.drawCircle(12, 110, 10, ST7735_RED);
    st7735.refresh();
    // st7735.drawTriangle(50, 50, 80, 80, 60, 90, ST7735_GREEN);
    // st7735.refresh();

    // while(1){
    //     st7735.refresh();
    //     usleep (100000);
    // }
}