#ifndef _MYFLASH__H
#define _MYFLASH__H

#include "headfile.h"
#include "KEA128_flash.h"


#define  Left_Motor_Erase			255	//左电机PID参数所使用的扇区
#define  Right_Motor_Erase		254	//右电机PID参数所使用的扇区
#define  Servo_Erase					253	//方向PID参数所使用的的扇区
#define  Speed_Erase          252 //速度参数

//KEA128包含128K的程序Flash
//128K的程序Flash分为256个扇区，每个扇区512B大小
//即扇区总数为256 ，即扇区取值范围为 0~255

#define FLASH_SECTOR   (FLASH_SECTOR_NUM-20) //尽量用后面的扇区，安全
#define FLASH_DATA_SIZE 40  //数据缓冲区大小                                    //80
#define FLASH_OK        0  //成功返回0，失败返回其他值，一般为1
#define FLASH_FAILED    1

void flash_read_data(void);
uint8 flash_write_data(void);


uint8 Save_Left_motor(void);
void Read_Left_motor(void);

uint8 Save_Right_motor(void);
void Read_Right_motor(void);

uint8 Save_Servo(void);
void Read_Servo(void);

uint8 Save_Speed(void);
void Read_Speed(void);

//void Read_all(void);

#endif


