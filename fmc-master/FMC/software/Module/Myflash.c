#include "Myflash.h"




/***************  Small_Cap  ********************************
 *  * �ļ�˵����flash��д����
 *  * ��    �ߣ�
 *  * ��    �ڣ�2017/03/26
 *  * ��    ����V1.0
 *  * �޸���  ��panda_lei
 *  * �޸����ڣ�2018/4/2
***********************************************************/

//uint32 read_buffer1[100]={0};             //����Flash������1
//uint32 write_buffer1[100]={0};            //����Flashд����1


///////////////�����׵�ַ�����ݳ�������ֵΪ0 

#if 0
/**************** Small_Cap  ****************************
 *  * �������� ��flash_write_data
 *  * �������� : ��
 *  * ����ֵ   : �ɹ�����FLASH_OK��0��ʧ�ܷ���FLASH_FAILED��1
 *  * �������� ��д���ݵ�flash
 *  * ��    �� ��
 *  * ��    �� : 2017/03/26
********************************************************/

uint8 flash_write_data(void)
{
    uint8 i = 0;  
    uint8 result = 0 ;
    DisableInterrupts; //�����ж�,flash��д���ܱ��жϴ��
    FLASH_Init(); //��ʼ��flash
 
    result = FLASH_EraseSector(FLASH_SECTOR);//�������� 
    if(result) //����ʧ��
      return FLASH_FAILED;
    systick_delay_ms(30);
    
    write_buffer1[0] = 0;
    
    //��дFLASH����ֵ��
    for(i = 1; i <= 8; i++)		//������ֵ
		write_buffer1[i] = (uint32)Adc.ad_max_val[i];
	
		write_buffer1[9] = (uint32)(Servo.kp * 1000);
		write_buffer1[10] = (uint32)(Servo.kd * 1000);
	
	
	
	
	
	
	
	
	
    FLASH_WriteSector(FLASH_SECTOR,(const uint8 *)write_buffer1,FLASH_DATA_SIZE,0); //д�����ݵ�ָ������
    systick_delay_ms(10);
 
    EnableInterrupts;

    return FLASH_OK;
}
/**************** Small_Cap  ****************************
 *  * �������� ��flash_read_data
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� ����flash�������
 *  * ��    �� ��
 *  * ��    �� : 2017/03/26
********************************************************/
void flash_read_data(void)
{
	uint8 i = 0;
	DisableInterrupts;
	
	for(;i<FLASH_DATA_SIZE/4; i++)
		read_buffer1[i] =  flash_read(FLASH_SECTOR,i<<2,uint32);

	//��FLASH
	for(i = 1;i <=8;i ++)   //������ֵ
	{
		Adc.ad_max_val[i]= (uint16)(read_buffer1[i]);
	}
	
	Servo.kp = (float)(read_buffer1[9]/1000.0);
	Servo.kd = (float)(read_buffer1[10]/1000.0);
	
	
	

	EnableInterrupts;
}
#endif 
/*
��������PID����
����ֵ
0-----����ɹ�
1-----����ʧ��

*/
uint8 Save_Left_motor(void)
{
  
    uint8 result = 0 ;
		uint32_t write_buffer[(FLASH_DATA_SIZE)>>2]={0};            //����Flashд����1
    DisableInterrupts; //�����ж�,flash��д���ܱ��жϴ��
    FLASH_Init(); //��ʼ��flash
 
    result = FLASH_EraseSector(Left_Motor_Erase);//�������� 
    if(result) //����ʧ��
		{
			EnableInterrupts;//�����ж�
      return FLASH_FAILED;
				
		}
		systick_delay_ms(30);
		
		write_buffer[0]=(uint32_t)(Motor_control.Motor_Left_pid.kp*1000.0f);
		write_buffer[1]=(uint32_t)(Motor_control.Motor_Left_pid.ki*1000.0f);
		write_buffer[2]=(uint32_t)(Motor_control.Motor_Left_pid.kd*1000.0f);
		write_buffer[3]=(uint32_t)(Motor_control.Motor_Left_pid.kvff*1000.0f);
		write_buffer[4]=(uint32_t)(Motor_control.Motor_Left_pid.kaff*1000.0f);
		
		result =FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)write_buffer,FLASH_DATA_SIZE,4);
		
		EnableInterrupts;//�����ж�
		return FLASH_OK;

}
void Read_Left_motor(void)
{
//		uint8 i = 0;
		uint32_t read_buffer[(FLASH_DATA_SIZE)>>2]={0};  
    DisableInterrupts;//�����ж�
		for(uint8 i=1;i<=(FLASH_DATA_SIZE)>>2; i++)
		read_buffer[i-1] =  flash_read(Left_Motor_Erase,i<<2,uint32);


    Motor_control.Motor_Left_pid.kp		=	(float)(	read_buffer[0]/1000.0f);
    Motor_control.Motor_Left_pid.ki		=	(float)(	read_buffer[1]/1000.0f);
    Motor_control.Motor_Left_pid.kd		=	(float)(	read_buffer[2]/1000.0f);
    Motor_control.Motor_Left_pid.kvff	=	(float)(	read_buffer[3]/1000.0f);
    Motor_control.Motor_Left_pid.kaff	=	(float)(	read_buffer[4]/1000.0f);

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
    uint8 result = 0 ;
		uint32_t write_buffer[(FLASH_DATA_SIZE)>>2]={0};            //����Flashд����1
    DisableInterrupts; //�����ж�,flash��д���ܱ��жϴ��
    FLASH_Init(); //��ʼ��flash
 
    result = FLASH_EraseSector(Right_Motor_Erase);//�������� 
    if(result) //����ʧ��
		{
			EnableInterrupts;//�����ж�
      return FLASH_FAILED;
				
		}
		systick_delay_ms(30);
		
		write_buffer[0]=(uint32_t)(Motor_control.Motor_Right_pid.kp*1000.0f);
		write_buffer[1]=(uint32_t)(Motor_control.Motor_Right_pid.ki*1000.0f);
		write_buffer[2]=(uint32_t)(Motor_control.Motor_Right_pid.kd*1000.0f);
		write_buffer[3]=(uint32_t)(Motor_control.Motor_Right_pid.kvff*1000.0f);
		write_buffer[4]=(uint32_t)(Motor_control.Motor_Right_pid.kaff*1000.0f);
		
		result =FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)write_buffer,FLASH_DATA_SIZE,4);
		
		EnableInterrupts;//�����ж�
		return FLASH_OK;

}
/***********************************************************************
// ��������:
// ��������
// ��������:
// ����ֵ:
// ��������
// ����ֵ:
// ����˵��:
//���ڣ�
***************************************************************************/

