#include "Myflash.h"



/*
保存左电机PID参数
返回值
0-----保存成功
1-----保存失败

*/
uint8 Save_Left_motor(void)
{
    uint8 i=1;

    DisableInterrupts;//关总中断
    FLASH_EraseSector(Left_Motor_Erase);		//擦除扇区

    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kp),4,4);             //写入四个字节到最后一个扇区 偏移0字节
    if(i)	return i;

    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.ki),4,8);
    if(i)	return i;
    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kd),4,12);
    if(i)	return i;
    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kvff),4,16);
    if(i)	return i;
    i=FLASH_WriteSector(Left_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Left_pid.kaff),4,20);
    return i;

//	EnableInterrupts;//开总中断

}
void Read_Left_motor(void)
{
    DisableInterrupts;//关总中断

    Motor_control.Motor_Left_pid.kp		=		flash_read(Left_Motor_Erase,4,float);
    Motor_control.Motor_Left_pid.ki		=		flash_read(Left_Motor_Erase,8,float);
    Motor_control.Motor_Left_pid.kd		=		flash_read(Left_Motor_Erase,12,float);
    Motor_control.Motor_Left_pid.kvff	=		flash_read(Left_Motor_Erase,16,float);
    Motor_control.Motor_Left_pid.kaff	=		flash_read(Left_Motor_Erase,20,float);

    EnableInterrupts;//开总中断

}

/*
保存右电机PID参数
返回值
0-----保存成功
1-----保存失败

*/
uint8 Save_Right_motor(void)
{
    uint8 i=1;

    DisableInterrupts;//关总中断
    FLASH_EraseSector(Right_Motor_Erase);		//擦除扇区

    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kp),4,4);             //写入四个字节到最后一个扇区 偏移0字节
    if(i)	return i;

    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.ki),4,8);
    if(i)	return i;
    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kd),4,12);
    if(i)	return i;
    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kvff),4,16);
    if(i)	return i;
    i=FLASH_WriteSector(Right_Motor_Erase,(const uint8 *)&(Motor_control.Motor_Right_pid.kaff),4,20);
    return i;

//	EnableInterrupts;//开总中断

}
void Read_Right_motor(void)
{
    DisableInterrupts;//关总中断

    Motor_control.Motor_Right_pid.kp		=		flash_read(Right_Motor_Erase,4,float);
    Motor_control.Motor_Right_pid.ki		=		flash_read(Right_Motor_Erase,8,float);
    Motor_control.Motor_Right_pid.kd		=		flash_read(Right_Motor_Erase,12,float);
    Motor_control.Motor_Right_pid.kvff	=		flash_read(Right_Motor_Erase,16,float);
    Motor_control.Motor_Right_pid.kaff	=		flash_read(Right_Motor_Erase,20,float);

    EnableInterrupts;//开总中断

}

/*
保存方向PID参数
返回值
0-----保存成功
1-----保存失败

*/
uint8 Save_Servo(void)
{
    uint8 i=1;

    DisableInterrupts;//关总中断
    FLASH_EraseSector(Servo_Erase);		//擦除扇区

    i=FLASH_WriteSector(Servo_Erase,(const uint8 *)&(Servo.kp),4,4);             //写入四个字节到最后一个扇区 偏移0字节
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

//	EnableInterrupts;//开总中断

}
void Read_Servo(void)
{
    DisableInterrupts;//关总中断

    Servo.kp											=			flash_read(Servo_Erase,4,float);
    Servo.ki											=			flash_read(Servo_Erase,8,float);
    Servo.kd											=			flash_read(Servo_Erase,12,float);
    Servo.distance_err_max_val		=			flash_read(Servo_Erase,16,float);
    Servo.distance_err_d_max_val	=			flash_read(Servo_Erase,20,float);
    Servo.max_dis_err							=			flash_read(Servo_Erase,24,float);
    Servo.max_dis_err_d						=			flash_read(Servo_Erase,28,float);

    EnableInterrupts;//开总中断

}

/*
保存速度参数
返回值
0-----保存成功
1-----保存失败

*/
uint8 Save_Speed(void)
{
    uint8 i=1;

    DisableInterrupts;//关总中断
    FLASH_EraseSector(Speed_Erase);		//擦除扇区

    i=FLASH_WriteSector(Speed_Erase,(const uint8 *)&(Speed.zhidao_speed_val),4,4);             //写入四个字节到最后一个扇区 偏移0字节
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

//	EnableInterrupts;//开总中断

}
void Read_Speed(void)
{
    DisableInterrupts;//关总中断


		
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



    EnableInterrupts;//开总中断

}


/*
读取所有值

*/
void Read_all(void)
{

    DisableInterrupts;//关总中断
    /*左电机参数*/
    Motor_control.Motor_Left_pid.kp			=		flash_read(Left_Motor_Erase,4,float);
    Motor_control.Motor_Left_pid.ki			=		flash_read(Left_Motor_Erase,8,float);
    Motor_control.Motor_Left_pid.kd			=		flash_read(Left_Motor_Erase,12,float);
    Motor_control.Motor_Left_pid.kvff		=		flash_read(Left_Motor_Erase,16,float);
    Motor_control.Motor_Left_pid.kaff		=		flash_read(Left_Motor_Erase,20,float);

    /*     右电机参数*/
    Motor_control.Motor_Right_pid.kp		=		flash_read(Right_Motor_Erase,4,float);
    Motor_control.Motor_Right_pid.ki		=		flash_read(Right_Motor_Erase,8,float);
    Motor_control.Motor_Right_pid.kd		=		flash_read(Right_Motor_Erase,12,float);
    Motor_control.Motor_Right_pid.kvff	=		flash_read(Right_Motor_Erase,16,float);
    Motor_control.Motor_Right_pid.kaff	=		flash_read(Right_Motor_Erase,20,float);


    /*				差速PID*/
    Servo.kp														=		flash_read(Servo_Erase,4,float);
    Servo.ki														=		flash_read(Servo_Erase,8,float);
    Servo.kd														=		flash_read(Servo_Erase,12,float);
    Servo.distance_err_max_val					=		flash_read(Servo_Erase,16,float);
    Servo.distance_err_d_max_val				=		flash_read(Servo_Erase,20,float);
    Servo.max_dis_err										=		flash_read(Servo_Erase,24,float);
    Servo.max_dis_err_d									=		flash_read(Servo_Erase,28,float);

    /*			速度*/
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



    EnableInterrupts;//开总中断


}






