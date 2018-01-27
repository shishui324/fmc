#include	"sensor.h"
#include	"struct.h"

ADC_info        Adc={0};
//Sensor_info     Sensor={0};

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
void get_adc_int_val(void)    //��ֵ�˲�  ��ֵ�˲�   ��ȡƽ��ֵ
{
		uint8 i=0;
		uint8 k=0;
		uint16 temp = 0;
		for(i=0;i<10;i++)
		{Adc.ad_add_val[i]=0;}  //�ȶ���ͽ�������
		for(i = 0; i <3 ;i++)
		{

			Adc.ad_avr_temp[i][1] = adc_once(Induc_8,ADC_12bit);    
			Adc.ad_avr_temp[i][2] = adc_once(Induc_7,ADC_12bit);    
			Adc.ad_avr_temp[i][3] = adc_once(Induc_6,ADC_12bit);    
			Adc.ad_avr_temp[i][4] = adc_once(Induc_5,ADC_12bit);      
			Adc.ad_avr_temp[i][5] = adc_once(Induc_4,ADC_12bit);    
			Adc.ad_avr_temp[i][6] = adc_once(Induc_3,ADC_12bit);
			
			Adc.ad_avr_temp[i][7] = adc_once(Induc_2,ADC_12bit);
			Adc.ad_avr_temp[i][8] = adc_once(Induc_1,ADC_12bit);
		}
		 //��ֵ�˲�
		for(k = 0; k < 3; k++)
		{	
				for(i = 1;i <= 8;i++)
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
		for(i = 1; i <= 8; i++)
		{
				Adc.ad_avr_val[i] = (uint16)(Adc.ad_add_val[i]*0.333);  //��ֵ
		}
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
		get_adc_int_val();

		for(i = 1;i <= 8;i ++)
		{
				if(Adc.ad_avr_val[i] > Adc.ad_max_temp[i])
						Adc.ad_max_temp[i] = Adc.ad_avr_val[i];
		}
}


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
//  if(update_dis1cm_encoder >= 75) //1cm��Ӧ75��������
//  {
//   update_dis1cm_encoder = 0;
//   for(i = 24; i > 0;i--)
//    Servo.dis1cm_err_store[i] = Servo.dis1cm_err_store[i-1];
//    Servo.dis1cm_err_store[0] = Servo.error[0]; 
//  }
//  
//}




