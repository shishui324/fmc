#ifndef __SENSOR_H
#define __SENSOR_H


#include "common.h"
/***********	оƬ��Դ	*************************/
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

/***********  ���ADC�ɼ�ͨ�� �궨�� *****************/

#define  Induc_8    ADC0_SE0
#define  Induc_7    ADC0_SE1
#define  Induc_6    ADC0_SE2
#define  Induc_5    ADC0_SE3
#define  Induc_4    ADC0_SE4
#define  Induc_3    ADC0_SE5	
#define  Induc_2    ADC0_SE6     
#define  Induc_1    ADC0_SE7		 


#define  BAT      ADC0_SE10      // PTA7��ص�ѹ���
#define  BAT_VAL  adc_once(BAT,ADC_16bit) 




void ad_init(void); //ad��ʼ��
void get_adc_int_val(void);  //ad����ֵ
void find_max_ad(void); //ɨ�����ֵ
//void deal_sensor(Sensor_info *sensor);//��д���
void update_1cm_error(void);//����1cm��ƫ���ƫ��仯��






#endif



