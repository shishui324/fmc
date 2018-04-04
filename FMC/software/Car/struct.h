#ifndef __STRUCT_H
#define __STRUCT_H
#include "headfile.h"
#include "common.h"


#define SERVO_LIMIT_VAL 15		//18
#define speed_duty  150
#define DIR_CONTROL 1
#define DEBUG_ON   0
#define Protect_ON	1

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
  float last_uk;               //��һ�εĿ������������ֵ�������ռ�ձ�/10
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
  uint16_t zhidao_speed_val; //ֱ���ٶ�
  uint16_t wandao_speed_val; //����ٶ�
  uint16_t cross_speed_val;  //СS�ٶ�
  uint16_t shizi_speed_val;  //ʮ���ٶ�
  uint16_t ramp1_speed_val; //�µ��ٶ�
  uint16_t ramp2_speed_val;
  uint16_t speed_high_mode; //����ģʽʹ��
  uint16_t high_speed_dis;  //����ģʽ����
  uint16_t stop_car_enable; //ͣ��
  uint16_t test_time;       //����ʱ��
}Speed_info;

//����pid
typedef struct
{
    float kp;
    float ki;
    float kd;
    int16 output;
		int16 reserve0;
    int error[10];						//���ƫ�����
	
	
	float  dis1cm_err_store[25];           //1cm��·ƫ��
	float  distance_err_max_val;           //���λƫ�� ,ң�ؿ��޸�
	float  distance_err_d_max_val;         //���ƫ����(ƫ��΢��),���޸�
	float  max_dis_err;                    // ���ƫ��仯
	float  max_dis_err_d;                  // ���ƫ��仯�� 
    
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

//typedef struct
//{
//	Cache_OLED_P6x8Str(uint8_t x,uint8_t y,char ch[]);
//	Cache_OLED_P6x8Num(uint8_t x,uint8_t y,int32_t Num);
//	Cache_OLED_P6x8Num_0X(uint8_t x,uint8_t y,int32_t Num_0X);
//	Cache_OLED_P6x8floatNum(O_byte x,O_byte y,float Num);
//	Cache_OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
//	Cache_Update_OLED(void);
//}Oled_info;

extern ADC_info 		Adc;
extern Sensor_info 		Sensor;



#endif