void Read_Right_motor(void)
{
    DisableInterrupts;//�����ж�

    uint32_t read_buffer[(FLASH_DATA_SIZE)>>2]={0};  
    DisableInterrupts;//�����ж�
		for(uint8 i=1;i<=(FLASH_DATA_SIZE)>>2; i++)
		read_buffer[i-1] =  flash_read(Right_Motor_Erase,i<<2,uint32);


    Motor_control.Motor_Right_pid.kp		=	(float)(	read_buffer[0]/1000.0f);
    Motor_control.Motor_Right_pid.ki		=	(float)(	read_buffer[1]/1000.0f);
    Motor_control.Motor_Right_pid.kd		=	(float)(	read_buffer[2]/1000.0f);
    Motor_control.Motor_Right_pid.kvff	=	(float)(	read_buffer[3]/1000.0f);
    Motor_control.Motor_Right_pid.kaff	=	(float)(	read_buffer[4]/1000.0f);

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
    uint8 result = 0 ;
		uint32_t write_buffer[(FLASH_DATA_SIZE)>>2]={0};            //����Flashд����1
    DisableInterrupts; //�����ж�,flash��д���ܱ��жϴ��
    FLASH_Init(); //��ʼ��flash
 
    result = FLASH_EraseSector(Servo_Erase);//�������� 
    if(result) //����ʧ��
		{
			EnableInterrupts;//�����ж�
      return FLASH_FAILED;
				
		}
		systick_delay_ms(30);
		
		write_buffer[0]=(uint32_t)(Servo.kp*1000.0f);
		write_buffer[1]=(uint32_t)(Servo.ki*1000.0f);
		write_buffer[2]=(uint32_t)(Servo.kd*1000.0f);
		write_buffer[3]=(uint32_t)(Servo.distance_err_max_val*1000.0f);
		write_buffer[4]=(uint32_t)(Servo.distance_err_d_max_val*1000.0f);
		write_buffer[5]=(uint32_t)(Servo.max_dis_err*1000.0f);
		write_buffer[6]=(uint32_t)(Servo.max_dis_err_d*1000.0f);
		
		result =FLASH_WriteSector(Servo_Erase,(const uint8 *)write_buffer,FLASH_DATA_SIZE,4);
		
		EnableInterrupts;//�����ж�
		return FLASH_OK;

}
void Read_Servo(void)
{
    DisableInterrupts;//�����ж�
	
    uint32_t read_buffer[(FLASH_DATA_SIZE)>>2]={0};  
    DisableInterrupts;//�����ж�
		for(uint8 i=1;i<=(FLASH_DATA_SIZE)>>2; i++)
		read_buffer[i-1] =  flash_read(Servo_Erase,i<<2,uint32);
		
    Servo.kp											=			(float)(	read_buffer[0]/1000.0f);
    Servo.ki											=			(float)(	read_buffer[1]/1000.0f);
    Servo.kd											=			(float)(	read_buffer[2]/1000.0f);
    Servo.distance_err_max_val		=			(float)(	read_buffer[3]/1000.0f);
    Servo.distance_err_d_max_val	=			(float)(	read_buffer[4]/1000.0f);
    Servo.max_dis_err							=			(float)(	read_buffer[5]/1000.0f);
    Servo.max_dis_err_d						=			(float)(	read_buffer[6]/1000.0f);

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
    uint8 result = 0 ;
		uint32_t write_buffer[(FLASH_DATA_SIZE)>>2]={0};            //����Flashд����1
    DisableInterrupts; //�����ж�,flash��д���ܱ��жϴ��
    FLASH_Init(); //��ʼ��flash
 
    result = FLASH_EraseSector(Speed_Erase);//�������� 
    if(result) //����ʧ��
		{
			EnableInterrupts;//�����ж�
      return FLASH_FAILED;
				
		}
		systick_delay_ms(30);
		
		write_buffer[0]=(uint32_t)(Speed.zhidao_speed_val);
		write_buffer[1]=(uint32_t)(Speed.wandao_speed_val);
		write_buffer[2]=(uint32_t)(Speed.cross_speed_val);
		write_buffer[3]=(uint32_t)(Speed.shizi_speed_val);
		write_buffer[4]=(uint32_t)(Speed.ramp1_speed_val);
		write_buffer[5]=(uint32_t)(Speed.ramp2_speed_val);
		write_buffer[6]=(uint32_t)(Speed.speed_high_mode);
		write_buffer[7]=(uint32_t)(Speed.high_speed_dis	);
		write_buffer[8]=(uint32_t)(Speed.stop_car_enable);
		write_buffer[9]=(uint32_t)(Speed.test_time);
		                                 				
		result =FLASH_WriteSector(Speed_Erase,(const uint8 *)write_buffer,FLASH_DATA_SIZE,4);
		
		EnableInterrupts;//�����ж�
		return FLASH_OK;

}
void Read_Speed(void)
{
    DisableInterrupts;//�����ж�

	uint32_t read_buffer[(FLASH_DATA_SIZE)>>2]={0};  
    DisableInterrupts;//�����ж�
		for(uint8 i=1;i<=(FLASH_DATA_SIZE)>>2; i++)
			read_buffer[i-1] =  flash_read(Speed_Erase,i<<2,uint32);
		
    Speed.zhidao_speed_val			=	(uint16_t)	(read_buffer[0] );
    Speed.wandao_speed_val			=	(uint16_t)	(read_buffer[1] );
    Speed.cross_speed_val				=	(uint16_t)	(read_buffer[2] );
    Speed.shizi_speed_val				=	(uint16_t)	(read_buffer[3] );
    Speed.ramp1_speed_val				=	(uint16_t)	(read_buffer[4] );
    Speed.ramp2_speed_val				=	(uint16_t)	(read_buffer[5] );
    Speed.speed_high_mode				=	(uint16_t)	(read_buffer[6] );
    Speed.high_speed_dis				=	(uint16_t)	(read_buffer[7] );
    Speed.stop_car_enable				=	(uint16_t)	(read_buffer[8] );
    Speed.test_time							=	(uint16_t)	(read_buffer[9] );
                                      

    EnableInterrupts;//�����ж�

}


