#include "headfile.h"
#include "math.h"
#include "stdbool.h"
#include "control.h"


//extern float sum_16_34;
//extern float sub_25[5];

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

int16 R_out_value; //右边pwm输出的值
int16 L_out_value; //左边pwm输出的值


bool STOP_CAR_FLAG = true;


extern void motor_set(void);

void servo_pid_caculate(void)           //差速控制pid
{
	uint8_t i;
		for(i = 9; i > 0; i--)
			{																	//更新偏差队列
			Servo.error[i] = Servo.error[i-1];
			}
			Servo.error[0] = 100*(Sensor.twice_uni_ad[1] - Sensor.twice_uni_ad[6]); //求出电感差值
		if(Sensor.sum_16_34<80)	//丢线判断
		{	
#if	Protect_ON
		motor_protect_time++;								//保护计数累加
#endif
//			if(abs(Servo.output)<5)
//			{
//				Servo.output=0;
//			}
//			else 
			if(Servo.output<0)
			{
				Servo.output=-30;
			}
			else
			{
			Servo.output=30;
			}			
		}
		else
		{
	//		//位置式
			Servo.output = (float)(Servo.kp*Servo.error[0] + 0.1*Servo.kd*(Servo.error[0]-Servo.error[1]));     //5   2
		
	//		/***********差方法************/
			
		}
//	}
	if(Servo.output>SERVO_LIMIT_VAL)//Servo.output>=0))
			{
				Servo.output = SERVO_LIMIT_VAL;
			}
		if(Servo.output<-SERVO_LIMIT_VAL)//Servo.output<0))
			{
				Servo.output = -SERVO_LIMIT_VAL;
			}
}


void motor_pid_caculate(Motor_pid_info *motor_info)
 {
  motor_info->delta_uk = 0;
  motor_info->out_duty = 0;
  uint8 i = 0;
  for(i=9;i>0;i--)
  {
    motor_info->error[i] = motor_info->error[i-1];
  }
//****************CAUTION*******************/
  motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];


   motor_info->speed_racc = 0;    //计算电机加速度,即速度微分
   for(i=0;i<1;i++)
     motor_info->speed_racc += motor_info->present_value[i];
   for(i=1;i<2;i++)
     motor_info->speed_racc -= motor_info->present_value[i];
///////以下是前馈-微分先行增量式PID计算
   motor_info->delta_uk = (float)(motor_info->kvff*motor_info->kvff_param*(motor_info->set_value[0] - motor_info->set_value[1])
              +motor_info->kp*(motor_info->error[0] - motor_info->error[1])
              + motor_info->ki*motor_info->error[0]
              + motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]
              + motor_info->error[2])
              -motor_info->kaff*motor_info->speed_racc*0.10
              );


  /*------------------------遇限削弱抗积分饱和等处理-----------------------*/
  if(motor_info->last_uk > MOTOR_DUTY_MAX)  
    if(motor_info->delta_uk > 0)
      motor_info->delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
  if(motor_info->last_uk < MOTOR_DUTY_MIN)
    if(motor_info->delta_uk < 0)
      motor_info->delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
  /*------------------------遇限削弱抗积分饱和等处理结束-------------------*/
  
  
  motor_info->out_duty = motor_info->last_uk + motor_info->delta_uk;//μ±?°Dèòaê?3?μ?êμ?ê????±è
  
  /*-------------------------限制 实际最后总输出的幅值---------------------*/
  if (motor_info->out_duty > MOTOR_DUTY_MAX)
     motor_info->out_duty = MOTOR_DUTY_MAX;
  else if (motor_info->out_duty < MOTOR_DUTY_MIN)
     motor_info->out_duty = MOTOR_DUTY_MIN;
  /*-------------------------限制实际最后总输出的幅值结束------------------*/
  
  motor_info->last_uk = motor_info->out_duty;               //更新上一次的实际控制量输出
  motor_info->output =(int16)(motor_info->out_duty);     //当前占空比输出
  
  for(i = 9;i> 0;i--)  //更新速度设定值队列
    motor_info->set_value[i] = motor_info->set_value[i-1];
   
  for(i= 9;i>0;i--)    //更新实测速度队列
  {
		motor_info->present_value[i] = motor_info->present_value[i-1];
  }

