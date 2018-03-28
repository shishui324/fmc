#ifndef __SENSOR_H
#define __SENSOR_H


#include "common.h"
/***********	芯片资源	*************************/
//    ADC0_SE0=0,  //A0
//    ADC0_SE1,    //A1
//    ADC0_SE2,    //A6
//    ADC0_SE3,    //A7
//    ADC0_SE4,    //B0
//    ADC0_SE5,    //B1
//    ADC0_SE6,    //B2
//    ADC0_SE7,    //B3
//    ADC0_SE8,    //C0
//    ADC0_SE9,    //C1
//    ADC0_SE10,   //C2
//    ADC0_SE11,   //C3
//    ADC0_SE12,   //F4
//    ADC0_SE13,   //F5
//    ADC0_SE14,   //F6
//    ADC0_SE15    //F7

/***********  电感ADC采集通道 宏定义 *****************/

#define  Induc_8    ADC0_SE15
#define  Induc_7    ADC0_SE14
#define  Induc_6    ADC0_SE13
#define  Induc_5    ADC0_SE12
#define  Induc_4    ADC0_SE7
#define  Induc_3    ADC0_SE6	
#define  Induc_2    ADC0_SE5     
#define  Induc_1    ADC0_SE4		 


#define  BAT      ADC0_SE3      // PTA7电池电压检测
#define  BAT_VAL  adc_once(BAT,ADC_12bit) 

#define SENSOR_NUM  2   //定义电感数量
#define AD_MAX_VAL 4095.0 //AD的最大值

extern float sum_12;   //电感12的和
extern float sub_12;	//电感12的差
extern float  Power_V;

extern uint16 ad_avr_val[10]; //AD采样平均值
extern float once_uni_ad[SENSOR_NUM+1];  //一次归一化
extern float twice_uni_ad[SENSOR_NUM+1]; //二次归一化




void ad_init(void); //ad初始化
void get_adc_int_value(void);    //中值滤波  均值滤波   求取平均值
//void get_adc_int_val(void);  //ad采样值

//void find_max_ad(void); //扫描最大值
//void deal_sensor(Sensor_info *sensor);//电感处理
void update_1cm_error(void);//更新1cm的偏差和偏差变化率
void deal_sensor(void);//电感处理






#endif



