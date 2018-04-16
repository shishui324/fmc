#ifndef __ENCODE_H
#define __ENCODE_H

#include "headfile.h"
#include "struct.h"

extern int16_t getCountNum_L;  //获取编码器计数值
extern int16_t getCountNum_R;  //获取编码器计数值


void encode_init(void);
void get_num(void);



#endif 


