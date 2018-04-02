#include	"sensor.h"
#include	"struct.h"
#include "headfile.h"


ADC_info        Adc={0};
Sensor_info     Sensor={0};

uint8 circle_left_flag = 0;
uint8 circle_right_flag = 0;
uint8 circle_in = 0;

float sum_12;   //���12�ĺ�
float sub_12;	//���12�Ĳ�

float sum_16_34 = 0;
float sub_25 = 0;

float  Power_V;

float once_uni_ad[SENSOR_NUM+1];  //һ�ι�һ��
float twice_uni_ad[SENSOR_NUM+1]; //���ι�һ��
uint32 update_dis1cm_encoder;

 uint16 ad_mid_val[10]; //AD������ֵ 
 uint32 ad_add_val[10]; //AD������ֵ��
 uint16 ad_avr_val[10]; //AD����ƽ��ֵ
 
 uint16 ad_max_val[10]; //AD���ֵ
 uint16 ad_max_temp[10];
 uint16 ad_int_val[10][3];
 uint16 ad_avr_temp[5][10];

//uint16 ad_max_temp[10];
//uint16 ad_int_val[10][3];
//uint16 ad_avr_temp[5][10];
//uint16 ad_mid_val[10]; //AD������ֵ
//uint32 ad_add_val[10]; //AD������ֵ��

//uint16 ad_avr_val[10]; //AD����ֵ

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
		adc_init(Induc_6);
		adc_init(Induc_5);
		adc_init(Induc_4);
//		adc_init(Induc_5);
//		adc_init(Induc_4);
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
static float voltageList[25] = { 0 };

void get_adc_int_value(void)    //��ֵ�˲�  ��ֵ�˲�   ��ȡƽ��ֵ
{
		uint8 i=0;
		uint8 k=0;
		uint16 temp = 0;
		for(i=0;i<10;i++)
		{ad_add_val[i]=0;}  //�ȶ���ͽ�������
		
		for(k = 0; k < 3; k++)
		{	
			for(i = 0; i <3 ;i++)
			{
				ad_avr_temp[i][1] = adc_once(Induc_1,ADC_12bit);    
				ad_avr_temp[i][2] = adc_once(Induc_2,ADC_12bit);    
				ad_avr_temp[i][3] = adc_once(Induc_3,ADC_12bit);    
				ad_avr_temp[i][4] = adc_once(Induc_4,ADC_12bit);      
				ad_avr_temp[i][5] = adc_once(Induc_5,ADC_12bit);    
				ad_avr_temp[i][6] = adc_once(Induc_6,ADC_12bit);
				
//				ad_avr_temp[i][7] = adc_once(Induc_7,ADC_12bit);
//				ad_avr_temp[i][8] = adc_once(Induc_8,ADC_12bit);
			}
		 //��ֵ�˲�
				for(i = 1;i <= 8;i++)
				{
						if(ad_avr_temp[0][i] > ad_avr_temp[2][i]) //ȷ��0 < 2
						{
								temp = Adc.ad_avr_temp[0][i];
								ad_avr_temp[0][i] = ad_avr_temp[2][i];
								ad_avr_temp[2][i] = temp;
						}
						if(ad_avr_temp[0][i] > ad_avr_temp[1][i]) //ȷ��0 < 1
						{
								temp = ad_avr_temp[0][i];
								ad_avr_temp[0][i] = ad_avr_temp[1][i];
								ad_avr_temp[1][i] = temp;
						}
						if(ad_avr_temp[1][i] > ad_avr_temp[2][i]) //ȷ��1 < 2
						{
								temp = Adc.ad_avr_temp[1][i];
								Adc.ad_avr_temp[1][i] = ad_avr_temp[2][i];
								Adc.ad_avr_temp[2][i] = temp;
						}   

						ad_add_val[i] +=  ad_avr_temp[1][i]; //��ֵ��
				}
		}
		for(i = 1; i <= 8; i++)
		{
				ad_avr_val[i] = (uint16)(ad_add_val[i]*0.333);  //��ֵ
		}
				//Power_V = (BAT_VAL /4096.0)*5*3;
				for(unsigned short n = 1; n < sizeof(voltageList) / sizeof(voltageList[0]); n++)
					voltageList[n - 1] = voltageList[n];
				voltageList[sizeof(voltageList) / sizeof(voltageList[0]) - 1] = ((float)BAT_VAL /4096.0f)*5.0f*3.0f;
				for(unsigned short n = 0; n < sizeof(voltageList) / sizeof(voltageList[0]); n++)
					Power_V = Power_V + voltageList[n];
				Power_V = Power_V / (float)(sizeof(voltageList) / sizeof(voltageList[0]));
				
}

