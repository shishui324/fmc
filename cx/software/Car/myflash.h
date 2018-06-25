#ifndef __MYFLASH_H
#define __MYFLASH_H

#include "common.h"
#include "headfile.h"

//k60FN115����1024K�ĳ���Flash
//1024K�ĳ���Flash��Ϊ256��������ÿ������4K��С
//����������Ϊ256 ��������ȡֵ��ΧΪ 0~255

#define FLASH_SECTOR   (FLASH_SECTOR_NUM-20) //�����ú������������ȫ
#define FLASH_DATA_SIZE 400  //���ݻ�������С                                    //80
#define FLASH_OK        1  //�ɹ�����1��ʧ�ܷ���0
#define FLASH_FAILED    0

void flash_read_data(void);
uint8 flash_write_data(void);





#endif
