
#include "KEA128_gpio.h"
#include "KEA128_ftm.h"

#include  "motor.h"
#include "control.h"

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


/***********************************************************************
**  函数名称:
**  函数功能:
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void motor_pid_caculate(Motor_pid_info *motor_info)
{
    motor_info->delta_uk[0] = 0;
    motor_info->out_duty = 0;
    uint8 i = 0;
    for(i=9; i>0; i--)			//更新速度偏差队列
    {
        motor_info->error[i] = motor_info->error[i-1];
    }
		
//****************CAUTION*******************/
    motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];
    motor_info->speed_racc = motor_info->present_value[0]-motor_info->present_value[1];//计算电机加速度,即速度微分
		for(i=3;i>0;i--)
		{
			motor_info->delta_uk[i]=motor_info->delta_uk[i-1];
			
		}	

///////以下是前馈-微分先行增量式PID计算
    motor_info->delta_uk[0]= (float)(
                               motor_info->kvff*(motor_info->set_value[0] - motor_info->set_value[1])
                               +	motor_info->kp*(motor_info->error[0] - motor_info->error[1])
                               + 	motor_info->ki*motor_info->error[0]
                               + 	motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]+ motor_info->error[2])
                               -	motor_info->kaff*motor_info->speed_racc
                           );
		


    /*------------------------遇限削弱抗积分饱和等处理-----------------------*/
    if(motor_info->last_uk > MOTOR_DUTY_MAX)
        if(motor_info->delta_uk[0] > 0)
            motor_info->delta_uk [0]= 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
    if(motor_info->last_uk < MOTOR_DUTY_MIN)
        if(motor_info->delta_uk[0] < 0)
            motor_info->delta_uk[0] = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
    /*------------------------遇限削弱抗积分饱和等处理结束-------------------*/
		motor_info->delta_uk[0]=0.7f*motor_info->delta_uk[0]+0.2f*motor_info->delta_uk[1]+0.1f*motor_info->delta_uk[2];
		
    motor_info->out_duty = motor_info->last_uk + motor_info->delta_uk[0];//μ±?°Dèòaê?3?μ?êμ?ê????±è

    /*-------------------------限制 实际最后总输出的幅值---------------------*/
    if (motor_info->out_duty > MOTOR_DUTY_MAX)
        motor_info->out_duty = MOTOR_DUTY_MAX;
    else if (motor_info->out_duty < MOTOR_DUTY_MIN)
        motor_info->out_duty = MOTOR_DUTY_MIN;
    /*-------------------------限制实际最后总输出的幅值结束------------------*/

    motor_info->last_uk = motor_info->out_duty;               //更新上一次的实际控制量输出
    motor_info->output =(int16)(motor_info->out_duty);     //当前占空比输出


    for(i = 9; i> 0; i--)  				//更新速度设定值队列
        motor_info->set_value[i] = motor_info->set_value[i-1];

    for(i= 9; i>0; i--)    				//更新实测速度队列
    {
        motor_info->present_value[i] = motor_info->present_value[i-1];
    }


//    if((motor_info->set_value[0]>0)&&(motor_info->output<0))
//    {
//        motor_info->output=0;
//    }
//    else if((motor_info->set_value[0]<0)&&(motor_info->output>0))
//    {
//        motor_info->output=0;
//    }


}
static float Motor_Error[4]={0}; 
int16_t  Motor_Out_Filter(int16_t motor_out)        
{
  float motor_Out_Filtered; 
  
  Motor_Error[3]=Motor_Error[2];
  Motor_Error[2]=Motor_Error[1];
  Motor_Error[1]=Motor_Error[0];
  Motor_Error[0]=(float)motor_out;
  motor_Out_Filtered=Motor_Error[0]*0.7f+Motor_Error[1]*0.15f+Motor_Error[2]*0.1f+Motor_Error[3]*0.05f;
  return (int16_t)motor_Out_Filtered;
}

 
//int16_t  Motor_Out_Filter(int16_t motor_out)        
//{
//	static float Motor_Error[4];
//  float motor_Out_Filtered; 
//  
//  Motor_Error[3]=Motor_Error[2];
//  Motor_Error[2]=Motor_Error[1];
//  Motor_Error[1]=Motor_Error[0];
//  Motor_Error[0]=(float)motor_out;
//  motor_Out_Filtered=Motor_Error[0]*0.7f+Motor_Error[1]*0.15f+Motor_Error[2]*0.1f+Motor_Error[3]*0.05f;
//  return (int16_t)motor_Out_Filtered;
//}



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


    if(STOP_CAR_FLAG == true)
    {
        ftm_pwm_duty(ftm2,ftm_ch5,0);
        ftm_pwm_duty(ftm2,ftm_ch3,0);
    }
    else
    {
        if(R_out_value>=0)
        {
            gpio_set(H3,1);
            ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //右边电机输出

        }
        else if(R_out_value < 0)
        {
            gpio_set(H3,0);
            ftm_pwm_duty(ftm2,ftm_ch3,(-R_out_value)); //右边电机输出

        }
				
        if(L_out_value>=0)
        {

            gpio_set(C0,1);
            ftm_pwm_duty(ftm2,ftm_ch5,L_out_value); //左边电机输出

        }
        else if(L_out_value < 0)
        {
            gpio_set(C0,0);
            ftm_pwm_duty(ftm2,ftm_ch5,(-L_out_value));//左边电机输出
        }
    }
}







