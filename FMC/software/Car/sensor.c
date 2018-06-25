#include "KEA128_adc.h"
#include "encode.h"
#include	"sensor.h"


ADC_info        Adc={0};
Sensor_info     Sensor={0};

uint8 circle_left_flag = 0;
uint8 circle_right_flag = 0;
uint8 circle_in = 0;
uint8 circle_counter=0;

float  Power_V;

//float once_uni_ad[SENSOR_NUM+1];  //һ�ι�һ��
//float twice_uni_ad[SENSOR_NUM+1]; //���ι�һ��
uint32 update_dis1cm_encoder;

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
//		adc_init(Induc_8);
//		adc_init(Induc_7);
//		adc_init(Induc_6);
//	
//		adc_init(Induc_5);
//		adc_init(Induc_4);
//	
//		adc_init(Induc_3);
//		adc_init(Induc_2);
//		adc_init(Induc_1);
//		
//		adc_init(BAT);
	  SIM->SCGC |= SIM_SCGC_ADC_MASK;         //����ADCʱ��
    ADC->SC3 = (0
                | ADC_SC3_ADIV(0)           //��Ƶϵ��
                | ADC_SC3_MODE(ADC_12bit)         //�ֱ���
                | ADC_SC3_ADICLK(0)         //ʹ������ʱ����ΪADC��ʱ��Դ
                );
        
    
    ADC->SC2 = ADC_SC2_REFSEL(0);           //��׼��ѹѡ��	

}
/**************** FUCK_MY_CAR  *************************
 *  * �������� ��get_adc_int_val
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� ��get_adc_int_val
 *  * ��    �� ��Panda_Lei
 *  * ��    �� : 2017/03/18
********************************************************/
//static float voltageList[25] = { 0 };

void get_adc_int_value(void)    //��ֵ�˲�  ��ֵ�˲�   ��ȡƽ��ֵ
{
	
				Adc.ad_avr_val[1] = adc_once(Induc_1,ADC_12bit);    
				Adc.ad_avr_val[2] = adc_once(Induc_2,ADC_12bit);    
				Adc.ad_avr_val[3] = adc_once(Induc_3,ADC_12bit);    
				Adc.ad_avr_val[4] = adc_once(Induc_4,ADC_12bit);      
				Adc.ad_avr_val[5] = adc_once(Induc_5,ADC_12bit);    
				Adc.ad_avr_val[6] = adc_once(Induc_6,ADC_12bit);	
				Adc.ad_avr_val[7] = adc_once(Induc_7,ADC_12bit);
				Adc.ad_avr_val[8] = adc_once(Induc_8,ADC_12bit);
	#if 0
		uint8 i=0;
		uint8 k=0;
		uint16 temp = 0;
		for(i=0;i<10;i++)
		{Adc.ad_add_val[i]=0;}  //�ȶ���ͽ�������
		
		for(k = 0; k < 4; k++)
		{	
			for(i = 0; i <3 ;i++)
			{
				Adc.ad_avr_temp[i][1] = adc_once(Induc_1,ADC_12bit);    
				Adc.ad_avr_temp[i][2] = adc_once(Induc_2,ADC_12bit);    
				Adc.ad_avr_temp[i][3] = adc_once(Induc_3,ADC_12bit);    
				Adc.ad_avr_temp[i][4] = adc_once(Induc_4,ADC_12bit);      
				Adc.ad_avr_temp[i][5] = adc_once(Induc_5,ADC_12bit);    
				Adc.ad_avr_temp[i][6] = adc_once(Induc_6,ADC_12bit);	
				Adc.ad_avr_temp[i][7] = adc_once(Induc_7,ADC_12bit);
				Adc.ad_avr_temp[i][8] = adc_once(Induc_8,ADC_12bit);
			}	
		 //��ֵ�˲�
				for(i = 1;i<= SENSOR_NUM;i++)
				{
						if(Adc.ad_avr_temp[0][i] > Adc.ad_avr_temp[2][i]) //ȷ��0 < 2
						{
								temp = Adc.ad_avr_temp[0][i];
								Adc.ad_avr_temp[0][i] = Adc.ad_avr_temp[2][i];
								Adc.ad_avr_temp[2][i] = temp;
						}
						if(Adc.ad_avr_temp[0][i] > Adc.ad_avr_temp[1][i]) //ȷ��0 < 1
						{
								temp = Adc.ad_avr_temp[0][i];
								Adc.ad_avr_temp[0][i] = Adc.ad_avr_temp[1][i];
								Adc.ad_avr_temp[1][i] = temp;
						}
						if(Adc.ad_avr_temp[1][i] > Adc.ad_avr_temp[2][i]) //ȷ��1 < 2
						{
								temp = Adc.ad_avr_temp[1][i];
								Adc.ad_avr_temp[1][i] = Adc.ad_avr_temp[2][i];
								Adc.ad_avr_temp[2][i] = temp;
						}   

						Adc.ad_add_val[i] +=  Adc.ad_avr_temp[1][i]; //��ֵ��
				}
					
		}
		for(i = 1; i <= SENSOR_NUM; i++)
		{
				Adc.ad_avr_val[i] = (Adc.ad_add_val[i]>>2);  //��ֵ
		}
		
				for(unsigned short n = 1; n < sizeof(voltageList) / sizeof(voltageList[0]); n++)
					voltageList[n - 1] = voltageList[n];
				voltageList[sizeof(voltageList) / sizeof(voltageList[0]) - 1] = ((float)BAT_VAL /4096.0f)*5.0f*3.0f;
				for(unsigned short n = 0; n < sizeof(voltageList) / sizeof(voltageList[0]); n++)
					Power_V = Power_V + voltageList[n];
				Power_V = Power_V / (float)(sizeof(voltageList) / sizeof(voltageList[0]));
	#endif


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
		for(i = 1;i <= SENSOR_NUM;i ++)
		{
				if(Adc.ad_max_val[i] < Adc.ad_avr_val[i])
						Adc.ad_max_val[i] = Adc.ad_avr_val[i];
				if(Adc.ad_max_val[i]>4096)
				{
					Adc.ad_max_val[i]=0;
				}
		}
		
}

