#ifndef __MYFLASH_H
#define __MYFLASH_H

#include "common.h"
#include "headfile.h"

//k60FN115包含1024K的程序Flash
//1024K的程序Flash分为256个扇区，每个扇区4K大小
//即扇区总数为256 ，即扇区取值范围为 0~255

#define FLASH_SECTOR   (FLASH_SECTOR_NUM-20) //尽量用后面的扇区，安全
#define FLASH_DATA_SIZE 400  //数据缓冲区大小                                    //80
#define FLASH_OK        1  //成功返回1，失败返回0
#define FLASH_FAILED    0

void flash_read_data(void);
uint8 flash_write_data(void);





#endif
