#include	"sensor.h"
#include	"struct.h"
#include "headfile.h"


//ADC_info        Adc={0};
//Sensor_info     Sensor={0};


float sum_12;   //���12�ĺ�
float sub_12;	//���12�Ĳ�

float  Power_V;

float once_uni_ad[SENSOR_NUM+1];  //һ�ι�һ��
float twice_uni_ad[SENSOR_NUM+1]; //���ι�һ��
uint32 update_dis1cm_encoder;

//uint16 ad_max_temp[10];
//uint16 ad_int_val[10][3];
//uint16 ad_avr_temp[5][10];
//uint16 ad_mid_val[10]; //AD������ֵ
//uint32 ad_add_val[10]; //AD������ֵ��

uint16 ad_avr_val[10]; //AD����ֵ

/**************** Small_Cap  ****************************
 *  * �������� ��ad_init
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� ��ADC��ʼ��
 *  * ��    �� ��Panda_Lei
 *  * ��    �� : 2017/03/18
********************************************************/
void ad_init(void)
{
		adc_init(Induc_8);
		adc_init(Induc_7);
		adc_init(Induc_6);
		adc_init(Induc_5);
		adc_init(Induc_4);
		adc_init(Induc_3);
		adc_init(Induc_2);
		adc_init(Induc_1);
		
		adc_init(BAT);

}
/**************** FUCK_MY_CAR  *************************
 *  * �������� ��get_adc_int_val
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� ��get_adc_int_val
 *  * ��    �� ��Panda_Lei
 *  * ��    �� : 2017/03/18
********************************************************/
void get_adc_int_value(void)    //��ֵ�˲�  ��ֵ�˲�   ��ȡƽ��ֵ
{
//    uint8 i=0;
//    uint8 k=0;
//    uint16 temp = 0;
//    for(i=0;i<SENSOR_NUM;i++)      //�ȶ���ͽ�������
//    {
//        ad_add_val[i]=0;
//    }
//    //��ֵ�˲�
//    for(k = 0; k < 3; k++)
//    {
//        for(i = 0; i <3 ;i++)
//        {
//            ad_avr_temp[i][1] = adc_once(ADC0_SE2,ADC_12bit);
//            ad_avr_temp[i][2] = adc_once(ADC0_SE3,ADC_12bit);
//        }
//        for(i = 1;i <= SENSOR_NUM;i++)
//        {
//            if(ad_avr_temp[0][i] > ad_avr_temp[2][i]) //ȷ��0 < 2
//            {
//                temp = ad_avr_temp[0][i];
//                ad_avr_temp[0][i] = ad_avr_temp[2][i];
//                ad_avr_temp[2][i] = temp;
//            }
//            if(ad_avr_temp[0][i] > ad_avr_temp[1][i]) //ȷ��0 < 1
//            {
//                temp = Adc.ad_avr_temp[0][i];
//                ad_avr_temp[0][i] = ad_avr_temp[1][i];
//                ad_avr_temp[1][i] = temp;
//            }
//            if(ad_avr_temp[1][i] > ad_avr_temp[2][i]) //ȷ��1 < 2
//            {
//                temp = Adc.ad_avr_temp[1][i];
//                ad_avr_temp[1][i] = ad_avr_temp[2][i];
//                ad_avr_temp[2][i] = temp;
//            }
//              
//            ad_add_val[i] += ad_avr_temp[1][i]; //��ֵ��
//        }
//    }
// 
//    for(i = 1; i <= SENSOR_NUM; i++)
//    {
//        ad_avr_val[i] = (uint16)(ad_add_val[i]*0.333);  //��ֵ
//    }
        ad_avr_val[1] = adc_once(Induc_1,ADC_12bit);
        ad_avr_val[2] = adc_once(Induc_2 ,ADC_12bit);
				ad_avr_val[3] = adc_once(Induc_3 ,ADC_12bit);
				ad_avr_val[6] = adc_once(Induc_6 ,ADC_12bit);
				ad_avr_val[7] = adc_once(Induc_7 ,ADC_12bit);
				ad_avr_val[8] = adc_once(Induc_8 ,ADC_12bit);
				Power_V = (BAT_VAL /4096.0)*5*3;
				
}


#if 0
//void get_adc_int_val(void)    //��ֵ�˲�  ��ֵ�˲�   ��ȡƽ��ֵ
//{
//		uint8 i=0;
//		uint8 k=0;
//		uint16 temp = 0;
//		for(i=0;i<10;i++)
//		{Adc.ad_add_val[i]=0;}  //�ȶ���ͽ�������
//		for(i = 0; i <3 ;i++)
//		{

