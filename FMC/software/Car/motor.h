#ifndef __MOTOR_H
#define __MOTOR_H
//#include "include.h"
#include "common.h"

#define EN              PTA5_OUT       //电机使能
#define DIR             PTA11_OUT       //电机方向 DIR = 1 正 

//int EN,DIR;
void motor_init(void);
void pulse_acc_init(void);
//void motor_set(int16 duty);

////电机，速度
//typedef struct 
//{ 
//  uint8 INH;                    //软件使能
//  int16 set_value_temp;         //遥控设定速度临时变量
//  int16 output;
//  int16 speed_racc;             //电机加速度
//  float last_uk;               //上一次的控制量输出绝对值，即电机占空比/10
//  float  kp;
//  float  ki;
//  float  kd; 
//  float  kvff;      //速度前馈系数
//  float  kaff;      //加速度系数
// 
//  uint16 kvff_param; 
//  int16 present_value[10];  //编码器实际测得值
//  int16 error[10];          //误差队列
//  int16 set_value[10];      //速度设定队列   
//}Motor_pid_info;             //电机PID所需的信息类型


#define MOTOR_DUTY_MAX       300      //限制占空比30%，以防电流过大烧驱动
#define MOTOR_DUTY_MIN       -300



void motor_pid_caculate(Motor_pid_info *motor_info);
void pid_ctrl(void);


#endif
