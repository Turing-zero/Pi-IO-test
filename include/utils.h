#ifndef __UTILS__
#define __UTILS__


#include <stdio.h>
#include <signal.h>

extern volatile sig_atomic_t g_signal_flag;

void init_signal_handler();

#endif