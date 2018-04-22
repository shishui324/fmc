#ifndef __MOTOR_H
#define __MOTOR_H
#include "struct.h"

#define MOTOR_DUTY_MAX       500      //����ռ�ձȣ��Է���������������
#define MOTOR_DUTY_MIN       -500

void motor_init(void );
void motor_pid_caculate(Motor_pid_info *motor_info);
int16_t  Motor_Out_Filter(int16_t motor_out) ;
void motor_set(void);


#endif
