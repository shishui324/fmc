#ifndef __MOTOR_H
#define __MOTOR_H
#include "common.h"



#define MOTOR_DUTY_MAX       500      //����ռ�ձȣ��Է���������������
#define MOTOR_DUTY_MIN       -500

void motor_init(void );

void motor_set(void);

#endif