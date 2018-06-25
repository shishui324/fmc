#ifndef __MOTOR_H
#define __MOTOR_H
#include "common.h"



//#define MOTOR_DUTY_MAX       750      //限制占空比，以防电流过大烧驱动
//#define MOTOR_DUTY_MIN       -750
#define EN GPIOB_PDOR&(1<<28)	//PTH4

void motor_init(void );		//电机初始化
void pulse_acc_init(void);	//脉冲累加计数初始化
void motor_set(void);
void motor_pid_caculate(Motor_pid_info *motor_info);

#endif



