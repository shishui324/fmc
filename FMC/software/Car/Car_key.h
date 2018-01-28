
#ifndef	_CAR_KEY_H
#define _CAR_KEY_H

//#include "SKEAZ1284.h"
#include "common.h"

#define Key_Flag1_Read	0x00
#define Key_Flag2_Read	0x01
#define Key_Flag3_Read	0x02
#define Key_Flag4_Read	0x03
#define Key_Up_Read			0	
#define Key_Down_Read		1
#define Key_Left_Read	0x06
#define Key_Right_Read	0x07
#define Key_Middle_Read	0x08

#define Key_bit_High		0x00	//Key状态-弹起
#define Key_bit_Low_		0x01	//Key状态-按下去抖检测
#define Key_bit_low		    0x02	//Key状态-按下
#define Key_bit_Acc			0x03	//Key状态-长按
#define Key_bit_Drop		0x04	//Key按下沿
#define Key_bit_Rise		0x05	//Key弹起沿
#define Key_bit_ACCRise		0x06	//Key长按弹起沿

//快捷读Key
//#define Key_Flag1		!(LPC_GPIO_PORT->B[6][29])
//#define Key_Flag2		!(LPC_GPIO_PORT->B[6][27])
//#define Key_Flag3		!(LPC_GPIO_PORT->B[6][28])
//#define Key_Flag4		!(LPC_GPIO_PORT->B[6][23])
#define Key5_Up			!((GPIOA->PDIR>>C2)&0x01)
#define Key5_Down		!((GPIOA->PDIR>>B5)&0x01)
//#define Key5_Left		!(LPC_GPIO_PORT->B[0][11])
//#define Key5_Right		!(LPC_GPIO_PORT->B[6][21])
//#define Key5_Middle		!(LPC_GPIO_PORT->B[6][7])

void Key_Message_Init(void);					//Key???
void Car_Gather_Data_Key(uint8_t time_ms);		//Key??
uint8_t Key_Inquire_data(uint8_t Key_name);			//Key????
uint8_t Key_Inquire_GD(uint8_t Key_name);			//Key????


#endif


