#ifndef _MYFLASH__H
#define _MYFLASH__H

#include "headfile.h"
#include "KEA128_flash.h"


#define  Left_Motor_Erase			255	//����PID������ʹ�õ�����
#define  Right_Motor_Erase		254	//�ҵ��PID������ʹ�õ�����
#define  Servo_Erase					253	//����PID������ʹ�õĵ�����
#define  Speed_Erase          252 //�ٶȲ���

//KEA128����128K�ĳ���Flash
//128K�ĳ���Flash��Ϊ256��������ÿ������512B��С
//����������Ϊ256 ��������ȡֵ��ΧΪ 0~255

#define FLASH_SECTOR   (FLASH_SECTOR_NUM-20) //�����ú������������ȫ
#define FLASH_DATA_SIZE 40  //���ݻ�������С                                    //80
#define FLASH_OK        0  //�ɹ�����0��ʧ�ܷ�������ֵ��һ��Ϊ1
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


