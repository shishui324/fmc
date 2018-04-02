#ifndef _MYFLASH__H
#define _MYFLASH__H

#include "headfile.h"
#include "KEA128_flash.h"


#define  Left_Motor_Erase			255	//����PID������ʹ�õ�����
#define  Right_Motor_Erase		254	//�ҵ��PID������ʹ�õ�����
#define  Servo_Erase					253	//����PID������ʹ�õĵ�����
#define  Speed_Erase          252 //�ٶȲ���


uint8 Save_Left_motor(void);
void Read_Left_motor(void);
uint8 Save_Right_motor(void);
void Read_Right_motor(void);
uint8 Save_Servo(void);
void Read_Servo(void);
void Read_Servo(void);
uint8 Save_Speed(void);
void Read_Speed(void);

void Read_all(void);

#endif


