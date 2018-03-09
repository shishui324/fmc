#include "headfile.h"
#include  "motor.h"


/**************** Small_Cap  ****************************
 *  * �������� ��motor_pid_caculate
 *  * �������� : Motor_pid_info *motor_info
 *  * ����ֵ   : ��
 *  * �������� �����PID����
 *  * Panda_Lei
 *  * ��    �� : 2017/03/13
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
			ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //�ұߵ�����
			ftm_pwm_duty(ftm2,ftm_ch2,0); //�ұߵ�����
		}
		else if(R_out_value == 0)
		{
			ftm_pwm_duty(ftm2,ftm_ch3,0); //�ұߵ�����
			ftm_pwm_duty(ftm2,ftm_ch2,0); //�ұߵ�����
		}
		else
		{
			if(R_out_value>-100)
					R_out_value = -100;
//			if(R_out_value<-800)
//					R_out_value = -800;
			ftm_pwm_duty(ftm2,ftm_ch2,-R_out_value); //�ұߵ�����
			ftm_pwm_duty(ftm2,ftm_ch3,0); //�ұߵ�����
		}
		
		if(L_out_value>0)
		{
			if(L_out_value<100)
					L_out_value = 100;
//			if(L_out_value>800)
//					L_out_value = 800;
			ftm_pwm_duty(ftm2,ftm_ch0,L_out_value); //��ߵ�����
			ftm_pwm_duty(ftm2,ftm_ch1,0); //��ߵ�����
		}
		else if(L_out_value == 0)
		{
			ftm_pwm_duty(ftm2,ftm_ch0,0); //��ߵ�����
			ftm_pwm_duty(ftm2,ftm_ch1,0); //��ߵ�����
		}
		else
		{
			if(L_out_value>-100)
					L_out_value = -100;
//			if(L_out_value<-800)
//					L_out_value = -800;
			ftm_pwm_duty(ftm2,ftm_ch1,-L_out_value); //��ߵ�����
			ftm_pwm_duty(ftm2,ftm_ch0,0); //��ߵ�����
		}
//		Motor.set_value[0] = 6;
	}
}




#if 0
/***************  FUCK_MY_CAR  *****************************
 *  * �ļ�˵���������ʼ����PID����
		Panda_Lei
 *  * ��    �ڣ�2017/11/14
 *  * ��    ����V1.0
 ***********************************************************/


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
****************************z****************************/
//void motor_init(void)
//{
//  uint8 i = 0;
//  
//	ftm_pwm_init(ftm0,ftm_ch0,15*1000,0);		//ftm0ģ�飬0ͨ����50HZ���ٷ�֮50��ռ�ձ�
////  ftm_pwm_init(FTM2, FTM_CH0,5000, 0); //30kHz,duty = 0 ~10000          2017.4.9  5kHZ
////  gpio_init(PTA11,GPO,0);
////  gpio_init(PTA5,GPO,0);
////  port_init_NoALT(PTA11,PULLDOWN);       //���ùܽ�����
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




#endif





