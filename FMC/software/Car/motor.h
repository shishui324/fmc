#ifndef __MOTOR_H
#define __MOTOR_H
//#include "include.h"
#include "common.h"

//#define EN              PTA5_OUT       //���ʹ��
//#define DIR             PTA11_OUT       //������� DIR = 1 �� 


#define MOTOR_DUTY_MAX       400      //����ռ�ձȣ��Է���������������
#define MOTOR_DUTY_MIN       -400
//int EN,DIR;
//void motor_init(void);
//void pulse_acc_init(void 
//void motor_set(int16 duty);






void motor_set(void);

//void pid_ctrl(void);


#endif
