#include "headfile.h"
#include  "motor.h"

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
uint16  MOTOR_DUTY_LIMIT = 700;     //����ռ�ձȣ��Է���������������
extern uint8 MOTOR_EN;

//int16_t MOTOR_DUTY_MAX;
//int16_t	MOTOR_DUTY_MIN;

Motor_control_info Motor_control;

uint32 speed_correction_count = 0;

void motor_init(void)						//�����ʼ��
{ 
	gpio_init(G5,GPO,0);    				//��ʼ��ENʹ�ܶ�
	gpio_init(F0,GPO,0);					//DR_R
	gpio_init(G4,GPO,0);					//DR_L
	ftm_pwm_init(ftm2,ftm_ch1,15*1000,300);   //�ҵ����ʼ�� ͨ��3 15Khz  	F1
	ftm_pwm_init(ftm2,ftm_ch4,15*1000,300);   //������ʼ�� ͨ��2 15Khz		G6
}

void pulse_acc_init(void)	//�����ۼӼ�����ʼ��
{
    ftm_count_init(ftm0);   //�������
    gpio_init(H5,GPI,0);    //DIR_Lʼ��
    port_pull(H5);

    ftm_count_init(ftm1);   //�ұ�����
    gpio_init(C5,GPI,0);    //DIR_R��ʼ��
    port_pull(C5);
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
	if(MOTOR_EN)
		gpio_set(G5,1);
	else 
		gpio_set(G5,0);
		
	if(Motor_control.Motor_Right_pid.output>=0)
	{
		gpio_set(F0,0);
//		if(Motor_control.Motor_Right_pid.output<100)
//			Motor_control.Motor_Right_pid.output = 100;
		ftm_pwm_duty(ftm2,ftm_ch1,Motor_control.Motor_Right_pid.output); //�ұߵ�����
	}
	else
	{
		gpio_set(F0,1);
//		if(Motor_control.Motor_Right_pid.output>-100)					  //����
//			Motor_control.Motor_Right_pid.output = -100;
		ftm_pwm_duty(ftm2,ftm_ch1,-Motor_control.Motor_Right_pid.output); //�ұߵ�����
		
	}
	if(Motor_control.Motor_Left_pid.output>=0)
	{		
		gpio_set(G4,1);
//		if(Motor_control.Motor_Left_pid.output<100)
//			Motor_control.Motor_Left_pid.output = 100;
		ftm_pwm_duty(ftm2,ftm_ch4,Motor_control.Motor_Left_pid.output); //��ߵ�����
	}
	else
	{
		gpio_set(G4,0);
//		if(Motor_control.Motor_Left_pid.output>-100)
//			Motor_control.Motor_Left_pid.output = -100;
		ftm_pwm_duty(ftm2,ftm_ch4,-Motor_control.Motor_Left_pid.output);//��ߵ�����
	}
}

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

	for(i=9;i>0;i--)     //����ƫ�����
		motor_info->error[i] = motor_info->error[i-1];
	motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];

	motor_info->speed_racc = 0;   //���������ٶ�,���ٶ�΢��
	for(i=0;i<1;i++)
		motor_info->speed_racc += motor_info->present_value[i];
	for(i=1;i<2;i++)
		motor_info->speed_racc -= motor_info->present_value[i];
	//������ǰ��-΢����������ʽPID����
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
														

	/*------------------------�������������ֱ��͵ȴ���-----------------------*/
	if(motor_info->last_uk > MOTOR_DUTY_LIMIT)  //��Χ-1000~1000
		if(delta_uk > 0)
			delta_uk = 0;//��ǰһʱ�̿�����(ռ�ձ�)�Ѿ��ﵽ���ʱ������������Ϊ�����ۼ�
	if(motor_info->last_uk < -800)//MOTOR_DUTY_LIMIT
		if(delta_uk < 0)
			delta_uk = 0;//��ǰһʱ�̿�����(ռ�ձ�)�Ѿ��ﵽ���ʱ������������Ϊ�����ۼ�
	/*------------------------�������������ֱ��͵ȴ������-------------------*/


	out_duty = motor_info->last_uk + delta_uk;//��ǰ��Ҫ�����ʵ��ռ�ձ�
		
		
		
		
		out_duty=0.9f*out_duty+0.1f*motor_info->last_uk;
		
		
		

	/*-------------------------���� ʵ�����������ķ�ֵ--------------------*/
	if (out_duty > MOTOR_DUTY_LIMIT)
		out_duty = MOTOR_DUTY_LIMIT;
	else if (out_duty < -800)
		out_duty = -800;
	/*-------------------------����ʵ�����������ķ�ֵ����------------------*/

	motor_info->last_uk = out_duty;               //������һ�ε�ʵ�ʿ��������
	motor_info->output =(int16)(out_duty);     //��ǰռ�ձ����

	for(i = 9;i > 0;i--)  //�����ٶ��趨ֵ����
		motor_info->set_value[i] = motor_info->set_value[i-1];

	for(i= 9;i>0;i--)    //����ʵ���ٶȶ���
		motor_info->present_value[i] = motor_info->present_value[i-1];
}

void speed_correction(void)		//�����������1m,ʵ����1m������
{
	speed_correction_count += Motor_control.Motor_Left_pid.present_value[0] + Motor_control.Motor_Right_pid.present_value[0];
}



