#ifndef __STRUCT_H
#define __STRUCT_H

#include "common.h"

//ADC
typedef struct
{

 uint16 ad_mid_val[10]; //AD������ֵ 
 uint32 ad_add_val[10]; //AD������ֵ��
 uint16 ad_avr_val[10]; //AD����ƽ��ֵ
 
 uint16 ad_max_val[10]; //AD���ֵ
 uint16 ad_max_temp[10];
 uint16 ad_int_val[10][3];
 uint16 ad_avr_temp[5][10];
}ADC_info;

////������ٶ�
//typedef struct 
//{ 
//  uint8 INH;                    //���ʹ��
//  int16 set_value_temp;         //ң���趨�ٶ���ʱ����
//  int16 output;
//  int16 speed_racc;             //������ٶ�
//  float last_uk;               //��һ�εĿ������������ֵ�������ռ�ձ�/10
//  float  kp;
//  float  ki;
//  float  kd; 
//  float  kvff;      //�ٶ�ǰ��ϵ��
//  float  kaff;      //���ٶ�ϵ��
// 
//  uint16 kvff_param; 
//  int16 present_value[10];  //������ʵ�ʲ��ֵ
//  int16 error[10];          //������
//  int16 set_value[10];      //�ٶ��趨����   
//}Motor_pid_info;             //���PID�������Ϣ����


//typedef struct
//{
//  uint16 zhidao_speed_val; //ֱ���ٶ�
//  uint16 wandao_speed_val; //����ٶ�
//  uint16 cross_speed_val;  //СS�ٶ�
//  uint16 shizi_speed_val;  //ʮ���ٶ�
//  uint16 zhijiao_speed_val;//ֱ���ٶ�
//  uint16 ramp1_speed_val; //�µ��ٶ�
//  uint16 ramp2_speed_val;
//  uint16 speed_high_mode; //����ģʽʹ��
//  uint16 high_speed_dis;  //����ģʽ����
//  uint16 stop_car_enable; //ͣ��
//  uint16 test_time;       //����ʱ��
//}Speed_info;

//����
typedef struct
{
  uint16 lose_line_flag;        //���߱�־λ
  uint16 lose_line_val;         //������ֵ
  uint16 lose_line_counter;     //���ߴ���
  uint16 turn_left_flag;        //��ת��־λ
  uint16 turn_right_flag;       //��ת��־λ
}LoseLine_info;

//ʮ��
typedef struct
{
  uint16 cross_fazhi_in;        //��ʮ�ֵ����25�˻���ֵ
  uint16 cross_fazhi_out_in;    //ʮ�ֵ�123456���ֵ����ֵ
  int16 cross_fazhi_out;       //��ʮ��2*5��ֵ
  uint16 cross_counter;         //�б����ʮ�ֵ���������
  uint16 cross_flag;            //ʮ�ֵ���־λ
  uint16 cross_time;            //ʮ�ּ���
}Cross_info;

//typedef struct
//{
//	Cache_OLED_P6x8Str(uint8_t x,uint8_t y,char ch[]);
//	Cache_OLED_P6x8Num(uint8_t x,uint8_t y,int32_t Num);
//	Cache_OLED_P6x8Num_0X(uint8_t x,uint8_t y,int32_t Num_0X);
//	Cache_OLED_P6x8floatNum(O_byte x,O_byte y,float Num);
//	Cache_OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
//	Cache_Update_OLED(void);
//}Oled_info;

extern ADC_info      Adc;
//extern Oled_info		 Oled;
#endif