/*
��ȡ����ֵ

*/
//void Read_all(void)
//{

//    DisableInterrupts;//�����ж�
//    /*��������*/
//    Motor_control.Motor_Left_pid.kp			=		flash_read(Left_Motor_Erase,4,float);
//    Motor_control.Motor_Left_pid.ki			=		flash_read(Left_Motor_Erase,8,float);
//    Motor_control.Motor_Left_pid.kd			=		flash_read(Left_Motor_Erase,12,float);
//    Motor_control.Motor_Left_pid.kvff		=		flash_read(Left_Motor_Erase,16,float);
//    Motor_control.Motor_Left_pid.kaff		=		flash_read(Left_Motor_Erase,20,float);

//    /*     �ҵ������*/
//    Motor_control.Motor_Right_pid.kp		=		flash_read(Right_Motor_Erase,4,float);
//    Motor_control.Motor_Right_pid.ki		=		flash_read(Right_Motor_Erase,8,float);
//    Motor_control.Motor_Right_pid.kd		=		flash_read(Right_Motor_Erase,12,float);
//    Motor_control.Motor_Right_pid.kvff	=		flash_read(Right_Motor_Erase,16,float);
//    Motor_control.Motor_Right_pid.kaff	=		flash_read(Right_Motor_Erase,20,float);


