#include "headfile.h"
#include "control.h"

Speed_info Speed;

extern uint16 Car_speed;
extern uint8 SERVO_LIMIT_VAL;
extern uint16  MOTOR_DUTY_LIMIT;

void speed_control(void)
{
//	Servo.max_dis_err = 0;
//	Servo.max_dis_err = Servo.error[0];
//	if(abs(Servo.max_dis_err) >=Servo.distance_err_max_val)
//		Servo.max_dis_err = Servo.distance_err_max_val;
//	MOTOR_DUTY_LIMIT = ((900-500)*((Servo.distance_err_max_val-abs(Servo.max_dis_err))/Servo.distance_err_max_val))+500;
		
	uint16 speed_dt = 0;
	uint8 i = 0;
  double dis_err_d_sub_val = 0;                                         //偏差变化率                                               //速度增量
  
  Servo.max_dis_err = 0;                                               //偏差
  Servo.max_dis_err_d = 0;                                             //偏差变化率
  
  for(i=0;i<10;i++)                                                     //5cm //寻找最近10cm内单位cm最大偏差
  {
    if(fabs(Servo.dis1cm_err_store[i]) > Servo.max_dis_err)
       Servo.max_dis_err = fabs(Servo.dis1cm_err_store[i]);
  }
  if(Servo.max_dis_err > Servo.distance_err_max_val)                  //最大单位偏差限幅
       Servo.max_dis_err = Servo.distance_err_max_val;
  
  for(i=0;i<10;i++)                                                     //寻找最近22cm内每12cm最大偏差变化率
  {
    dis_err_d_sub_val = Servo.dis1cm_err_store[i]-Servo.dis1cm_err_store[i+12];
    if(fabs(dis_err_d_sub_val) > Servo.max_dis_err_d)
       Servo.max_dis_err_d = fabs(dis_err_d_sub_val);
  }
	if(Servo.max_dis_err_d > Servo.distance_err_d_max_val)              //最大12cm偏差变化率限幅
       Servo.max_dis_err_d = Servo.distance_err_d_max_val;

	speed_dt =  (uint16)((Speed.zhidao_speed_val-Speed.wandao_speed_val)*Servo.max_dis_err/Servo.distance_err_max_val
					 +(Speed.zhidao_speed_val-Speed.cross_speed_val)*Servo.max_dis_err_d/Servo.distance_err_d_max_val);
					   
	if(Speed.cross_speed_val < Speed.wandao_speed_val)           //限制最低速度
	{
	  if(speed_dt > (Speed.zhidao_speed_val-Speed.cross_speed_val))
		speed_dt = Speed.zhidao_speed_val-Speed.cross_speed_val;
	}
	else
	{
	  if(speed_dt > (Speed.zhidao_speed_val-Speed.wandao_speed_val))
		speed_dt = Speed.zhidao_speed_val-Speed.wandao_speed_val;
	}
//	MOTOR_DUTY_LIMIT = Speed.zhidao_speed_val-speed_dt;
	Car_speed = Speed.zhidao_speed_val-speed_dt;
//	SERVO_LIMIT_VAL = CAL_SERVO_LIMIT_VAL(Car_speed);
}










