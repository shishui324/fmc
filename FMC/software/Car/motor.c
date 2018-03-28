#include "headfile.h"
#include  "motor.h"


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
/*****************************************************************

//PWM_L_OUT      PTC2        FTM2_CH2
//PWM_R_OUT      PTC3        FTM2_CH3

//DR_R      PTH3
//DR_L      PTC0

******************************************************************************///////



void motor_init(void )			//电机初始化
{ 
		gpio_init(H4,GPO,1);    //初始化EN使能端
	
		gpio_init(H3,GPO,0);		//DR_R	
	
		gpio_init(C0,GPO,0);		//DR_L

//	
		ftm_pwm_init(ftm2,ftm_ch5,15*1000,0);         //左电机初始化 通道2 15Khz		G7
	
	
    ftm_pwm_init(ftm2,ftm_ch3,15*1000,0);         //右电机初始化 通道3 15Khz  C3
	  
	
	
	
}

/**************** FUCK_MY_CAR  ****************************
 *  * 函数名称 ：motor_set
 *  * 函数参数 : int16 duty
 *  * 返回值   : 无
 *  * 函数功能 ：电机调速
 *  * Panda_Lei
 *  * 日    期 : 2017/03/13
********************************************************/


void motor_set(void)
{
	

//	if(STOP_CAR_FLAG == true)
//	{
////		ftm_pwm_duty(ftm2,ftm_ch2,0);
////		ftm_pwm_duty(ftm2,ftm_ch3,0);
////		
////		Motor.set_value[0] = 0;
//	}
//	else
//	{
		if(R_out_value>0)
		{
			if(R_out_value<150)
					R_out_value = 150;
//			if(R_out_value>800)
//					R_out_value = 800;
			gpio_set(H3,1);
			ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //右边电机输出
		
		}
		else if(R_out_value == 0)
		{
			gpio_set(H3,1);
			ftm_pwm_duty(ftm2,ftm_ch3,0); //右边电机输出
			
		}
		else
		{
			if(R_out_value>-100)
					R_out_value = -100;
//			if(R_out_value<-800)
//					R_out_value = -800;
			gpio_set(H3,0);
			ftm_pwm_duty(ftm2,ftm_ch3,-R_out_value); //右边电机输出
//			ftm_pwm_duty(ftm2,ftm_ch3,0); //右边电机输出
		}

		if(L_out_value>0)
		{		
			
			if(L_out_value<100)
					L_out_value = 100;
//			if(L_out_value>800)
//					L_out_value = 800;
			
			gpio_set(C0,1);
			ftm_pwm_duty(ftm2,ftm_ch5,L_out_value); //左边电机输出
			
		}
		else if(L_out_value == 0)
		{
			gpio_set(C0,1);
			ftm_pwm_duty(ftm2,ftm_ch5,L_out_value);//左边电机输出
		}
		else
		{
			if(L_out_value>-100)
					L_out_value = -100;
//			if(L_out_value<-800)
//					L_out_value = -800;
			gpio_set(C0,0);
			ftm_pwm_duty(ftm2,ftm_ch5,-L_out_value);//左边电机输出
		}
//		Motor.set_value[0] = 6;
//	}
	
	
//			gpio_set(C0,(uint16 )flag);
//			ftm_pwm_duty(ftm2,ftm_ch5,L_out_value);//左边电机输出
//			gpio_set(H3,(uint16 )flag);                                         
//			ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //右边电机输出
		
		
		
		
}







