#include "direction.h"
#include "headfile.h"

Servo_info 		Servo;
extern uint16 ad_avr_val[10];
extern uint16 summer[9];
uint8 circle_turn = 0;
uint8 circle_turn_in = 0;
uint8 circle_out = 0;
uint8 circle_turn_val[4] = {190,175,160,160};
extern uint8 circle_count;
uint8 circle_num = 0;
extern uint8 MOTOR_EN;
extern uint16 mid_sensor_max_distence;


uint8 SERVO_LIMIT_VAL = 0;
extern uint8 stop_car;
uint16 stop_car_count=0;

#define Position_left  0
#define Position_right 1
extern uint8 ramp_in;
char Position = 0;
float R;
uint16_t Car_speed = 15;
uint16_t Ramp_speed = 12;
uint16_t circle_speed = 12;
int8_t TURN_Para = 5;   //13   5
uint16_t TURN_LOSE_VAL = 50;	//150

extern float sum_16_34;
extern float sub_25;
extern int error_24;

char circle_turn_direction[4] = {0,1,0,1};  //0 ：左边还	1 ：右边环
extern uint8 circle_in;
uint16 circle_distence = 0;

uint32 motor_protect_time;//保护参数

/************************************************
环半径	速度	过环中值  溢出距离	过环效果	备注
50cm    <17    	z 180     140		O			?
		<17    	z 170    			S
		17	   	z 190    			O
70cm    <17    	z 180     240		B
				z 160     240       O
*************************************************/

void servo_pid_caculate(void)           //差速控制pid
{
	if(sum_16_34<100																						//丢线
		||(Motor_control.Motor_Left_pid.set_value[0] - Motor_control.Motor_Left_pid.present_value[0])<4		//杜撰
		||(Motor_control.Motor_Right_pid.set_value[0] - Motor_control.Motor_Right_pid.present_value[0]<4))
	{
		motor_protect_time++;
		if(motor_protect_time>1000)
		{
			MOTOR_EN = 0;
			motor_protect_time = 0;
		}
	}
	if(circle_in)
	{
		BB_Cry(100,50);
		if(Servo.middle_val[0]<circle_turn_val[circle_num])
		{
			circle_turn = 1;
		}
		if(Servo.middle_val[0]<70)
		{
			circle_turn_in = 1;
 		}
		if((circle_turn_in&&Servo.middle_val[0]>130)||circle_out)
		{
			circle_out = 1;
			circle_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>>1;
			if(circle_distence>8000)
			{
				circle_num++;
				if(circle_num>4)
					circle_num = 0;
				circle_out = 0;
				circle_count = 0;
				circle_in = 0;
				circle_turn = 0;
				circle_turn_in = 0;
				circle_distence = 0;
				mid_sensor_max_distence = 0;
			}
		}
	}
		if(sum_16_34>TURN_LOSE_VAL)
		{
			if(ad_avr_val[1]>ad_avr_val[5])
				Position = Position_right;
			else
				Position = Position_left;
		}
		if((sum_16_34<10&&ramp_in==0)||(circle_turn&&circle_turn_in==0))		//120  35
		{
			if(circle_turn&&circle_turn_in==0)		//开始转向但没有转进，打死处理，需要再优化，判断溢出距离决定打脚多少，做到平滑过环//QTMD
			{
				Servo.output = ((float)(Servo.kp *error_24) + Servo.kd*5.0* (1.5));     //4  5   2
				motor_protect_time = 0;
		//		/***********差方法************/
				if(Servo.output>SERVO_LIMIT_VAL)
				{
					Servo.output = SERVO_LIMIT_VAL;
				}
				if(Servo.output<-SERVO_LIMIT_VAL)
				{
					Servo.output = -SERVO_LIMIT_VAL;
				}
//				if(circle_turn_direction[circle_num])	//若为1   则是右环
//					Servo.output = -SERVO_LIMIT_VAL;
//				else
//					Servo.output = SERVO_LIMIT_VAL;
			}
			else
			{
				if(Position == Position_right)
					Servo.output=SERVO_LIMIT_VAL;
				else 
					Servo.output=-SERVO_LIMIT_VAL; 
//					Servo.error[0] = Servo.error[1];
//				Servo.output = ((float)(Servo.kp *Servo.error[0]) + Servo.kd*5.0* (Servo.error[0]-Servo.error[9]));     //4  5   2
//				/***********差方法************/
//				if(Servo.output>SERVO_LIMIT_VAL)
//				{
//					Servo.output = SERVO_LIMIT_VAL;
//				}
//				if(Servo.output<-SERVO_LIMIT_VAL)
//				{
//					Servo.output = -SERVO_LIMIT_VAL;
//				}
					BB_Cry(1996,4);
			}
		}
//		if(Sensor.once_uni_ad[7]<60)
//		{
//			if(Sensor.once_uni_ad[7]>50)
//				R = 0.5;
//			else if(Sensor.once_uni_ad[7]>40)
//				R = 0.45;
//			else if(Sensor.once_uni_ad[7]>30)
//				R = 0.4;
//			else
//				R = 0.3;
//		}
		else
		{
	//		//位置式
			Servo.output = ((float)(Servo.kp *Servo.error[0]) + Servo.kd*5.0* (Servo.error[0]-Servo.error[9]));     //4  5   2
			motor_protect_time = 0;
	//		/***********差方法************/
			if(Servo.output>SERVO_LIMIT_VAL)
			{
				Servo.output = SERVO_LIMIT_VAL;
			}
			if(Servo.output<-SERVO_LIMIT_VAL)
			{
				Servo.output = -SERVO_LIMIT_VAL;
			}
		}
//	}
}

void speed_dif(void)
{
	if(stop_car)
	{
//		stop_car_count++;
//		if(stop_car_count>1200)
//		{
//			MOTOR_EN = 0;
//			stop_car_count = 0;
//			stop_car = 0;
//		}
//		BB_Cry(1,0);
//		SERVO_LIMIT_VAL = CAL_SERVO_LIMIT_VAL(10);
		Motor_control.Motor_Left_pid.set_value[0] = 0;
		Motor_control.Motor_Right_pid.set_value[0] = 0;
	}
//	else if(ramp_in)
//	{
//		BB_Cry(50,0);
//		SERVO_LIMIT_VAL = CAL_SERVO_LIMIT_VAL(Ramp_speed);
//		Motor_control.Motor_Left_pid.set_value[0] = Ramp_speed - Servo.output;
//		Motor_control.Motor_Right_pid.set_value[0] = Ramp_speed + Servo.output;
//	}
//	else if(circle_in)
//	{
//		SERVO_LIMIT_VAL = CAL_SERVO_LIMIT_VAL(circle_speed);
//		Motor_control.Motor_Left_pid.set_value[0] = circle_speed - Servo.output;
//		Motor_control.Motor_Right_pid.set_value[0] = circle_speed + Servo.output;
//	}
	else
	{
		SERVO_LIMIT_VAL = CAL_SERVO_LIMIT_VAL(Car_speed);
		Motor_control.Motor_Left_pid.set_value[0] = Car_speed - Servo.output;
		Motor_control.Motor_Right_pid.set_value[0] = Car_speed + Servo.output;
	}
}

uint8 CAL_SERVO_LIMIT_VAL(uint8 Car_speed)
{
	if(Sensor.once_uni_ad[7]>45)
		return (Car_speed-(TURN_Para));  //-5
	else
		return (Car_speed-TURN_Para-1);
}


