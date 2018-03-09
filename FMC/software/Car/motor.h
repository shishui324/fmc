#ifndef __MOTOR_H
#define __MOTOR_H
//#include "include.h"
#include "common.h"

//#define EN              PTA5_OUT       //电机使能
//#define DIR             PTA11_OUT       //电机方向 DIR = 1 正 


#define MOTOR_DUTY_MAX       400      //限制占空比，以防电流过大烧驱动
#define MOTOR_DUTY_MIN       -400
//int EN,DIR;
//void motor_init(void);
//void pulse_acc_init(void 
//void motor_set(int16 duty);






void motor_set(void);

//void pid_ctrl(void);


#endif
