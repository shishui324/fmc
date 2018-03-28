#ifndef __MOTOR_H
#define __MOTOR_H
#include "common.h"



#define MOTOR_DUTY_MAX       600      //限制占空比，以防电流过大烧驱动
#define MOTOR_DUTY_MIN       -600


void motor_init(void );

void motor_set(void);


#endif
