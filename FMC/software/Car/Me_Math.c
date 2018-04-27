#include "Me_Math.h"
//#include "arm_math.h"




/*
*	Ln(x)	泰勒展开---
*/
float Ln_TaylorExpansion(float x)
{
	float y = (x-1.0f)/(x+1.0f);
	float y2 = y * y;
	float power = 1.0f;
	float sum = 1.0f;

	uint16_t time;
	uint16_t time_max = (uint16_t)x;
	time_max = time_max / 10 + 1;
	time_max *= 10;

	for(time = 0;time < time_max;time++)
	{
		power = power * y2;
		sum = sum + (power / (2*time+3));
	}

	return (2 * y * sum);
}

//Sqrt使用卡马克算法实现
float Sqrt_by_Carmack(float number)
{
	int i;  
	float x2, y;  
	/*const	嵌入式环境const存储在flash反而更慢*/ float threehalfs = 1.5f;  
      
	x2 = number * 0.5f;  
	y  = number;  
	i  = * ( int * ) &y;            // evil floating point bit level hacking  
	//i  = 0x5f3759df - ( i >> 1 ); 
	i = 0x5f375a86 - (i >> 1);
	y  = * ( float * ) &i;  
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration  牛顿迭代
	//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed  
	
	return number*y;  
}

//卡马克算法求Sqrt倒数
float Carmack(float number)
{
	int i;  
	float x2, y;  
	/*const	嵌入式环境const存储在flash反而更慢*/ float threehalfs = 1.5f;  
      
	x2 = number * 0.5f;  
	y  = number;  
	i  = * ( int * ) &y;            // evil floating point bit level hacking  
	//i  = 0x5f3759df - ( i >> 1 ); 
	i = 0x5f375a86 - (i >> 1);
	y  = * ( float * ) &i;  
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration  牛顿迭代
	//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed  
	
	return y;
}  

//开3次方--迭代法
float NthRoot_by_Iteration(float number)
{
	float x0,x1,x00;

	if(number == 0.0f)
		return 0.0f;

	x0 = number;
	x1 = number;
	do
	{
		x0 = x1;
		x00 = x0 * x0;
		if(x00 == 0.0f)
			break;
		x1 = (2.0f * x0 + number / (x0 * x0)) / 3.0f;
	}while(Me_fabs(x1-x0) > 0.01f);
	return x1;
}


//冒泡排序法---float
void Bubble_Sort_float_largen(float data[], uint8_t order[],uint8_t num)
{
	uint8_t rank_flag = 0;		//排序标志
	uint16_t i;
	float data_temp;
	uint8_t order_temp;

	for(i = 0;i<num;i++)
	{
		order[i] = i;
	}

	num--;

	while(1)
	{
		for(i = 0;i<num;i++)//排序循环
		{
			if(data[i] > data[i+1])
			{
				data_temp = data[i];
				data[i] = data[i+1];
				data[i+1] = data_temp;
				order_temp = order[i];
				order[i] = order[i+1];
				order[i+1] = order_temp;
				rank_flag = 1;
			}
		}
		if(!rank_flag)
			break;
	}
}

//最小二乘法拟合斜率
float Slope_Calculate_Time(uint16_t num, int16_t *Data)    
{
//	static float resultlast;

	float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
	
	for (uint16_t i = 0; i < num; i++)
	{
		xsum += (float)i;
		ysum += Data[i];
		xysum += (float)i * (float)Data[i];
		x2sum += (float)(i * i);
	}

	float ChuShu = num * x2sum - xsum * xsum;
	if (ChuShu) //判断除数是否为零 
		return (num * xysum - xsum * ysum) / ChuShu;
	else
		return 0;
}

//最小二乘法拟合斜率
float Slope_Calculate_XY(uint8_t begin, uint8_t end, int16_t *p,int32_t *l)    
{
	static float resultlast;

	float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
	float result = 0;
	uint8_t i = 0;
	int32_t *L;
	
	L = l + begin;
	p = p + begin;
	l = l + begin;
	for (i = begin;i<end;i++)
	{
		xsum += (*l - *L);
		ysum += *p;
		xysum += (*l - *L) * (*p);
		x2sum += (*l - *L) * (*l - *L);
		p = p + 1;
		l = l + 1;
	}
	if ((end - begin)*x2sum - xsum*xsum) //判断除数是否为零 
	{
		result = ((end - begin)*xysum - xsum*ysum) / ((end - begin)*x2sum - xsum*xsum);
		resultlast = result;
	}
	else
	{
		result = resultlast;
	}
	return result;
}

//float--fabs
float Me_fabs(float temp)
{
	if(temp < 0.0f)
		return -temp;
	else
		return temp;
}


//float数据限幅
float Data_Limit_Range_float(float data,float max,float min)
{
	if(data < min)
		return min;
	else if(data > max)
		return max;
	return data;
}

//int16_t数据限幅
int16_t Data_Limit_Range_int16_t(int16_t data,int16_t max,int16_t min)
{
	if(data < min)
		return min;
	else if(data > max)
		return max;
	return data;
}


























