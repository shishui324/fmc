#ifndef __STRUCT_H
#define __STRUCT_H
#include "common.h"

///////////////////////�ṹ�嶨��////////////////////////////////

//������
typedef struct 
{

  float once_uni_ad[9];  //һ�ι�һ��
  float twice_uni_ad[9]; //���ι�һ��
  float PARAM_A;         
  float PARAM_B;         
  float PARAM_CROSS;     

}Sensor_info;

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

//������ٶ�
typedef struct 
{ 
  uint8 INH;                    //���ʹ��
  int16 set_value_temp;         //ң���趨�ٶ���ʱ����
  int16 output;
  int16 speed_racc;             //������ٶ�
  float last_uk;               //��һ�εĿ������������ֵ
  float  kp;
  float  ki;
  float  kd; 
  float  kvff;      //�ٶ�ǰ��ϵ��
  float  kaff;      //���ٶ�ϵ��
  float delta_uk;
  float out_duty;
 
  uint16 kvff_param; 
  int16 present_value[10];  //������ʵ�ʲ��ֵ
  int16 error[10];          //������
  int16 set_value[10];      //�ٶ��趨����   
}Motor_pid_info;            //���PID�������Ϣ����


typedef struct
{
	Motor_pid_info	Motor_Left_pid;
	Motor_pid_info	Motor_Right_pid;
}Motor_control_info;

typedef struct
{
  uint16 zhidao_speed_val; //ֱ���ٶ�
  uint16 wandao_speed_val; //����ٶ�
  uint16 cross_speed_val;  //СS�ٶ�
  uint16 shizi_speed_val;  //ʮ���ٶ�
  uint16 ramp1_speed_val; //�µ��ٶ�
  uint16 ramp2_speed_val;
  uint16 speed_high_mode; //����ģʽʹ��
  uint16 high_speed_dis;  //����ģʽ����
  uint16 stop_car_enable; //ͣ��
  uint16 test_time;       //����ʱ��
}Speed_info;

//����pid
typedef struct
{
    float kp;
    float ki;
    float kd;
    int16 output;
    int error[10];						//���ƫ�����
	
	
	float  dis1cm_err_store[25];           //1cm��·ƫ��
	float  distance_err_max_val;           //���λƫ�� ,ң�ؿ��޸�
	float  distance_err_d_max_val;         //���ƫ����(ƫ��΢��),���޸�
	float  max_dis_err;                    // ���ƫ��仯
	float  max_dis_err_d;                  // ���ƫ��仯��
	
	int middle_val[10];
    
}Servo_info;    //����pid


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

extern 	ADC_info 			Adc;
extern 	Sensor_info 		Sensor;
extern 	Servo_info 			Servo;
extern	Motor_control_info 	Motor_control;
extern	Speed_info 			Speed;

#endif