//  motor_set(motor_info->output);

}


void control(void)  //控制函数
{
	if(motor_protect_time>=400)//当保护计数超过限制
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
	Motor_control.Motor_Left_pid.present_value[0] = (int16)((float)(0.1*Motor_control.Motor_Left_pid.present_value[0])+(float)(0.9*getCountNum_L));
	Motor_control.Motor_Right_pid.present_value[0] = (int16)((float)(0.1*Motor_control.Motor_Right_pid.present_value[0])+(float)(0.9*getCountNum_R));
#if !(DEBUG_ON)
	
#if Protect_ON
	if(STOP_CAR_FLAG==false)
	{
	if(((abs(Motor_control.Motor_Left_pid.present_value[0]-Motor_control.Motor_Left_pid.set_value[0])>40)
		||(abs(Motor_control.Motor_Right_pid.set_value[0]-Motor_control.Motor_Right_pid.present_value[0])>40)))
	{
		motor_protect_time++;
	}
	else if(Sensor.sum_16_34>100)
		{
				motor_protect_time = 0;
		}
	}
#endif 
  servo_pid_caculate();
//	speed_control();

//	Motor.set_value[0] = 7;
//	
//	Servo.output = Speed.set_speed_val * Servo.output/10.0f;  //0.05
	Motor_control.Motor_Left_pid.set_value[0] 	= Speed.set_speed_val - (int32_t)Servo.output;
	Motor_control.Motor_Right_pid.set_value[0] 	= Speed.set_speed_val + (int32_t)Servo.output;
	
//	if(isLeftlose == true)
//	{
////		Motor_control.Motor_Left_pid.set_value[0] = 0;
//		Motor_control.Motor_Right_pid.set_value[0] = Motor_control.Motor_Right_pid.set_value[0]+12;   //25
//	}
//	else if(isRightlose == true)
//	{
//		Motor_control.Motor_Left_pid.set_value[0] = Motor_control.Motor_Left_pid.set_value[0] + 12;
////		Motor_control.Motor_Right_pid.set_value[0] = 0;
//	}
//	motor_pid_caculate(&Motor);
#endif
	
	motor_pid_caculate(&Motor_control.Motor_Left_pid);
	motor_pid_caculate(&Motor_control.Motor_Right_pid);
	
	L_out_value = Motor_control.Motor_Left_pid.output;
	R_out_value = Motor_control.Motor_Right_pid.output;
	motor_set();
    /********差方法********/
//    R_out_value = Motor.output + Servo.output;
//    L_out_value = Motor.output - Servo.output;
    /**********************/
    
    /********比方法*********
    Servo.output = R_out_value/L_out_value；
    R_out_value+L_out_value；= 400;
    
    ***********************/


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



  #if 0
//void motor_pid_caculate(Motor_pid_info *motor_info)
//{
//  motor_info->delta_uk = 0;
//  motor_info->out_duty = 0;
//  uint8 i = 0;
//  for(i=9;i>0;i--)
//  {
//    motor_info->error[i] = motor_info->error[i-1];
//  }
////  motor_info->present_value[0] = (getCountNum_L + getCountNum_R)/2;			//****************CAUTION*******************/
//  motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];


//   motor_info->speed_racc = 0;    //计算电机加速度,即速度微分
//   for(i=0;i<1;i++)
//     motor_info->speed_racc += motor_info->present_value[i];
//   for(i=1;i<2;i++)
//     motor_info->speed_racc -= motor_info->present_value[i];
//以下是前馈-微分先行增量式PID计算
//   motor_info->delta_uk = (float)(motor_info->kvff*motor_info->kvff_param*(motor_info->set_value[0] - motor_info->set_value[1])
//              +motor_info->kp*(motor_info->error[0] - motor_info->error[1])
//              + motor_info->ki*motor_info->error[0]
//              + motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]
//              + motor_info->error[2])
//              -motor_info->kaff*motor_info->speed_racc*0.10
//              );
//                                                        

//  /*------------------------遇限削弱抗积分饱和等处理-----------------------*/
//  if(motor_info->last_uk > MOTOR_DUTY_MAX)  //·??§-1000~1000
//    if(motor_info->delta_uk > 0)
//      motor_info->delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
//  if(motor_info->last_uk < MOTOR_DUTY_MIN)
//    if(motor_info->delta_uk < 0)
//      motor_info->delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
//  /*------------------------遇限削弱抗积分饱和等处理结束-------------------*/
//  
//  
//  motor_info->out_duty = motor_info->last_uk + motor_info->delta_uk;//μ±?°Dèòaê?3?μ?êμ?ê????±è
//  
//  /*-------------------------限制 实际最后总输出的幅值---------------------*/
//  if (motor_info->out_duty > MOTOR_DUTY_MAX)
//     motor_info->out_duty = MOTOR_DUTY_MAX;
//  else if (motor_info->out_duty < MOTOR_DUTY_MIN)
//     motor_info->out_duty = MOTOR_DUTY_MIN;
//  /*-------------------------限制实际最后总输出的幅值结束------------------*/
//  
//  motor_info->last_uk = motor_info->out_duty;               //更新上一次的实际控制量输出
//  motor_info->output =(int16)(motor_info->out_duty);     //当前占空比输出
//  
//  for(i = 9;i > 0;i--)  //更新速度设定值队列
//    motor_info->set_value[i] = motor_info->set_value[i-1];
//   
//  for(i= 9;i>0;i--)    //更新实测速度队列
//  {
//    motor_info->present_value[i] = motor_info->present_value[i-1];
//  }
//  
////  motor_set(motor_info->output);

//}

/**************** Small_Cap  ****************************
 *  * 函数名称 ：speed_set_val
 *  * 函数参数 : Servo_pid_info *servo,Motor_pid_info *motor
 *  * 返回值   : 无
 *  * 函数功能 ：速度设定
 *  * 日    期 : 2017/04/13
********************************************************/ 
void speed_set_val(Servo_pid_info *servo,Motor_pid_info *motor)
{
  uint8 i = 0;
  double dis_err_d_sub_val = 0;                                         //偏差变化率
  uint16 speed_dt = 0;                                                   //速度增量
  static uint16 speed_dis_encoder = 0;                                  //高速模式累加脉冲数
  
  servo->max_dis_err = 0;                                               //偏差
  servo->max_dis_err_d = 0;                                             //偏差变化率
  
  for(i=0;i<10;i++)                                                     //5cm //寻找最近10cm内单位cm最大偏差
  {
    if(fabs(servo->dis1cm_err_store[i]) > servo->max_dis_err)
       servo->max_dis_err = fabs(servo->dis1cm_err_store[i]);
  }
  if(servo->max_dis_err > servo->distance_err_max_val)                  //最大单位偏差限幅
       servo->max_dis_err = servo->distance_err_max_val;
  
  for(i=0;i<10;i++)                                                     //寻找最近22cm内每12cm最大偏差变化率
  {
    dis_err_d_sub_val = servo->dis1cm_err_store[i]-servo->dis1cm_err_store[i+12];
    if(fabs(dis_err_d_sub_val) > servo->max_dis_err_d)
       servo->max_dis_err_d = fabs(dis_err_d_sub_val);
  }
  if(servo->max_dis_err_d > servo->distance_err_d_max_val)              //最大12cm偏差变化率限幅
       servo->max_dis_err_d = servo->distance_err_d_max_val;

//  /********** 冲出跑道减速 ****************/
//  if(stop_car_flag==1)  
//  {
//     Motor.INH = 0;
//  }
    /********** 启动时开启高速模式时，38个脉冲,5m/s,定距离轰******************/
  if(Speed.speed_high_mode == 1) //开启高速模式
  {
    motor->set_value[0] = 50;
    speed_dis_encoder += Motor.present_value[0];
    if(speed_dis_encoder >= Speed.high_speed_dis*75) //退出高速模式，减速到4m/s 
    {
      Speed.speed_high_mode = 0;
      speed_dis_encoder = 0;
//      Motor.INH = 0;
    }
  }
  /*********** 坡道速度  ********************/

//  else if((RampRoad.ramp_road_flag == 1))
//  {
//     Motor.kvff_param = 100; //前馈系数放大100倍  kvff*kvff_param
//    if(RoadRemember.ramp_road_remember == 0) //说明最近检测的是第一个坡道
//    {
//     motor->set_value[0] = Speed.ramp1_speed_val;
//    }
//    else if(RoadRemember.ramp_road_remember == 1)
//    {
//      motor->set_value[0] = Speed.ramp2_speed_val;
//    }
//    else       //其他取最低的
//    {
//      if(Speed.ramp1_speed_val < Speed.ramp2_speed_val)
//       motor->set_value[0] = Speed.ramp1_speed_val;
//      else
//       motor->set_value[0] = Speed.ramp2_speed_val;
//    }
//    
//    ramp_dis_encoder += Motor.present_value[0];
//    if(ramp_dis_encoder > RampRoad.ramp_shut_dis*75)  //减速距离,75*130cm
//    {
//      ramp_dis_encoder = 0;
//      RampRoad.ramp_road_flag = 0;
//      Motor.kvff_param = 1; //恢复前馈系数 
//      RoadRemember.ramp_road_remember++;//坡道计数
//    }
//  }
//  /********** 弯道接十字 *******************/
//  else if(special_cross_flag == 1)
//  {
//    Motor.set_value[0] = Speed.ramp2_speed_val;
//  }
//  /*********** 环形速度设定 *****************/

 //else if((ZhijiaoRoad.zhijiao_servo_flag == 1)||(zhijiao_speed_time_cnt > 0))
//  else if((ZhijiaoRoad.zhijiao_servo_flag == 1)||(zhijiao_shut_flag == 1))
//  {
// 
//     motor->kvff_param = 100; //前馈系数放大100倍
//     motor->set_value[0] = Speed.zhijiao_speed_val;
//     zhijiao_shut_flag = 1;
////     zhijiao_speed_time_cnt++;   //计时刹车不准，原因至今不清楚
////     if(zhijiao_speed_time_cnt > ZhijiaoRoad.zhijiao_speed_time) //直角减速时间
////     {
////       Motor.kvff_param = 1;
////       zhijiao_speed_time_cnt = 0;
////     }
//     zhijiao_dis_encoder += Motor.present_value[0];
//     if(zhijiao_dis_encoder > ZhijiaoRoad.zhijiao_speed_time*75)  //100cm减速
//     {
//        motor->kvff_param = 1;   //恢复前馈系数
//        zhijiao_dis_encoder = 0;
//        zhijiao_shut_flag = 0;
//     }
//  }

 /************** 正常速度设定 ***************/ 
     else
    {
      Motor.kvff_param = 1;

      speed_dt =  (uint16)((Speed.zhidao_speed_val-Speed.wandao_speed_val)*servo->max_dis_err/servo->distance_err_max_val
                         +(Speed.zhidao_speed_val-Speed.cross_speed_val)*servo->max_dis_err_d/servo->distance_err_d_max_val);
                           
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
      
       motor->set_value[0] = Speed.zhidao_speed_val-speed_dt;
     }

}

#endif