//    /*				����PID*/
//    Servo.kp														=		flash_read(Servo_Erase,4,float);
//    Servo.ki														=		flash_read(Servo_Erase,8,float);
//    Servo.kd														=		flash_read(Servo_Erase,12,float);
//    Servo.distance_err_max_val					=		flash_read(Servo_Erase,16,float);
//    Servo.distance_err_d_max_val				=		flash_read(Servo_Erase,20,float);
//    Servo.max_dis_err										=		flash_read(Servo_Erase,24,float);
//    Servo.max_dis_err_d									=		flash_read(Servo_Erase,28,float);

//    /*			�ٶ�*/
//    Speed.zhidao_speed_val			  			=		flash_read(Servo_Erase,4,float);
//    Speed.wandao_speed_val			  			=		flash_read(Servo_Erase,8,float);
//    Speed.cross_speed_val				  			=		flash_read(Servo_Erase,12,float);
//    Speed.shizi_speed_val				  			=		flash_read(Servo_Erase,16,float);
//    Speed.ramp1_speed_val				  			=		flash_read(Servo_Erase,20,float);
//    Speed.ramp2_speed_val				  			=		flash_read(Servo_Erase,24,float);
//    Speed.speed_high_mode				  			=		flash_read(Servo_Erase,28,float);
//    Speed.high_speed_dis				  			=		flash_read(Servo_Erase,32,float);
//    Speed.stop_car_enable				  			=		flash_read(Servo_Erase,36,float);
//    Speed.test_time							  			=		flash_read(Servo_Erase,40,float);



//    EnableInterrupts;//�����ж�


//}






