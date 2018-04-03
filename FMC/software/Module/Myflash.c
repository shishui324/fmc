#include "Myflash.h"



/*
��������PID����
����ֵ
0-----����ɹ�
1-----����ʧ��

*/
uint8 Save_Left_motor(void)
{
    uint8 i=1;

    DisableInterrupts;//�����ж�
    FLASH_EraseSector(Left_Motor_Erase);		//��������

    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kp),4,4);             //д���ĸ��ֽڵ����һ������ ƫ��0�ֽ�
    if(i)	return i;

    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.ki),4,8);
    if(i)	return i;
    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kd),4,12);
    if(i)	return i;
    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kvff),4,16);
    if(i)	return i;
    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kaff),4,20);
    return i;

//	EnableInterrupts;//�����ж�

}
void Read_Left_motor(void)
{
    DisableInterrupts;//�����ж�

    Motor_control.Motor_Left_pid.kp		=		flash_read(Left_Motor_Erase,4,float);
    Motor_control.Motor_Left_pid.ki		=		flash_read(Left_Motor_Erase,8,float);
    Motor_control.Motor_Left_pid.kd		=		flash_read(Left_Motor_Erase,12,float);
    Motor_control.Motor_Left_pid.kvff	=		flash_read(Left_Motor_Erase,16,float);
    Motor_control.Motor_Left_pid.kaff	=		flash_read(Left_Motor_Erase,20,float);

    EnableInterrupts;//�����ж�

}

/*
�����ҵ��PID����
����ֵ
0-----����ɹ�
1-----����ʧ��

*/
uint8 Save_Right_motor(void)
{
    uint8 i=1;

    DisableInterrupts;//�����ж�
    FLASH_EraseSector(Right_Motor_Erase);		//��������

    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kp),4,4);             //д���ĸ��ֽڵ����һ������ ƫ��0�ֽ�
    if(i)	return i;

    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.ki),4,8);
    if(i)	return i;
    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kd),4,12);
    if(i)	return i;
    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kvff),4,16);
    if(i)	return i;
    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kaff),4,20);
    return i;

//	EnableInterrupts;//�����ж�

}
void Read_Right_motor(void)
{
    DisableInterrupts;//�����ж�

    Motor_control.Motor_Right_pid.kp		=		flash_read(Right_Motor_Erase,4,float);
    Motor_control.Motor_Right_pid.ki		=		flash_read(Right_Motor_Erase,8,float);
    Motor_control.Motor_Right_pid.kd		=		flash_read(Right_Motor_Erase,12,float);
    Motor_control.Motor_Right_pid.kvff	=		flash_read(Right_Motor_Erase,16,float);
    Motor_control.Motor_Right_pid.kaff	=		flash_read(Right_Motor_Erase,20,float);

    EnableInterrupts;//�����ж�

}

/*
���淽��PID����
����ֵ
0-----����ɹ�
1-----����ʧ��

*/
uint8 Save_Servo(void)
{
    uint8 i=1;

    DisableInterrupts;//�����ж�
    FLASH_EraseSector(Servo_Erase);		//��������

    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.kp),4,4);             //д���ĸ��ֽڵ����һ������ ƫ��0�ֽ�
    if(i)	return i;

    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.ki),4,8);
    if(i)	return i;
    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.kd),4,12);
    if(i)	return i;
    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.distance_err_max_val),4,16);
    if(i)	return i;
    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.distance_err_d_max_val),4,20);
    if(i)	return i;
    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.max_dis_err),4,24);
    if(i)	return i;
    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.max_dis_err_d),4,28);
    return i;

//	EnableInterrupts;//�����ж�

}
void Read_Servo(void)
{
    DisableInterrupts;//�����ж�

    Servo.kp											=			flash_read(Servo_Erase,4,float);
    Servo.ki											=			flash_read(Servo_Erase,8,float);
    Servo.kd											=			flash_read(Servo_Erase,12,float);
    Servo.distance_err_max_val		=			flash_read(Servo_Erase,16,float);
    Servo.distance_err_d_max_val	=			flash_read(Servo_Erase,20,float);
    Servo.max_dis_err							=			flash_read(Servo_Erase,24,float);
    Servo.max_dis_err_d						=			flash_read(Servo_Erase,28,float);

    EnableInterrupts;//�����ж�

}

