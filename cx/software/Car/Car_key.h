#ifndef	_CAR_KEY_H
#define _CAR_KEY_H

//#include "SKEAZ1284.h"
#include "common.h"



#define Key_Front_Read	0x00
#define Key_Back_Read	0x01
#define Key_Left_Read	0x02
#define Key_Right_Read	0x03
#define Key_Middle_Read	0x04
#define Key_Flag5_Read	0x05



#define Key_bit_High		0x00	//Key״̬-����
#define Key_bit_Low_		0x01	//Key״̬-����ȥ�����
#define Key_bit_low		    0x02	//Key״̬-����
#define Key_bit_Acc			0x03	//Key״̬-����
#define Key_bit_Drop		0x04	//Key������
#define Key_bit_Rise		0x05	//Key������
#define Key_bit_ACCRise		0x06	//Key����������

////��ݶ�Key

//���뿪��
#define Key_Flag1		gpio_get(I6)//((GPIOA->PDIR>>D7)&0x01)
#define Key_Flag2		gpio_get(D5)//((GPIOC->PDIR>>D6)&0x01)
#define Key_Flag3		gpio_get(D6)//((GPIOC->PDIR>>(D5))&0x01)
#define Key_Flag4		gpio_get(D7)//((GPIOA->PDIR>>(I6-64))&0x01)
//��������
#define Key_Flag5		gpio_get(E1)//((GPIOA->PDIR>>D6)&0x01)
// ���򰴼� 
#define Key_Front		gpio_get(C3)//((GPIOA->PDIR>>D4)&0x01)
#define Key_Back		gpio_get(E6)//((GPIOA->PDIR>>D2)&0x01)
#define Key_Left		gpio_get(B5)//((GPIOB->PDIR>>(G6-32))&0x01)
#define Key_Right		gpio_get(C2)//((GPIOA->PDIR>>D3)&0x01)
#define Key_Middle		gpio_get(E5)//((GPIOB->PDIR>>(G5-32))&0x01)

void Key_Message_Init(void);					//Key��ʼ��
void Car_Gather_Data_Key(uint8_t time_ms);		//Key���
uint8_t Key_Inquire_data(uint8_t Key_name);			//KeyKey��ȡ״̬
uint8_t Key_Inquire_GD(uint8_t Key_name);			//Key��ȡ��ƽ


#endif
