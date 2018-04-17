#ifndef __CONTROL_H
#define __CONTROL_H


#include "struct.h"
#include "stdbool.h"
#include "headfile.h" 



////extern void speed_set_val(Servo_pid_info *servo,Motor_pid_info *motor);
//extern Servo_info Servo;

extern uint8_t circle_turn;
extern uint8 error_d;

extern bool circle_flag;

extern uint16 circle_distence;
extern uint32 motor_protect_time;//保护参数
//extern Motor_pid_info Motor;
extern Servo_info Servo;
extern Motor_control_info Motor_control;
extern Speed_info Speed;
extern bool STOP_CAR_FLAG;
extern uint32 motor_protect_time;//保护参数
extern int16 R_out_value; //右边pwm输出的值
extern int16 L_out_value; //左边pwm输出的值



void servo_pid_caculate(void);           //差速控


void control(void);  //控制函数
void speed_control(void);





#endif 

