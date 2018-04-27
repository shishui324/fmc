#ifndef __ME_MATH_H_
#define __ME_MATH_H_

#include "common.h"


//	Ln(x)	泰勒展开---
float Ln_TaylorExpansion(float x);

//Sqrt使用卡马克算法实现
float Sqrt_by_Carmack(float number);

//卡马克算法求Sqrt倒数
float Carmack(float number);

//开3次方迭代法
float NthRoot_by_Iteration(float number);

//冒泡排序法---float
void Bubble_Sort_float_largen(float data[], uint8_t order[],uint8_t num);

//最小二乘法拟合斜率
float Slope_Calculate_Time(uint16_t num, int16_t *Data);

////float--fabs
float Me_fabs(float temp);

//float数据限幅
float Data_Limit_Range_float(float data,float max,float min);

//int16_t数据限幅
int16_t Data_Limit_Range_int16_t(int16_t data,int16_t max,int16_t min);





#endif

