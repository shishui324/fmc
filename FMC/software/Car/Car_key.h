#ifndef	_CAR_KEY_H
#define _CAR_KEY_H


#include "common.h"



#define Key_Front_Read	0x00
#define Key_Back_Read		0x01
#define Key_Left_Read		0x02
#define Key_Right_Read	0x03
#define Key_Middle_Read	0x04
#define Key_Flag5_Read	0x05



#define Key_bit_High		0x00	//Key状态-弹起
#define Key_bit_Low_		0x01	//Key状态-按下去抖检测
#define Key_bit_low		  0x02	//Key状态-按下
#define Key_bit_Acc			0x03	//Key状态-长按
#define Key_bit_Drop		0x04	//Key按下沿
#define Key_bit_Rise		0x05	//Key弹起沿
#define Key_bit_ACCRise	0x06	//Key长按弹起沿

////快捷读Key

//拨码开关
#define Key_Flag1		!((GPIOA->PDIR>>C1)&0x01)
#define Key_Flag2		!((GPIOC->PDIR>>(I5-64))&0x01)
#define Key_Flag3		!((GPIOC->PDIR>>(I6-64))&0x01)
#define Key_Flag4		!((GPIOA->PDIR>>D5)&0x01)
//独立按键
#define Key_Flag5		((GPIOA->PDIR>>D6)&0x01)
// 五向按键 
#define Key_Front		((GPIOA->PDIR>>D4)&0x01)
#define Key_Back		((GPIOA->PDIR>>D2)&0x01)
#define Key_Left		((GPIOB->PDIR>>(G6-32))&0x01)
#define Key_Right		((GPIOA->PDIR>>D3)&0x01)
#define Key_Middle	((GPIOB->PDIR>>(G5-32))&0x01)




void Key_Message_Init(void);					//Key初始化
void Car_Gather_Data_Key(uint8_t time_ms);		//Key检测
uint8_t Key_Inquire_data(uint8_t Key_name);			//KeyKey读取状态
uint8_t Key_Inquire_GD(uint8_t Key_name);			//Key读取电平


#endif