/*
�����ٶȲ���
����ֵ
0-----����ɹ�
1-----����ʧ��

*/
uint8 Save_Speed(void)
{
    uint8 i=1;

    DisableInterrupts;//�����ж�
    FLASH_EraseSector(Speed_Erase);		//��������

    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.zhidao_speed_val),4,4);             //д���ĸ��ֽڵ����һ������ ƫ��0�ֽ�
    if(i)	return i;

    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.wandao_speed_val),4,8);
    if(i)	return i;
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.cross_speed_val),4,12);
    if(i)	return i;                                                         
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.shizi_speed_val),4,16);
    if(i)	return i;                                                         
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.ramp1_speed_val),4,20);
    if(i)	return i;                                                         
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.ramp2_speed_val),4,24);
    if(i)	return i;                                                         
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.speed_high_mode),4,28);
    if(i)	return i;
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.high_speed_dis),4,32);
    if(i)	return i;
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.stop_car_enable),4,36);
    if(i)	return i;
    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.test_time),4,40);
    return i;

//	EnableInterrupts;//�����ж�

}
void Read_Speed(void)
{
    DisableInterrupts;//�����ж�


		
    Speed.zhidao_speed_val			=			flash_read(Speed_Erase,4,uint16_t);
    Speed.wandao_speed_val			=			flash_read(Speed_Erase,8,uint16_t);
    Speed.cross_speed_val				=			flash_read(Speed_Erase,12,uint16_t);
    Speed.shizi_speed_val				=			flash_read(Speed_Erase,16,uint16_t);
    Speed.ramp1_speed_val				=			flash_read(Speed_Erase,20,uint16_t);
    Speed.ramp2_speed_val				=			flash_read(Speed_Erase,24,uint16_t);
    Speed.speed_high_mode				=			flash_read(Speed_Erase,28,uint16_t);
    Speed.high_speed_dis				=			flash_read(Speed_Erase,32,uint16_t);
    Speed.stop_car_enable				=			flash_read(Speed_Erase,36,uint16_t);
    Speed.test_time							=			flash_read(Speed_Erase,40,uint16_t);



    EnableInterrupts;//�����ж�

}


/*
��ȡ����ֵ

*/
void Read_all(void)
{

    DisableInterrupts;//�����ж�
    /*��������*/
    Motor_control.Motor_Left_pid.kp			=		flash_read(Left_Motor_Erase,4,float);
    Motor_control.Motor_Left_pid.ki			=		flash_read(Left_Motor_Erase,8,float);
    Motor_control.Motor_Left_pid.kd			=		flash_read(Left_Motor_Erase,12,float);
    Motor_control.Motor_Left_pid.kvff		=		flash_read(Left_Motor_Erase,16,float);
    Motor_control.Motor_Left_pid.kaff		=		flash_read(Left_Motor_Erase,20,float);

    /*     �ҵ������*/
    Motor_control.Motor_Right_pid.kp		=		flash_read(Right_Motor_Erase,4,float);
    Motor_control.Motor_Right_pid.ki		=		flash_read(Right_Motor_Erase,8,float);
    Motor_control.Motor_Right_pid.kd		=		flash_read(Right_Motor_Erase,12,float);
    Motor_control.Motor_Right_pid.kvff	=		flash_read(Right_Motor_Erase,16,float);
    Motor_control.Motor_Right_pid.kaff	=		flash_read(Right_Motor_Erase,20,float);


    /*				����PID*/
    Servo.kp														=		flash_read(Servo_Erase,4,float);
    Servo.ki														=		flash_read(Servo_Erase,8,float);
    Servo.kd														=		flash_read(Servo_Erase,12,float);
    Servo.distance_err_max_val					=		flash_read(Servo_Erase,16,float);
    Servo.distance_err_d_max_val				=		flash_read(Servo_Erase,20,float);
    Servo.max_dis_err										=		flash_read(Servo_Erase,24,float);
    Servo.max_dis_err_d									=		flash_read(Servo_Erase,28,float);

    /*			�ٶ�*/
    Speed.zhidao_speed_val			  			=		flash_read(Servo_Erase,4,float);
    Speed.wandao_speed_val			  			=		flash_read(Servo_Erase,8,float);
    Speed.cross_speed_val				  			=		flash_read(Servo_Erase,12,float);
    Speed.shizi_speed_val				  			=		flash_read(Servo_Erase,16,float);
    Speed.ramp1_speed_val				  			=		flash_read(Servo_Erase,20,float);
    Speed.ramp2_speed_val				  			=		flash_read(Servo_Erase,24,float);
    Speed.speed_high_mode				  			=		flash_read(Servo_Erase,28,float);
    Speed.high_speed_dis				  			=		flash_read(Servo_Erase,32,float);
    Speed.stop_car_enable				  			=		flash_read(Servo_Erase,36,float);
    Speed.test_time							  			=		flash_read(Servo_Erase,40,float);



    EnableInterrupts;//�����ж�


}






