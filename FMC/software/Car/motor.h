#ifndef __MOTOR_H
#define __MOTOR_H
//#include "include.h"
#include "common.h"

#define EN              PTA5_OUT       //���ʹ��
#define DIR             PTA11_OUT       //������� DIR = 1 �� 

//int EN,DIR;
void motor_init(void);
void pulse_acc_init(void);
//void motor_set(int16 duty);

////������ٶ�
//typedef struct 
//{ 
//  uint8 INH;                    //���ʹ��
//  int16 set_value_temp;         //ң���趨�ٶ���ʱ����
//  int16 output;
//  int16 speed_racc;             //������ٶ�
//  float last_uk;               //��һ�εĿ������������ֵ�������ռ�ձ�/10
//  float  kp;
//  float  ki;
//  float  kd; 
//  float  kvff;      //�ٶ�ǰ��ϵ��
//  float  kaff;      //���ٶ�ϵ��
// 
//  uint16 kvff_param; 
//  int16 present_value[10];  //������ʵ�ʲ��ֵ
//  int16 error[10];          //������
//  int16 set_value[10];      //�ٶ��趨����   
//}Motor_pid_info;             //���PID�������Ϣ����


#define MOTOR_DUTY_MAX       300      //����ռ�ձ�30%���Է���������������
#define MOTOR_DUTY_MIN       -300



void motor_pid_caculate(Motor_pid_info *motor_info);
void pid_ctrl(void);


#endif
