#ifndef __DEBUG_H
#define __DEBUG_H

#include "common.h"


void vcan_sendware(void *wareaddr, uint32_t waresize);

void BB_Play(void);
void BB_Cry(uint16 bb_time,uint16 qq_time);
void bling_bling_led3(uint16 bb_time,uint16 qq_time);
void bling_bling_led3_play(void);
void bling_bling_led2(uint16 bb_time,uint16 qq_time);
void bling_bling_led2_play(void);
void bling_bling_led1(uint16 bb_time,uint16 qq_time);
void bling_bling_led1_play(void);
void bling_bling_led0(uint16 bb_time,uint16 qq_time);
void bling_bling_led0_play(void);
void BB_LED_PLAY(void);




#endif


