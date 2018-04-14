#ifndef __SERVO_H
#define __SERVO_H



#include "struct.h"



#define  SERVO_MID_VAL    3560

#define  SERVO_LEFT_VAL    4260  //620
#define  SERVO_RIGHT_VAL   2850  //720


void servo_init(void);

void servo_pid_caculate(void);





#endif
