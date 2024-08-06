#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
#include <pigpio.h>
#include <unistd.h>
// using namespace std;
/*
   gcc -pthread -o mcp3008 mcp3008.c -lpigpio
*/

int main(int argc, char *argv[]){
   int i;
   int h;
   double v;
   int loops;
   int speed;
   double start, diff, sps;
   char buf[2];
   char ref[2];

   if (argc > 1) loops = atoi(argv[1]);
   else loops = 10000;

   if (argc > 2) speed = atoi(argv[2]);
   else speed = 20000000;

   if (gpioInitialise() < 0) return 1;

   h = spiOpen(0, speed, 0);

   if (h < 0) return 2;

   start = time_time();
    usleep(1800);
    diff = time_time() - start;
    printf("%.6f\n",diff);
   for (i=0; i<loops; i++)
   {
      uint16_t config = 0xF120;
      buf[0] = 0xF1;
      buf[1] = 0x20;
      spiXfer(h, buf, ref, 2);

      v = (ref[0]<<8 | ref[1])/65535*4.096;
      // printf("%f\n", v);
   }

   diff = time_time() - start;
   printf("%.6f",diff);

   fprintf(stderr, "sps=%.1f @ %d bps (%d/%.1f)\n",
      (double)loops / diff, speed, loops, diff);

    spiClose(h);

    gpioTerminate();

    return 0;
}