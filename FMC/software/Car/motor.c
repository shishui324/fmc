#if 0
#include  "motor.h"

/***************  FUCK_MY_CAR  *****************************
 *  * �ļ�˵���������ʼ����PID����
		Panda_Lei
 *  * ��    �ڣ�2017/11/14
 *  * ��    ����V1.0
 ***********************************************************/

Motor_pid_info Motor;

//B������ֱ��56mm�������106��,500�߱���������40
//2ms����,����������ΪN ,
//ת��V  
// 3.6cm/r
//�����ܳ�17.584cm
//������ת1Ȧ����6.64cm
//v= 0.0754*N m/s = 7 .54*N cm/s
//2ms,���� 0.01507cm, 1cm 75������

//C��
//V = getCountNum/6.16��
/**************** FUCK_MY_CAR  ****************************
 *  * �������� ��motor_init
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� �������ʼ��
 *  * Panda_Lei
 *  * ��    �� : 2017/03/13
********************************************************/
void motor_init(void)
{
  uint8 i = 0;
  
	ftm_pwm_init(ftm0,ftm_ch0,15*1000,0);		//ftm0ģ�飬0ͨ����50HZ���ٷ�֮50��ռ�ձ�
//  ftm_pwm_init(FTM2, FTM_CH0,5000, 0); //30kHz,duty = 0 ~10000          2017.4.9  5kHZ
//  gpio_init(PTA11,GPO,0);
//  gpio_init(PTA5,GPO,0);
//  port_init_NoALT(PTA11,PULLDOWN);       //���ùܽ�����
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

////�����ۼӼ�����ʼ��
//void pulse_acc_init(void)
//{
//   ftm_quad_init(FTM1);                          //���������ʼ��
//   port_init_NoALT(FTM1_QDPHA_PIN,PULLUP);       //���ùܽ�������������ģ��û����������˴˴���Ҫ����������
//   port_init_NoALT(FTM1_QDPHB_PIN,PULLUP);
//}

/**************** FUCK_MY_CAR  ****************************
 *  * �������� ��motor_set
 *  * �������� : int16 duty
 *  * ����ֵ   : ��
 *  * �������� ���������
 *  * Panda_Lei
 *  * ��    �� : 2017/03/13
********************************************************/

//void motor_set(int16 duty)
//{
//  if(duty > 3000)
//   duty = 3000;
//  if(duty < -3000)
//   duty = -3000;          //����ռ�ձ����Ϊ30%
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
 *  * �������� ��motor_pid_caculate
 *  * �������� : Motor_pid_info *motor_info
 *  * ����ֵ   : ��
 *  * �������� �����PID����
 *  * Panda_Lei
 *  * ��    �� : 2017/03/13
********************************************************/
void motor_pid_caculate(Motor_pid_info *motor_info)
{
  float delta_uk = 0;//��ǰʵ��Ӧ�����ռ�ձȣ���delta_duty��last_uk
  float out_duty = 0;
  uint8 i = 0;
  
  if(Motor.present_value[0]>500)
  {
      Motor.present_value[0] = 0;
  }
  for(i=9;i>0;i--)     //����ƫ�����
  {
    motor_info->error[i] = motor_info->error[i-1];
  }
  motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];


   motor_info->speed_racc = 0;   //���������ٶ�,���ٶ�΢��
   for(i=0;i<1;i++)
     motor_info->speed_racc += motor_info->present_value[i];
   for(i=1;i<2;i++)
     motor_info->speed_racc -= motor_info->present_value[i];
   //������ǰ��-΢����������ʽPID����
   delta_uk = (float)(motor_info->kvff*motor_info->kvff_param*(motor_info->set_value[0] - motor_info->set_value[1])
              +motor_info->kp*(motor_info->error[0] - motor_info->error[1])
              + motor_info->ki*motor_info->error[0]
              + motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]
              + motor_info->error[2])
              -motor_info->kaff*motor_info->speed_racc*0.10
              );
                                                        

  /*------------------------�������������ֱ��͵ȴ���-----------------------*/
  if(motor_info->last_uk > MOTOR_DUTY_MAX)  //��Χ-1000~1000
    if(delta_uk > 0)
      delta_uk = 0;//��ǰһʱ�̿�����(ռ�ձ�)�Ѿ��ﵽ���ʱ������������Ϊ�����ۼ�
  if(motor_info->last_uk < MOTOR_DUTY_MIN)
    if(delta_uk < 0)
      delta_uk = 0;//��ǰһʱ�̿�����(ռ�ձ�)�Ѿ��ﵽ���ʱ������������Ϊ�����ۼ�
  /*------------------------�������������ֱ��͵ȴ������-------------------*/
  
  
  out_duty = motor_info->last_uk + delta_uk;//��ǰ��Ҫ�����ʵ��ռ�ձ�
  
  /*-------------------------���� ʵ�����������ķ�ֵ--------------------*/
  if (out_duty > MOTOR_DUTY_MAX)
     out_duty = MOTOR_DUTY_MAX;
  else if (out_duty < MOTOR_DUTY_MIN)
     out_duty = MOTOR_DUTY_MIN;
  /*-------------------------����ʵ�����������ķ�ֵ����------------------*/
  
  motor_info->last_uk = out_duty;               //������һ�ε�ʵ�ʿ��������
  motor_info->output =(int16)(out_duty);     //��ǰռ�ձ����
  
  for(i = 9;i > 0;i--)  //�����ٶ��趨ֵ����
    motor_info->set_value[i] = motor_info->set_value[i-1];
   
  for(i= 9;i>0;i--)    //����ʵ���ٶȶ���
  {
    motor_info->present_value[i] = motor_info->present_value[i-1];
  } 
  
  motor_set(motor_info->output);

}

#endif




