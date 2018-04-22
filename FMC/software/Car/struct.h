#ifndef __STRUCT_H
#define __STRUCT_H


#include "common.h"


#define SERVO_LIMIT 100		//差速限副
#define speed_duty  150			
//#define DIR_CONTROL 1
#define DEBUG_ON   0
#define Protect_ON	1
#define SENSOR_NUM  7   //定义电感数量
///////////////////////结构体定义////////////////////////////////

//传感器
typedef struct 
{

  float once_uni_ad[SENSOR_NUM+1];  //一次归一化
  float twice_uni_ad[SENSOR_NUM+1]; //二次归一化
	

	float sub_16;	//电感16的差
	float sum_16;
	float sum_16_34;

	float sub_25[5];
	float sub_25_d;
  float PARAM_A;         
  float PARAM_B;         
  float PARAM_CROSS;     

}Sensor_info;

//ADC
typedef struct
{

 uint16 ad_mid_val[10]; //AD采样中值 
 uint32 ad_add_val[10]; //AD采样中值和
 uint16 ad_avr_val[10]; //AD采样平均值
 
 uint16 ad_max[10]; //AD最大值
 uint16 ad_int_val[10][3];
 uint16 ad_avr_temp[5][10];
	uint16_t ad_max_val[10];
}ADC_info;

//电机
typedef struct 
{ 
//  uint8 INH;                    //软件使能
  int16 set_value_temp;         //遥控设定速度临时变量
  int16 output;
  int16 speed_racc;             //电机加速度
	uint16 kvff_param; 
  float last_uk;               //上一次的控制量输出绝对值，即电机占空比/10
  float kp;
  float ki;
  float kd; 
  float kvff;      //速度前馈系数
  float kaff;      //加速度系数
  float delta_uk[3];
  float out_duty;
 
  
  int16 present_value[10];  //编码器实际测得值
  int16 error[10];          //误差队列
  int16 set_value[10];      //速度设定队列   
}Motor_pid_info;            //电机PID所需的信息类型


typedef struct
{
	Motor_pid_info	Motor_Left_pid;
	Motor_pid_info	Motor_Right_pid;
}Motor_control_info;

typedef struct
{
  uint16_t zhidao_speed_val; //直道速度
  uint16_t wandao_speed_val; //弯道速度
  uint16_t cross_speed_val;  //小S速度
  uint16_t shizi_speed_val;  //十字速度
  uint16_t ramp1_speed_val; //上坡道速度
  uint16_t ramp2_speed_val;		//下坡速度
  uint16_t speed_high_mode; //高速模式使能
  uint16_t high_speed_dis;  //高速模式定距
  uint16_t stop_car_enable; //停车
  uint16_t test_time;       //测试时间
	uint16_t present_speed_val;
	uint16_t set_speed_val;
}Speed_info;

//差速pid
typedef struct
{
    float kp;
    float ki;
    float kd;
    float output;
		int16 reserve0;
    int16_t error[10];						//电感偏差队列
	
	
	float  dis1cm_err_store[25];           //1cm道路偏差
	float  distance_err_max_val;           //最大单位偏差 ,遥控可修改
	float  distance_err_d_max_val;         //最大偏差率(偏差微分),可修改
	float  max_dis_err;                    // 最大偏差变化
	float  max_dis_err_d;                  // 最大偏差变化率 
    
}Servo_info;    //差速pid



//丢线
typedef struct
{
  uint16 lose_line_flag;        //丢线标志位
  uint16 lose_line_val;         //丢线阈值
  uint16 lose_line_counter;     //丢线次数
  uint16 turn_left_flag;        //左转标志位
  uint16 turn_right_flag;       //右转标志位
}LoseLine_info;

//十字
typedef struct
{
  uint16 cross_fazhi_in;        //进十字道电感25乘积阈值
  uint16 cross_fazhi_out_in;    //十字道123456电感值和阈值
  int16 cross_fazhi_out;       //出十字2*5阈值
  uint16 cross_counter;         //判别进入十字道次数计数
  uint16 cross_flag;            //十字道标志位
  uint16 cross_time;            //十字计数
}Cross_info;




#endif


