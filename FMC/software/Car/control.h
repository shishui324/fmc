#ifndef __CONTROL_H
#define __CONTROL_H


#include "struct.h"
#include "stdbool.h"
#include "headfile.h" 



////extern void speed_set_val(Servo_pid_info *servo,Motor_pid_info *motor);
//extern Servo_info Servo;
extern uint32 motor_protect_time;//��������
extern Motor_pid_info Motor;
extern Servo_info Servo;
extern Motor_control_info Motor_control;
extern Speed_info Speed;
extern bool STOP_CAR_FLAG;
extern int16 R_out_value; //�ұ�pwm�����ֵ
extern int16 L_out_value; //���pwm�����ֵ

extern int8_t getCountNum_L;  //��ȡ����������ֵ
//extern int8_t getCountNum_R;  //��ȡ����������ֵ

//extern void motor_pid_caculate(Motor_pid_info *motor_info);
void servo_pid_caculate(void);           //���ٿ�
void motor_pid_caculate(Motor_pid_info *motor_info);
void control(void);  //���ƺ���
void speed_control(void);





#endif 

