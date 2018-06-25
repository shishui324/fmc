#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "common.h"
//#define SERVO_LIMIT_VAL 10		//18
void servo_pid_caculate(void);           //²îËÙ¿ØÖÆpid
void speed_dif(void);
uint8 CAL_SERVO_LIMIT_VAL(uint8 Car_speed);

#endif

//   8  3
//   10 5
//   12 7   8
//   13 8
//   14 9
//	 15 10

