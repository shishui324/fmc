#include "headfile.h"
#include  "motor.h"


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
/*****************************************************************

//PWM_L_OUT      PTC2        FTM2_CH2
//PWM_R_OUT      PTC3        FTM2_CH3

//DR_R      PTH3
//DR_L      PTC0

******************************************************************************///////



void motor_init(void )			//�����ʼ��
{ 
		gpio_init(H4,GPO,1);    //��ʼ��ENʹ�ܶ�
	
		gpio_init(H3,GPO,0);		//DR_R	
	
		gpio_init(C0,GPO,0);		//DR_L

//	
		ftm_pwm_init(ftm2,ftm_ch5,15*1000,0);         //������ʼ�� ͨ��2 15Khz		G7
	
	
    ftm_pwm_init(ftm2,ftm_ch3,15*1000,0);         //�ҵ����ʼ�� ͨ��3 15Khz  C3
	  
	
	
	
}

/**************** FUCK_MY_CAR  ****************************
 *  * �������� ��motor_set
 *  * �������� : int16 duty
 *  * ����ֵ   : ��
 *  * �������� ���������
 *  * Panda_Lei
 *  * ��    �� : 2017/03/13
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
			ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //�ұߵ�����
		
		}
		else if(R_out_value == 0)
		{
			gpio_set(H3,1);
			ftm_pwm_duty(ftm2,ftm_ch3,0); //�ұߵ�����
			
		}
		else
		{
			if(R_out_value>-100)
					R_out_value = -100;
//			if(R_out_value<-800)
//					R_out_value = -800;
			gpio_set(H3,0);
			ftm_pwm_duty(ftm2,ftm_ch3,-R_out_value); //�ұߵ�����
//			ftm_pwm_duty(ftm2,ftm_ch3,0); //�ұߵ�����
		}

		if(L_out_value>0)
		{		
			
			if(L_out_value<100)
					L_out_value = 100;
//			if(L_out_value>800)
//					L_out_value = 800;
			
			gpio_set(C0,1);
			ftm_pwm_duty(ftm2,ftm_ch5,L_out_value); //��ߵ�����
			
		}
		else if(L_out_value == 0)
		{
			gpio_set(C0,1);
			ftm_pwm_duty(ftm2,ftm_ch5,L_out_value);//��ߵ�����
		}
		else
		{
			if(L_out_value>-100)
					L_out_value = -100;
//			if(L_out_value<-800)
//					L_out_value = -800;
			gpio_set(C0,0);
			ftm_pwm_duty(ftm2,ftm_ch5,-L_out_value);//��ߵ�����
		}
//		Motor.set_value[0] = 6;
//	}
	
	
//			gpio_set(C0,(uint16 )flag);
//			ftm_pwm_duty(ftm2,ftm_ch5,L_out_value);//��ߵ�����
//			gpio_set(H3,(uint16 )flag);                                         
//			ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //�ұߵ�����
		
		
		
		
}







