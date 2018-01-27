#if 0
#include  "motor.h"

/***************  FUCK_MY_CAR  *****************************
 *  * 文件说明：电机初始化及PID计算
		Panda_Lei
 *  * 日    期：2017/11/14
 *  * 版    本：V1.0
 ***********************************************************/

Motor_pid_info Motor;

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
********************************************************/
void motor_init(void)
{
  uint8 i = 0;
  
	ftm_pwm_init(ftm0,ftm_ch0,15*1000,0);		//ftm0模块，0通道，50HZ，百分之50的占空比
//  ftm_pwm_init(FTM2, FTM_CH0,5000, 0); //30kHz,duty = 0 ~10000          2017.4.9  5kHZ
//  gpio_init(PTA11,GPO,0);
//  gpio_init(PTA5,GPO,0);
//  port_init_NoALT(PTA11,PULLDOWN);       //配置管脚下拉
//  port_init_NoALT(PTA5,PULLDOWN);
//  
//  for(i = 0; i < 10; i++)
//  {
//    Motor.set_value[i] = 0;
//    Motor.present_value[i] = 0;
//    Motor.error[i] = 0;
//  }
//  Motor.speed_racc = 0;
//  
}

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

//void motor_set(int16 duty)
//{
//  if(duty > 3000)
//   duty = 3000;
//  if(duty < -3000)
//   duty = -3000;          //限制占空比最大为30%
//  if(duty > 0)
//  {
//    EN = 1;
//    DIR = 0;
//    ftm_pwm_duty(FTM2,FTM_CH0, duty);   
//  }
//  else if( duty == 0)
//  {
//    EN = 0;
//    ftm_pwm_duty(FTM2, FTM_CH0, duty);
//  }
//  else
//  {
//    EN = 1;
//    DIR = 1;
//    ftm_pwm_duty(FTM2, FTM_CH0, (0-duty));
//  }
//}


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
  
  if(Motor.present_value[0]>500)
  {
      Motor.present_value[0] = 0;
  }
  for(i=9;i>0;i--)     //更新偏差队列
  {
    motor_info->error[i] = motor_info->error[i-1];
  }
  motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];


   motor_info->speed_racc = 0;   //计算电机加速度,即速度微分
   for(i=0;i<1;i++)
     motor_info->speed_racc += motor_info->present_value[i];
   for(i=1;i<2;i++)
     motor_info->speed_racc -= motor_info->present_value[i];
   //以下是前馈-微分先行增量式PID计算
   delta_uk = (float)(motor_info->kvff*motor_info->kvff_param*(motor_info->set_value[0] - motor_info->set_value[1])
              +motor_info->kp*(motor_info->error[0] - motor_info->error[1])
              + motor_info->ki*motor_info->error[0]
              + motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]
              + motor_info->error[2])
              -motor_info->kaff*motor_info->speed_racc*0.10
              );
                                                        

  /*------------------------遇限削弱抗积分饱和等处理-----------------------*/
  if(motor_info->last_uk > MOTOR_DUTY_MAX)  //范围-1000~1000
    if(delta_uk > 0)
      delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
  if(motor_info->last_uk < MOTOR_DUTY_MIN)
    if(delta_uk < 0)
      delta_uk = 0;//当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
  /*------------------------遇限削弱抗积分饱和等处理结束-------------------*/
  
  
  out_duty = motor_info->last_uk + delta_uk;//当前需要输出的实际占空比
  
  /*-------------------------限制 实际最后总输出的幅值--------------------*/
  if (out_duty > MOTOR_DUTY_MAX)
     out_duty = MOTOR_DUTY_MAX;
  else if (out_duty < MOTOR_DUTY_MIN)
     out_duty = MOTOR_DUTY_MIN;
  /*-------------------------限制实际最后总输出的幅值结束------------------*/
  
  motor_info->last_uk = out_duty;               //更新上一次的实际控制量输出
  motor_info->output =(int16)(out_duty);     //当前占空比输出
  
  for(i = 9;i > 0;i--)  //更新速度设定值队列
    motor_info->set_value[i] = motor_info->set_value[i-1];
   
  for(i= 9;i>0;i--)    //更新实测速度队列
  {
    motor_info->present_value[i] = motor_info->present_value[i-1];
  } 
  
  motor_set(motor_info->output);

}

#endif




