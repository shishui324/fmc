#ifndef UI_H
#define UI_H

#include "chip.h"


extern uint8_t UI_Kind_Catalog;


/*
 *	UI初始化
 */
void UI_OLED_Init(void);

/*
 *	UI扫描
 *	参数：nop
 */
void UI_OLED_play(uint16_t time);

#endif

