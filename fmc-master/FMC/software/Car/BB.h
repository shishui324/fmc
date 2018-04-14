#ifndef __BB_H
#define __BB_H
#include "headfile.h"

#define BELL_ON   gpio_set(H6,0)
#define BELL_OFF	gpio_set(H6,1)


void Bell_Cry(uint16_t D_ms, uint16_t C_ms);
void Bell_Play(void);




#endif 


