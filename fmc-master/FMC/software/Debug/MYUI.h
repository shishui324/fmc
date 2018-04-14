
#ifndef MYUI_H_
#define MYUI_H_

#include "headfile.h"
//调参界面ChangeParameterVal()函数修改某变量的值的时候使用
typedef enum{

	pint8_t,
	puint8_t,
	pint16_t,
	puint16_t,
    pint32_t,
    puint32_t,
	pfloat,
	pdouble,
}parameter_type;

extern uint32 duty;
extern uint8 flag;
//uint8_t GetKeyInpt();


void OLED_ConfigParameter(void );
//void clean_invalid_char(uint8_t endx);
void ChangeParameterVal(parameter_type ptype,void* ppara,double precision);
void ChangeFlagVal(uint8 *flag,uint8 min,uint8 max);


//void My_UI_Play(void);

#endif 