void update_1cm_error(void)
{
  uint8 i = 0;

  update_dis1cm_encoder += (getCountNum_L + getCountNum_R)/2;//�ۼӵ�ǰ���������������
  
  if(update_dis1cm_encoder >= 48) //1cm��Ӧ24��������
  {
   update_dis1cm_encoder = 0;
   for(i = 48; i > 0;i--)
    Servo.dis1cm_err_store[i] = Servo.dis1cm_err_store[i-1];
    Servo.dis1cm_err_store[0] = Servo.error[0]; 
  }
}


void deal_sensor(Sensor_info *sensor)//��д���
{
    char i;
	
	find_max_ad();
	
	 for(i = 1;i <= SENSOR_NUM ;i++)  //һ�ι�һ������
	{
//		if(ad_avr_val[i] > 2*ad_max_val[i])
//		   ad_avr_val[i] = 2*ad_max_val[i];

		  sensor->once_uni_ad[i] = (float)((Adc.ad_avr_val[i] *100.0)/4096.0f);
	}

	for(i = 4; i > 0; i--)
			{																	//����ƫ�����
			Sensor.sub_25[i] = Sensor.sub_25[i-1];
			}
	sensor->sub_25[0] = sensor->once_uni_ad[2] - sensor->once_uni_ad[5];         //??25??
	sensor->sub_25_d  = sensor->sub_25[0]-sensor->sub_25[1];	
		
	sensor->sum_16_34 = (sensor->once_uni_ad[1] + sensor->once_uni_ad[6]
										 + sensor->once_uni_ad[3] + sensor->once_uni_ad[4]);
	sensor->sum_16=			(sensor->once_uni_ad[1] + sensor->once_uni_ad[6]);

  
  sensor->twice_uni_ad[1] = sensor->once_uni_ad[1] / sensor->sum_16;
  sensor->twice_uni_ad[3] = sensor->once_uni_ad[3] / sensor->sum_16_34;
  sensor->twice_uni_ad[4] = sensor->once_uni_ad[4] / sensor->sum_16_34;
  sensor->twice_uni_ad[6] = sensor->once_uni_ad[6] / sensor->sum_16;
	
  sensor->sub_16 = sensor->once_uni_ad[1] - sensor->once_uni_ad[6];         //??16??
#if 0
//		if((circle_flag))
//		{

//			circle_counter++;
//		}

	if(abs(sensor->sub_25[0])>30)									//200
//	if((abs(sub_25[0])>35&&(sum_16_34>250))&&((circle_counter>200)||!(circle_flag))&&!(circle_in))	//   �� �� ��
	{
//		circle_in = 1;
		
		if((sensor->sum_16_34>250)&&(abs(sensor->sub_25[0])>20))

			Bell_Cry(300,300);
		
//		circle_flag=false;
//		circle_counter=0;
//		if(!(circle_left_flag)&&!(circle_right_flag))
//		{
//		if(sensor->once_uni_ad[2]>sensor->once_uni_ad[5])
//			circle_left_flag = 1;
//		else
//			circle_right_flag = 1;
//		}

	}
    else{
		circle_left_flag = 0;
		circle_in = 0;
		circle_right_flag = 0;
		
		}
#endif 																																
			
			update_1cm_error();
//	}


}




