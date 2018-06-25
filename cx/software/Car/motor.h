#ifndef __MOTOR_H
#define __MOTOR_H
#include "common.h"



//#define MOTOR_DUTY_MAX       750      //����ռ�ձȣ��Է���������������
//#define MOTOR_DUTY_MIN       -750
#define EN GPIOB_PDOR&(1<<28)	//PTH4

void motor_init(void );		//�����ʼ��
void pulse_acc_init(void);	//�����ۼӼ�����ʼ��
void motor_set(void);
void motor_pid_caculate(Motor_pid_info *motor_info);

#endif



