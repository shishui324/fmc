#include	"sensor.h"
#include	"struct.h"

ADC_info        Adc={0};
//Sensor_info     Sensor={0};

/**************** Small_Cap  ****************************
 *  * 函数名称 ：ad_init
 *  * 函数参数 : 无
 *  * 返回值   : 无
 *  * 函数功能 ：ADC初始化
 *  * 作    者 ：Panda_Lei
 *  * 日    期 : 2017/03/18
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
 *  * 函数名称 ：get_adc_int_val
 *  * 函数参数 : 无
 *  * 返回值   : 无
 *  * 函数功能 ：get_adc_int_val
 *  * 作    者 ：Panda_Lei
 *  * 日    期 : 2017/03/18
********************************************************/
void get_adc_int_val(void)    //中值滤波  均值滤波   求取平均值
{
		uint8 i=0;
		uint8 k=0;
		uint16 temp = 0;
		for(i=0;i<10;i++)
		{Adc.ad_add_val[i]=0;}  //先对其和进行清零
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
		 //中值滤波
		for(k = 0; k < 3; k++)
		{	
				for(i = 1;i <= 8;i++)
				{
						if(Adc.ad_avr_temp[0][i] > Adc.ad_avr_temp[2][i]) //确保0 < 2
						{
								temp = Adc.ad_avr_temp[0][i];
								Adc.ad_avr_temp[0][i] = Adc.ad_avr_temp[2][i];
								Adc.ad_avr_temp[2][i] = temp;
						}
						if(Adc.ad_avr_temp[0][i] > Adc.ad_avr_temp[1][i]) //确保0 < 1
						{
								temp = Adc.ad_avr_temp[0][i];
								Adc.ad_avr_temp[0][i] = Adc.ad_avr_temp[1][i];
								Adc.ad_avr_temp[1][i] = temp;
						}
						if(Adc.ad_avr_temp[1][i] > Adc.ad_avr_temp[2][i]) //确保1 < 2
						{
								temp = Adc.ad_avr_temp[1][i];
								Adc.ad_avr_temp[1][i] = Adc.ad_avr_temp[2][i];
								Adc.ad_avr_temp[2][i] = temp;
						}   

						Adc.ad_add_val[i] +=  Adc.ad_avr_temp[1][i]; //中值和
				}
		}
		for(i = 1; i <= 8; i++)
		{
				Adc.ad_avr_val[i] = (uint16)(Adc.ad_add_val[i]*0.333);  //均值
		}
}
/**************** FUCK_MY_CAR  *************************
 *  * 函数名称 ：find_max_ad
 *  * 函数参数 : 无
 *  * 返回值   : 无
 *  * 函数功能 ：寻找ad最大值,扫描用
 *  * 作    者 ：Panda_Lei
 *  * 日    期 : 2017/03/18
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
// *  * 函数名称  : 
// *  * 参    数  ：
// *  * 功    能  : 更新1cm偏差和偏差变化率
// *  * 日    期	：2017/04/13
//************************************************************/
//void update_1cm_error(void)
//{
//  uint8 i = 0;

//  update_dis1cm_encoder += Motor.present_value[0];//累加当前编码器测得脉冲数
//  
//  if(update_dis1cm_encoder >= 75) //1cm对应75个脉冲数
//  {
//   update_dis1cm_encoder = 0;
//   for(i = 24; i > 0;i--)
//    Servo.dis1cm_err_store[i] = Servo.dis1cm_err_store[i-1];
//    Servo.dis1cm_err_store[0] = Servo.error[0]; 
//  }
//  
//}




