#include "headfile.h"
#include "math.h"
#include "stdbool.h"
#include "control.h"



extern uint8 circle_left_flag;
extern uint8 circle_right_flag;
extern uint8 circle_in;
uint8_t circle_turn =0;
uint16 circle_distence = 0;
bool  circle_flag=false;

uint32 motor_protect_time=0;//保护参数
Servo_info Servo;
Motor_control_info Motor_control;
Speed_info Speed;

int16 R_out_value; 		//右边pwm输出的值
int16 L_out_value; 		//左边pwm输出的值

uint8 error_d=1;


bool STOP_CAR_FLAG = true;


extern void motor_set(void);

void servo_pid_caculate(void)           //差速控制pid
{
	uint8_t i;
			for(i = 9; i > 0; i--)
			{																	//更新偏差队列
			Servo.error[i] = Servo.error[i-1];
			}
			Servo.error[0] =(int16_t)100*(Sensor.twice_uni_ad[1] - Sensor.twice_uni_ad[6]); //求出电感差值
				
	if(Sensor.sum_16_34<80)
	{
	
#if	Protect_ON
		motor_protect_time++;								//保护计数累加
#endif
	}
#if 0
	if(circle_in)
	{
	
		if (circle_left_flag)
		{
			if(circle_turn)
			{
			Servo.output=(int16)(Servo.kp *Servo.error[0] + Servo.kd*5.0* (Servo.error[0]-Servo.error[9]));
			}
			else
					{
						if(Sensor.sub_25[0]<-15)//小于-15
						{
//							circle_turn =1;
//						if (Servo.error[0]>0)
//							{
							circle_turn =1;
							Bell_Cry(500,500);
							
//							}							
						}
						else if((Sensor.sub_25[0]<10))//-15-0
						{
//						
							Servo.output=(int16)((float)((getCountNum_L+getCountNum_R)*2)/10.0f);
							
						}
						
						else 	Servo.output=(int16)(Servo.kp *Servo.error[0] + Servo.kd*5.0* (Servo.error[0]-Servo.error[9]));							
					}
						
		}
		else if(circle_right_flag)
		{
		
			if(circle_turn)
			{
			Servo.output=(int16)(Servo.kp *Servo.error[0] + Servo.kd*5.0* (Servo.error[0]-Servo.error[9]));
			}
			else
					{
						if((Sensor.sub_25[0]>15))
						{
							if (Servo.error[0]<0)
							{
							circle_turn =1;
							Bell_Cry(500,500);
							}	
								
//								circle_turn =1;
						}
						else if (Sensor.sub_25[0]>-10)
							Servo.output=-(int16)((float)((getCountNum_L+getCountNum_R)*2)/12.0f);
						else Servo.output=(int16)(Servo.kp *Servo.error[0] + Servo.kd*5.0* (Servo.error[0]-Servo.error[9]));
		
					}
		}
	
#if 0	
	if(sub_25==0)
	{

		if(abs(sub_25)>15)   //八字电感
//		circle_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>>2;
//		if(circle_distence<25*60)
			Servo.output = 0;
		else if(circle_left_flag)
		{	
			circle_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>>2;
			if(circle_distence<25*50)
				Servo.output=SERVO_LIMIT_VAL;
			else{ 
				circle_distence = 0;
				circle_in = 0;
				circle_left_flag=0;
			}
		}
		else 
		{
			circle_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>>2;
			if(circle_distence<25*50)
				Servo.output=-SERVO_LIMIT_VAL;
			else
			{
				circle_distence = 0;
				circle_in = 0;
				circle_right_flag = 0;
			}
		}
	}
	#endif
		
//		circle_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>>2;
		if((abs(Sensor.sub_25[0])>25&&(Sensor.sum_16_34>250)&&circle_turn)||(circle_distence>20000))
		{
		Bell_Cry(1000,200);
		circle_in = 0;
		circle_left_flag = 0;		
		circle_right_flag = 0;
		circle_turn=0;
		circle_distence=0;
		circle_flag=true ;
		}
		

	
			
	}
	else{
		
		
	#endif

		if((Sensor.sum_16_34<80)&&(Sensor.sum_16_34>40))	//丢线判断
		{	
			
				Bell_Cry(50,50);
//			if(Servo.output>0)
//			{
//				Servo.output=SERVO_LIMIT;
//			}
//			else
//			{
//				Servo.output=-SERVO_LIMIT;
//			}
			
		}
//		else
//		{
	//		//位置式
			Servo.output = (float)(Servo.kp *Servo.error[0] + Servo.ki*(Servo.error[0]-2*Servo.error[1]+Servo.error[2])+Servo.kd* (Servo.error[0]-Servo.error[error_d]));     //5   2
		
	//		/***********差方法************/
			
//		}
	
	if(Servo.output>SERVO_LIMIT)//Servo.output>=0))
			{
				Servo.output = SERVO_LIMIT;
			}
		if(Servo.output<-SERVO_LIMIT)//Servo.output<0))
			{
				Servo.output = -SERVO_LIMIT;
			}
//		}
}





void control(void)  //控制函数
{
	float  out=0;
	if(motor_protect_time>=750)//当保护计数超过限制
	{
		STOP_CAR_FLAG = true;
		motor_protect_time = 0;
//		circle_in = 0;
//		circle_left_flag = 0;	
//		circle_right_flag = 0;
//		circle_turn=0;
//		circle_distence=0;
//		circle_flag=true ;
	}
	
    /*******************问题*****************
                差速是用算差还是算比
    ****************************************/
	Motor_control.Motor_Left_pid.present_value[0] = getCountNum_L;
	Motor_control.Motor_Right_pid.present_value[0] = getCountNum_R;
#if !(DEBUG_ON)
	
#if Protect_ON
	if(STOP_CAR_FLAG==false)
	{
	if(((abs(Motor_control.Motor_Left_pid.present_value[0]-Motor_control.Motor_Left_pid.set_value[0])>10)
		||(abs(Motor_control.Motor_Right_pid.set_value[0]-Motor_control.Motor_Right_pid.present_value[0])>10)))
	{
		motor_protect_time++;
	}
	else if(Sensor.sum_16_34>80)
		{
				motor_protect_time = 0;
		}
	}
#endif 
  servo_pid_caculate();
//	speed_control();

//	
	out = 0.01f*Speed.set_speed_val * Servo.output;  
	Motor_control.Motor_Left_pid.set_value[0] 	= Speed.set_speed_val - (int16_t) out;
	Motor_control.Motor_Right_pid.set_value[0] 	= Speed.set_speed_val + (int16_t) out;
	
#endif
	
	motor_pid_caculate(&Motor_control.Motor_Left_pid);
	motor_pid_caculate(&Motor_control.Motor_Right_pid);
	
	L_out_value = Motor_control.Motor_Left_pid.output;
	R_out_value = Motor_control.Motor_Right_pid.output;
	motor_set();



}




void speed_control(void)
{	
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
//  if(sum_12<10);
//  Motor.set_value[0] = 1;
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

	Speed.set_speed_val= Speed.zhidao_speed_val-speed_dt;
}