/**************** FUCK_MY_CAR  *************************
 *  * �������� ��find_max_ad
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� ��Ѱ��ad���ֵ,ɨ����
 *  * ��    �� ��Panda_Lei
 *  * ��    �� : 2017/03/18
********************************************************/
void find_max_ad(void)
{
		uint8 i=0;
		get_adc_int_value();

		for(i = 1;i <= 6;i ++)
		{
				if(ad_avr_val[i] > ad_max_temp[i])
						ad_max_temp[i] = ad_avr_val[i];
		}
}
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


void deal_sensor(Sensor_info *sensor)//��д���
{
    char i;
//    get_adc_int_value();	//�˲���ADֵ
	
	 for(i = 1;i <= 6 ;i++)  //һ�ι�һ������
	{
//		if(ad_avr_val[i] > 2*ad_max_val[i])
//		   ad_avr_val[i] = 2*ad_max_val[i];

		  sensor->once_uni_ad[i] = (float)((ad_avr_val[i] *100.0)/ad_max_val[i]);
	}
//   sub_16 = sensor->once_uni_ad[1] - sensor->once_uni_ad[6];         //??16??
   sub_25 = sensor->once_uni_ad[2] - sensor->once_uni_ad[5];         //??25??
//   sub_34 = sensor->once_uni_ad[3] - sensor->once_uni_ad[4];
//   sub_78 = sensor->once_uni_ad[7] - sensor->once_uni_ad[8];
// 
//   right_angle_val = fabs(sub_25) - fabs(sub_16);                    //??25????????16???????

//  sum_34 = sensor->once_uni_ad[3] + sensor->once_uni_ad[4];
//  sum_78 = sensor->once_uni_ad[7] + sensor->once_uni_ad[8];
  
//  sum_16_25 = sensor->once_uni_ad[1] + sensor->once_uni_ad[5] + sensor->once_uni_ad[2]
//               + sensor->once_uni_ad[6];
  sum_16_34 = sensor->once_uni_ad[1] + sensor->once_uni_ad[6]
               + sensor->once_uni_ad[3] + sensor->once_uni_ad[4];
//  sum_16_34_25 = sensor->once_uni_ad[1] + sensor->once_uni_ad[6] + sensor->once_uni_ad[3]
//                  + sensor->once_uni_ad[4] + sensor->once_uni_ad[2] + sensor->once_uni_ad[5];
//  
//  multi_78 = (uint16)(sensor->once_uni_ad[7]*sensor->once_uni_ad[8]);
//  multi_25 = (uint16)(sensor->once_uni_ad[2]*sensor->once_uni_ad[5]);
//  multi_34 = (uint16)(sensor->once_uni_ad[3]*sensor->once_uni_ad[4]);
  
  sensor->twice_uni_ad[1] = sensor->once_uni_ad[1] / sum_16_34;
  sensor->twice_uni_ad[3] = sensor->once_uni_ad[3] / sum_16_34;
  sensor->twice_uni_ad[4] = sensor->once_uni_ad[4] / sum_16_34;
  sensor->twice_uni_ad[6] = sensor->once_uni_ad[6] / sum_16_34;
////    for(i = 1;i <= SENSOR_NUM ;i++)  //һ�ι�һ������
////    {
//        Adc.once_uni_ad[1] = 100*(float)(Adc.ad_avr_val[1]/AD_MAX_VAL);//�ٷֱ�
//        Adc.once_uni_ad[8] = 100*(float)(Adc.ad_avr_val[8]/AD_MAX_VAL);//�ٷֱ�
////    }
//    sum_12 = once_uni_ad[1] + once_uni_ad[2];   //���12�ĺ�
//		sub_12 = once_uni_ad[1] - once_uni_ad[2];
//    twice_uni_ad[1] = once_uni_ad[1] / sum_12 ;
//    twice_uni_ad[2] = once_uni_ad[2] / sum_12 ;



	if((abs(sub_25>25)&&sum_16_34>250)||circle_in)	//   �� �� ��
	{
		circle_in = 1;
		if(sensor->once_uni_ad[2]>sensor->once_uni_ad[5])
			circle_left_flag = 1;
		else
			circle_right_flag = 1;
	}
    else{
//		circle_left_flag = 0;
//		circle_in = 0;
//		circle_right_flag = 0;
		for(i = 9; i > 0; i--)
			{																	//����ƫ�����
			Servo.error[i] = Servo.error[i-1];
			}
			Servo.error[0] = 25*(sensor->twice_uni_ad[1] - sensor->twice_uni_ad[6]); //�����в�ֵ
																																							
			
			update_1cm_error();
	}


}




