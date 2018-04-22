
#include "KEA128_gpio.h"
#include "KEA128_ftm.h"

#include  "motor.h"
#include "control.h"

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


/***********************************************************************
**  ��������:
**  ��������:
**  �� �� ֵ:
**  �� �� ֵ:
**  ����˵��:
**  ��    ��:
***************************************************************************/

void motor_pid_caculate(Motor_pid_info *motor_info)
{
    motor_info->delta_uk[0] = 0;
    motor_info->out_duty = 0;
    uint8 i = 0;
    for(i=9; i>0; i--)			//�����ٶ�ƫ�����
    {
        motor_info->error[i] = motor_info->error[i-1];
    }
		
//****************CAUTION*******************/
    motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];
    motor_info->speed_racc = motor_info->present_value[0]-motor_info->present_value[1];//���������ٶ�,���ٶ�΢��
		for(i=3;i>0;i--)
		{
			motor_info->delta_uk[i]=motor_info->delta_uk[i-1];
			
		}	

///////������ǰ��-΢����������ʽPID����
    motor_info->delta_uk[0]= (float)(
                               motor_info->kvff*(motor_info->set_value[0] - motor_info->set_value[1])
                               +	motor_info->kp*(motor_info->error[0] - motor_info->error[1])
                               + 	motor_info->ki*motor_info->error[0]
                               + 	motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]+ motor_info->error[2])
                               -	motor_info->kaff*motor_info->speed_racc
                           );
		


    /*------------------------�������������ֱ��͵ȴ���-----------------------*/
    if(motor_info->last_uk > MOTOR_DUTY_MAX)
        if(motor_info->delta_uk[0] > 0)
            motor_info->delta_uk [0]= 0;//��ǰһʱ�̿�����(ռ�ձ�)�Ѿ��ﵽ���ʱ������������Ϊ�����ۼ�
    if(motor_info->last_uk < MOTOR_DUTY_MIN)
        if(motor_info->delta_uk[0] < 0)
            motor_info->delta_uk[0] = 0;//��ǰһʱ�̿�����(ռ�ձ�)�Ѿ��ﵽ���ʱ������������Ϊ�����ۼ�
    /*------------------------�������������ֱ��͵ȴ������-------------------*/
		motor_info->delta_uk[0]=0.7f*motor_info->delta_uk[0]+0.2f*motor_info->delta_uk[1]+0.1f*motor_info->delta_uk[2];
		
    motor_info->out_duty = motor_info->last_uk + motor_info->delta_uk[0];//�̡�?��D����a��?3?��?����?��????����

    /*-------------------------���� ʵ�����������ķ�ֵ---------------------*/
    if (motor_info->out_duty > MOTOR_DUTY_MAX)
        motor_info->out_duty = MOTOR_DUTY_MAX;
    else if (motor_info->out_duty < MOTOR_DUTY_MIN)
        motor_info->out_duty = MOTOR_DUTY_MIN;
    /*-------------------------����ʵ�����������ķ�ֵ����------------------*/

    motor_info->last_uk = motor_info->out_duty;               //������һ�ε�ʵ�ʿ��������
    motor_info->output =(int16)(motor_info->out_duty);     //��ǰռ�ձ����


    for(i = 9; i> 0; i--)  				//�����ٶ��趨ֵ����
        motor_info->set_value[i] = motor_info->set_value[i-1];

    for(i= 9; i>0; i--)    				//����ʵ���ٶȶ���
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
 *  * �������� ��motor_set
 *  * �������� : int16 duty
 *  * ����ֵ   : ��
 *  * �������� ���������
 *  * Panda_Lei
 *  * ��    �� : 2017/03/13
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
            ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //�ұߵ�����

        }
        else if(R_out_value < 0)
        {
            gpio_set(H3,0);
            ftm_pwm_duty(ftm2,ftm_ch3,(-R_out_value)); //�ұߵ�����

        }
				
        if(L_out_value>=0)
        {

            gpio_set(C0,1);
            ftm_pwm_duty(ftm2,ftm_ch5,L_out_value); //��ߵ�����

        }
        else if(L_out_value < 0)
        {
            gpio_set(C0,0);
            ftm_pwm_duty(ftm2,ftm_ch5,(-L_out_value));//��ߵ�����
        }
    }
}