//			Adc.ad_avr_temp[i][1] = adc_once(Induc_8,ADC_12bit);    
//			Adc.ad_avr_temp[i][2] = adc_once(Induc_7,ADC_12bit);    
//			Adc.ad_avr_temp[i][3] = adc_once(Induc_6,ADC_12bit);    
//			Adc.ad_avr_temp[i][4] = adc_once(Induc_5,ADC_12bit);      
//			Adc.ad_avr_temp[i][5] = adc_once(Induc_4,ADC_12bit);    
//			Adc.ad_avr_temp[i][6] = adc_once(Induc_3,ADC_12bit);
//			
//			Adc.ad_avr_temp[i][7] = adc_once(Induc_2,ADC_12bit);
//			Adc.ad_avr_temp[i][8] = adc_once(Induc_1,ADC_12bit);
//		}
//		 //��ֵ�˲�
//		for(k = 0; k < 3; k++)
//		{	
//				for(i = 1;i <= 8;i++)
//				{
//						if(Adc.ad_avr_temp[0][i] > Adc.ad_avr_temp[2][i]) //ȷ��0 < 2
//						{
//								temp = Adc.ad_avr_temp[0][i];
//								Adc.ad_avr_temp[0][i] = Adc.ad_avr_temp[2][i];
//								Adc.ad_avr_temp[2][i] = temp;
//						}
//						if(Adc.ad_avr_temp[0][i] > Adc.ad_avr_temp[1][i]) //ȷ��0 < 1
//						{
//								temp = Adc.ad_avr_temp[0][i];
//								Adc.ad_avr_temp[0][i] = Adc.ad_avr_temp[1][i];
//								Adc.ad_avr_temp[1][i] = temp;
//						}
//						if(Adc.ad_avr_temp[1][i] > Adc.ad_avr_temp[2][i]) //ȷ��1 < 2
//						{
//								temp = Adc.ad_avr_temp[1][i];
//								Adc.ad_avr_temp[1][i] = Adc.ad_avr_temp[2][i];
//								Adc.ad_avr_temp[2][i] = temp;
//						}   

//						Adc.ad_add_val[i] +=  Adc.ad_avr_temp[1][i]; //��ֵ��
//				}
//		}
//		for(i = 1; i <= 8; i++)
//		{
//				Adc.ad_avr_val[i] = (uint16)(Adc.ad_add_val[i]*0.333);  //��ֵ
//		}
//}
/**************** FUCK_MY_CAR  *************************
 *  * �������� ��find_max_ad
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� ��Ѱ��ad���ֵ,ɨ����
 *  * ��    �� ��Panda_Lei
 *  * ��    �� : 2017/03/18
********************************************************/
//void find_max_ad(void)
//{
//		uint8 i=0;
//		get_adc_int_val();

//		for(i = 1;i <= 8;i ++)
//		{
//				if(Adc.ad_avr_val[i] > Adc.ad_max_temp[i])
//						Adc.ad_max_temp[i] = Adc.ad_avr_val[i];
//		}
//}


///***********************************************************
// *  * ��������  : 
// *  * ��    ��  ��
// *  * ��    ��  : ����1cmƫ���ƫ��仯��
// *  * ��    ��	��2017/04/13
//************************************************************/
//void update_1cm_error(void)
//{
//  uint8 i = 0;

//  update_dis1cm_encoder += Motor.present_value[0];//�ۼӵ�ǰ���������������
//  
//  if(update_dis1cm_encoder >= 75) //1cm��Ӧ��������
//  {
//   update_dis1cm_encoder = 0;
//   for(i = 24; i > 0;i--)
//    Servo.dis1cm_err_store[i] = Servo.dis1cm_err_store[i-1];
//    Servo.dis1cm_err_store[0] = Servo.error[0]; 
//  }
//  
//}

#endif
void update_1cm_error(void)
{
  uint8 i = 0;

  update_dis1cm_encoder += (getCountNum_L + getCountNum_R)/2;;//�ۼӵ�ǰ���������������
  
  if(update_dis1cm_encoder >= 24) //1cm��Ӧ75��������
  {
   update_dis1cm_encoder = 0;
   for(i = 24; i > 0;i--)
    Servo.dis1cm_err_store[i] = Servo.dis1cm_err_store[i-1];
    Servo.dis1cm_err_store[0] = Servo.error[0]/10; 
  }
}
void deal_sensor(void)//��д���
{
    char i;
    get_adc_int_value();
//    for(i = 1;i <= SENSOR_NUM ;i++)  //һ�ι�һ������
//    {
        once_uni_ad[1] = 100*(float)(ad_avr_val[1]/AD_MAX_VAL);//�ٷֱ�
        once_uni_ad[2] = 100*(float)(ad_avr_val[2]/AD_MAX_VAL);//�ٷֱ�
//    }
    sum_12 = once_uni_ad[1] + once_uni_ad[2];   //���12�ĺ�
	sub_12 = once_uni_ad[1] - once_uni_ad[2];
    twice_uni_ad[1] = once_uni_ad[1] / sum_12 ;
    twice_uni_ad[2] = once_uni_ad[2] / sum_12 ;
    
    for(i = 9; i > 0; i--)																	//����ƫ�����
         Servo.error[i] = Servo.error[i-1];
	
	update_1cm_error();

}




