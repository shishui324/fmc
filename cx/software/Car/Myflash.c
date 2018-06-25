#include "myflash.h"
#include "struct.h"
#include "headfile.h"

extern uint8 circle_turn_val[4];
extern int8_t TURN_Para;   //13   5
extern uint16_t TURN_LOSE_VAL;	//150
extern char circle_turn_direction[4];
extern uint16  MOTOR_DUTY_LIMIT;
extern uint16_t Car_speed;
extern uint16_t Ramp_speed;

/***************  Small_Cap  ********************************
 *  * 文件说明：flash读写数据
 *  * 作    者：
 *  * 日    期：2017/03/26
 *  * 版    本：V1.0
 *  * 修改者  ：panda_lei
 *  * 修改日期：2018/4/2
***********************************************************/

uint32 read_buffer1[100]={0};             //定义Flash读缓冲1
uint32 write_buffer1[100]={0};            //定义Flash写缓冲1


/**************** Small_Cap  ****************************
 *  * 函数名称 ：flash_write_data
 *  * 函数参数 : 无
 *  * 返回值   : 成功返回FLASH_OK即0，失败返回FLASH_FAILED即1
 *  * 函数功能 ：写数据到flash
 *  * 作    者 ：
 *  * 日    期 : 2017/03/26
********************************************************/

uint8 flash_write_data(void)
{
    uint8 i = 0;  
    uint8 result = 0 ;
    DisableInterrupts; //关总中断,flash读写不能被中断打断
    FLASH_Init(); //初始化flash
 
    result = FLASH_EraseSector(FLASH_SECTOR);//擦除扇区 
    if(result) //擦除失败
		return FLASH_FAILED;
    systick_delay_ms(30);
    
    write_buffer1[0] = 0;
    
    //（写FLASH）赋值区
    for(i = 1; i <= 8; i++)		//电感最大值
		write_buffer1[i] = 4095;//(uint32)Adc.ad_max_val[i];
	write_buffer1[3] = (uint32)Adc.ad_max_val[3];
	
	write_buffer1[9] = (uint32)(Servo.kp * 1000);
	write_buffer1[10] = (uint32)(Servo.kd * 1000);
    write_buffer1[11] = (uint32)(Servo.distance_err_max_val * 1000); 
    write_buffer1[12] = (uint32)(Servo.distance_err_d_max_val * 1000);
 
    write_buffer1[13] = (uint32)(Motor_control.Motor_Left_pid.kp * 1000);
    write_buffer1[14] = (uint32)(Motor_control.Motor_Left_pid.ki * 1000);
    write_buffer1[15] = (uint32)(Motor_control.Motor_Left_pid.kd * 1000);
    write_buffer1[16] = (uint32)(Motor_control.Motor_Left_pid.kvff * 1000);
    write_buffer1[17] = (uint32)(Motor_control.Motor_Left_pid.kaff * 1000);
	
	write_buffer1[18] = (uint32)(Motor_control.Motor_Right_pid.kp * 1000);
    write_buffer1[19] = (uint32)(Motor_control.Motor_Right_pid.ki * 1000);
    write_buffer1[20] = (uint32)(Motor_control.Motor_Right_pid.kd * 1000);
    write_buffer1[21] = (uint32)(Motor_control.Motor_Right_pid.kvff * 1000);
    write_buffer1[22] = (uint32)(Motor_control.Motor_Right_pid.kaff * 1000);
	
	write_buffer1[23] = (uint32)TURN_Para+10;
	write_buffer1[24] = (uint32)Car_speed;
	write_buffer1[25] = (uint32)TURN_LOSE_VAL;
	write_buffer1[26] = (uint32)MOTOR_DUTY_LIMIT;
	write_buffer1[27] = (uint32)circle_turn_direction[0];
	write_buffer1[28] = (uint32)circle_turn_direction[1];
	write_buffer1[29] = (uint32)circle_turn_direction[2];
	write_buffer1[30] = (uint32)circle_turn_direction[3];
	write_buffer1[31] = (uint32)circle_turn_val[0];
	write_buffer1[32] = (uint32)circle_turn_val[1];
	write_buffer1[33] = (uint32)circle_turn_val[2];
	write_buffer1[34] = (uint32)circle_turn_val[3];
	write_buffer1[35] = (uint32)Ramp_speed;
	
    FLASH_WriteSector(FLASH_SECTOR,(const uint8 *)write_buffer1,FLASH_DATA_SIZE,0); //写入数据到指定扇区
    systick_delay_ms(10);
 
    EnableInterrupts;

    return FLASH_OK;
}
/**************** Small_Cap  ****************************
 *  * 函数名称 ：flash_read_data
 *  * 函数参数 : 无
 *  * 返回值   : 无
 *  * 函数功能 ：读flash里的数据
 *  * 作    者 ：
 *  * 日    期 : 2017/03/26
********************************************************/
void flash_read_data(void)
{
	uint8 i = 0;
	DisableInterrupts;
	
	for(;i<FLASH_DATA_SIZE>>2; i++)
		read_buffer1[i] =  (*(uint32 *)((uint32)(((FLASH_SECTOR)*SECTOR_SIZE) + (i<<2)))); //flash_read(FLASH_SECTOR,i<<2,uint32);

	//读FLASH
	for(i = 1;i <=8;i ++)   //电感最大值
	{
		Adc.ad_max_val[i]= (uint16)(read_buffer1[i]);
	}
	
	Servo.kp = (float)(read_buffer1[9]/1000.0);
	Servo.kd = (float)(read_buffer1[10]/1000.0);
	Servo.distance_err_max_val = (float)(read_buffer1[11]/1000.0);
	Servo.distance_err_d_max_val = (float)(read_buffer1[12]/1000.0);
	
	Motor_control.Motor_Left_pid.kp		=	(float)(read_buffer1[13]/1000.0);
	Motor_control.Motor_Left_pid.ki		=	(float)(read_buffer1[14]/1000.0);
	Motor_control.Motor_Left_pid.kd		=	(float)(read_buffer1[15]/1000.0);
	Motor_control.Motor_Left_pid.kvff	=	(float)(read_buffer1[16]/1000.0);
	Motor_control.Motor_Left_pid.kaff	=	(float)(read_buffer1[17]/1000.0);
	
	Motor_control.Motor_Right_pid.kp	=	(float)(read_buffer1[18]/1000.0);
	Motor_control.Motor_Right_pid.ki	=	(float)(read_buffer1[19]/1000.0);
	Motor_control.Motor_Right_pid.kd	=	(float)(read_buffer1[20]/1000.0);
	Motor_control.Motor_Right_pid.kvff	=	(float)(read_buffer1[21]/1000.0);
	Motor_control.Motor_Right_pid.kaff	=	(float)(read_buffer1[22]/1000.0);
	
	TURN_Para	=	read_buffer1[23] - 10;
	Car_speed	=	read_buffer1[24];
	TURN_LOSE_VAL	=	read_buffer1[25];
	MOTOR_DUTY_LIMIT	=	read_buffer1[26];
	circle_turn_direction[0]	=	read_buffer1[27];
	circle_turn_direction[1]	=	read_buffer1[28];
	circle_turn_direction[2]	=	read_buffer1[29];
	circle_turn_direction[3]	=	read_buffer1[30];
	circle_turn_val[0]	=	read_buffer1[31];
	circle_turn_val[1]	=	read_buffer1[32];
	circle_turn_val[2]	=	read_buffer1[33];
	circle_turn_val[3]	=	read_buffer1[34];
	Ramp_speed			=	read_buffer1[35];
	EnableInterrupts;
}


