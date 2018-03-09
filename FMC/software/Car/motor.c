#include "headfile.h"
#include  "motor.h"


/**************** Small_Cap  ****************************
 *  * 函数名称 ：motor_pid_caculate
 *  * 函数参数 : Motor_pid_info *motor_info
 *  * 返回值   : 无
 *  * 函数功能 ：电机PID计算
 *  * Panda_Lei
 *  * 日    期 : 2017/03/13
********************************************************/

//Motor_pid_info Motor;

void motor_set(void)
{
	if(STOP_CAR_FLAG == true)
	{
		ftm_pwm_duty(ftm2,ftm_ch0,0);
		ftm_pwm_duty(ftm2,ftm_ch1,0);
		ftm_pwm_duty(ftm2,ftm_ch2,0);
		ftm_pwm_duty(ftm2,ftm_ch3,0);
		
		Motor.set_value[0] = 0;
	}
	else
	{
		if(R_out_value>0)
		{
			if(R_out_value<100)
					R_out_value = 100;
//			if(R_out_value>800)
//					R_out_value = 800;
			ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //右边电机输出
			ftm_pwm_duty(ftm2,ftm_ch2,0); //右边电机输出
		}
		else if(R_out_value == 0)
		{
			ftm_pwm_duty(ftm2,ftm_ch3,0); //右边电机输出
			ftm_pwm_duty(ftm2,ftm_ch2,0); //右边电机输出
		}
		else
		{
			if(R_out_value>-100)
					R_out_value = -100;
//			if(R_out_value<-800)
//					R_out_value = -800;
			ftm_pwm_duty(ftm2,ftm_ch2,-R_out_value); //右边电机输出
			ftm_pwm_duty(ftm2,ftm_ch3,0); //右边电机输出
		}
		
		if(L_out_value>0)
		{
			if(L_out_value<100)
					L_out_value = 100;
//			if(L_out_value>800)
//					L_out_value = 800;
			ftm_pwm_duty(ftm2,ftm_ch0,L_out_value); //左边电机输出
			ftm_pwm_duty(ftm2,ftm_ch1,0); //左边电机输出
		}
		else if(L_out_value == 0)
		{
			ftm_pwm_duty(ftm2,ftm_ch0,0); //左边电机输出
			ftm_pwm_duty(ftm2,ftm_ch1,0); //左边电机输出
		}
		else
		{
			if(L_out_value>-100)
					L_out_value = -100;
//			if(L_out_value<-800)
//					L_out_value = -800;
			ftm_pwm_duty(ftm2,ftm_ch1,-L_out_value); //左边电机输出
			ftm_pwm_duty(ftm2,ftm_ch0,0); //左边电机输出
		}
//		Motor.set_value[0] = 6;
	}
}




#if 0
/***************  FUCK_MY_CAR  *****************************
 *  * 文件说明：电机初始化及PID计算
		Panda_Lei
 *  * 日    期：2017/11/14
 *  * 版    本：V1.0
 ***********************************************************/


//B车后轮直径56mm，大齿轮106齿,500线编码器齿轮40
//2ms周期,设测得脉冲数为N ,
//转速V  
// 3.6cm/r
//轮子周长17.584cm
//编码器转1圈步进6.64cm
//v= 0.0754*N m/s = 7 .54*N cm/s
//2ms,步进 0.01507cm, 1cm 75个脉冲

//C车
//V = getCountNum/6.16；
/**************** FUCK_MY_CAR  ****************************
 *  * 函数名称 ：motor_init
 *  * 函数参数 : 无
 *  * 返回值   : 无
 *  * 函数功能 ：电机初始化
 *  * Panda_Lei
 *  * 日    期 : 2017/03/13
****************************z****************************/
//void motor_init(void)
//{
//  uint8 i = 0;
//  
//	ftm_pwm_init(ftm0,ftm_ch0,15*1000,0);		//ftm0模块，0通道，50HZ，百分之50的占空比
////  ftm_pwm_init(FTM2, FTM_CH0,5000, 0); //30kHz,duty = 0 ~10000          2017.4.9  5kHZ
////  gpio_init(PTA11,GPO,0);
////  gpio_init(PTA5,GPO,0);
////  port_init_NoALT(PTA11,PULLDOWN);       //配置管脚下拉
////  port_init_NoALT(PTA5,PULLDOWN);
////  
////  for(i = 0; i < 10; i++)
////  {
////    Motor.set_value[i] = 0;
////    Motor.present_value[i] = 0;
////    Motor.error[i] = 0;
////  }
////  Motor.speed_racc = 0;
////  
//}

////脉冲累加计数初始化
//void pulse_acc_init(void)
//{
//   ftm_quad_init(FTM1);                          //正交解码初始化
//   port_init_NoALT(FTM1_QDPHA_PIN,PULLUP);       //配置管脚上拉（编码器模块没有上拉，因此此处需要配置上拉）
//   port_init_NoALT(FTM1_QDPHB_PIN,PULLUP);
//}

/**************** FUCK_MY_CAR  ****************************
 *  * 函数名称 ：motor_set
 *  * 函数参数 : int16 duty
 *  * 返回值   : 无
 *  * 函数功能 ：电机调速
 *  * Panda_Lei
 *  * 日    期 : 2017/03/13
********************************************************/




#endif





