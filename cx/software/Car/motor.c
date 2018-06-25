#include "headfile.h"
#include  "motor.h"

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
uint16  MOTOR_DUTY_LIMIT = 700;     //限制占空比，以防电流过大烧驱动
extern uint8 MOTOR_EN;

//int16_t MOTOR_DUTY_MAX;
//int16_t	MOTOR_DUTY_MIN;

Motor_control_info Motor_control;

uint32 speed_correction_count = 0;

void motor_init(void)						//电机初始化
{ 
	gpio_init(G5,GPO,0);    				//初始化EN使能端
	gpio_init(F0,GPO,0);					//DR_R
	gpio_init(G4,GPO,0);					//DR_L
	ftm_pwm_init(ftm2,ftm_ch1,15*1000,300);   //右电机初始化 通道3 15Khz  	F1
	ftm_pwm_init(ftm2,ftm_ch4,15*1000,300);   //左电机初始化 通道2 15Khz		G6
}

void pulse_acc_init(void)	//脉冲累加计数初始化
{
    ftm_count_init(ftm0);   //左编码器
    gpio_init(H5,GPI,0);    //DIR_L始化
    port_pull(H5);

    ftm_count_init(ftm1);   //右编码器
    gpio_init(C5,GPI,0);    //DIR_R初始化
    port_pull(C5);
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
	if(MOTOR_EN)
		gpio_set(G5,1);
	else 
		gpio_set(G5,0);
		
	if(Motor_control.Motor_Right_pid.output>=0)
	{
		gpio_set(F0,0);
//		if(Motor_control.Motor_Right_pid.output<100)
//			Motor_control.Motor_Right_pid.output = 100;
		ftm_pwm_duty(ftm2,ftm_ch1,Motor_control.Motor_Right_pid.output); //右边电机输出
	}
	else
	{
		gpio_set(F0,1);
//		if(Motor_control.Motor_Right_pid.output>-100)					  //死区
//			Motor_control.Motor_Right_pid.output = -100;
		ftm_pwm_duty(ftm2,ftm_ch1,-Motor_control.Motor_Right_pid.output); //右边电机输出
		
	}
	if(Motor_control.Motor_Left_pid.output>=0)
	{		
		gpio_set(G4,1);
//		if(Motor_control.Motor_Left_pid.output<100)
//			Motor_control.Motor_Left_pid.output = 100;
		ftm_pwm_duty(ftm2,ftm_ch4,Motor_control.Motor_Left_pid.output); //左边电机输出
	}
	else
	{
		gpio_set(G4,0);
//		if(Motor_control.Motor_Left_pid.output>-100)
//			Motor_control.Motor_Left_pid.output = -100;
		ftm_pwm_duty(ftm2,ftm_ch4,-Motor_control.Motor_Left_pid.output);//左边电机输出
	}
}

/**************** Small_Cap  ****************************
 *  * 函数名称 ：motor_pid_caculate
 *  * 函数参数 : Motor_pid_info *motor_info
 *  * 返回值   : 无
 *  * 函数功能 ：电机PID计算
 *  * Panda_Lei
 *  * 日    期 : 2017/03/13
********************************************************/
void motor_pid_caculate(Motor_pid_info *motor_info)
{
	float delta_uk = 0;//当前实际应输出的占空比，是delta_duty加last_uk
	float out_duty = 0;
	uint8 i = 0;

	for(i=9;i>0;i--)     //更新偏差队列
		motor_info->error[i] = motor_info->error[i-1];
	motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];

	motor_info->speed_racc = 0;   //计算电机加速度,即速度微分
	for(i=0;i<1;i++)
		motor_info->speed_racc += motor_info->present_value[i];
	for(i=1;i<2;i++)
		motor_info->speed_racc -= motor_info->present_value[i];
	//以下是前馈-微分先行增量式PID计算
//	delta_uk = (float)(motor_info->kvff*motor_info->kvff_param*(motor_info->set_value[0] - motor_info->set_value[1])
//			  +motor_info->kp*(motor_info->error[0])// - motor_info->error[1]
//			  + motor_info->ki*(motor_info->error[0]+motor_info->error[1]+motor_info->error[2]
//			  +motor_info->error[3]+motor_info->error[4]+motor_info->error[5])
////			  + //)motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]
//			  //+ motor_info->error[2]
////			  -motor_info->kaff*motor_info->speed_racc*0.10
//			  );
	delta_uk = (float)(motor_info->kvff*motor_info->kvff_param*(motor_info->set_value[0] - motor_info->set_value[1])
              +motor_info->kp*(motor_info->error[0] - motor_info->error[1])
              + motor_info->ki*motor_info->error[0] 
              + motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]
              + motor_info->error[2])
              -motor_info->kaff*motor_info->speed_racc*0.10
              );
														

	/*------------------------遇限削弱抗积分饱和等处理-----------------------*/
	if(motor_info->last_uk > MOTOR_DUTY_LIMIT)  //范围-1000~1000
		if(delta_uk > 0)
			delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
	if(motor_info->last_uk < -800)//MOTOR_DUTY_LIMIT
		if(delta_uk < 0)
			delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
	/*------------------------遇限削弱抗积分饱和等处理结束-------------------*/


	out_duty = motor_info->last_uk + delta_uk;//当前需要输出的实际占空比
		
		
		
		
		out_duty=0.9f*out_duty+0.1f*motor_info->last_uk;
		
		
		

	/*-------------------------限制 实际最后总输出的幅值--------------------*/
	if (out_duty > MOTOR_DUTY_LIMIT)
		out_duty = MOTOR_DUTY_LIMIT;
	else if (out_duty < -800)
		out_duty = -800;
	/*-------------------------限制实际最后总输出的幅值结束------------------*/

	motor_info->last_uk = out_duty;               //更新上一次的实际控制量输出
	motor_info->output =(int16)(out_duty);     //当前占空比输出

	for(i = 9;i > 0;i--)  //更新速度设定值队列
		motor_info->set_value[i] = motor_info->set_value[i-1];

	for(i= 9;i>0;i--)    //更新实测速度队列
		motor_info->present_value[i] = motor_info->present_value[i-1];
}

void speed_correction(void)		//两个磁铁相距1m,实测其1m脉冲数
{
	speed_correction_count += Motor_control.Motor_Left_pid.present_value[0] + Motor_control.Motor_Right_pid.present_value[0];
}



