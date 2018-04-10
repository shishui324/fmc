#include	"sensor.h"
#include	"struct.h"
#include "headfile.h"


ADC_info        Adc={0};
Sensor_info     Sensor={0};

uint8 circle_left_flag = 0;
uint8 circle_right_flag = 0;
uint8 circle_in = 0;
uint8 circle_counter=0;

float  Power_V;

//float once_uni_ad[SENSOR_NUM+1];  //一次归一化
//float twice_uni_ad[SENSOR_NUM+1]; //二次归一化
uint32 update_dis1cm_encoder;

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
		adc_init(Induc_6);
		adc_init(Induc_5);
		adc_init(Induc_4);
	
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
static float voltageList[25] = { 0 };

void get_adc_int_value(void)    //中值滤波  均值滤波   求取平均值
{
		uint8 i=0;
		uint8 k=0;
		uint16 temp = 0;
		for(i=0;i<10;i++)
		{Adc.ad_add_val[i]=0;}  //先对其和进行清零
		
		for(k = 0; k < 3; k++)
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
		 //中值滤波
				for(i = 1;i<= SENSOR_NUM;i++)
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
		for(i = 1; i <= SENSOR_NUM; i++)
		{
				Adc.ad_avr_val[i] = (uint16)(Adc.ad_add_val[i]*0.333);  //均值
		}
		
				for(unsigned short n = 1; n < sizeof(voltageList) / sizeof(voltageList[0]); n++)
					voltageList[n - 1] = voltageList[n];
				voltageList[sizeof(voltageList) / sizeof(voltageList[0]) - 1] = ((float)BAT_VAL /4096.0f)*5.0f*3.0f;
				for(unsigned short n = 0; n < sizeof(voltageList) / sizeof(voltageList[0]); n++)
					Power_V = Power_V + voltageList[n];
				Power_V = Power_V / (float)(sizeof(voltageList) / sizeof(voltageList[0]));
				
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
		get_adc_int_value();

		for(i = 1;i <= SENSOR_NUM;i ++)
		{
				if(Adc.ad_avr_val[i] > Adc.ad_max_temp[i])
						Adc.ad_max_temp[i] = Adc.ad_avr_val[i];
		}
}

void update_1cm_error(void)
{
  uint8 i = 0;

  update_dis1cm_encoder += (getCountNum_L + getCountNum_R)/2;;//累加当前编码器测得脉冲数
  
  if(update_dis1cm_encoder >= 24) //1cm对应24个脉冲数
  {
   update_dis1cm_encoder = 0;
   for(i = 24; i > 0;i--)
    Servo.dis1cm_err_store[i] = Servo.dis1cm_err_store[i-1];
    Servo.dis1cm_err_store[0] = Servo.error[0]; 
  }
}


void deal_sensor(Sensor_info *sensor)//电感处理
{
    char i;
	
	 for(i = 1;i <= SENSOR_NUM ;i++)  //一次归一化处理
	{
//		if(ad_avr_val[i] > 2*ad_max_val[i])
//		   ad_avr_val[i] = 2*ad_max_val[i];

		  sensor->once_uni_ad[i] = (float)((Adc.ad_avr_val[i] *100.0)/Adc.ad_max_val[i]);
	}
  sensor->sub_16 = sensor->once_uni_ad[1] - sensor->once_uni_ad[6];         //??16??
	for(i = 4; i > 0; i--)
			{																	//更新偏差队列
			Sensor.sub_25[i] = Sensor.sub_25[i-1];
			}
 sensor->sub_25[0] = sensor->once_uni_ad[2] - sensor->once_uni_ad[5];         //??25??
		sensor->sub_25_d=sensor->sub_25[0]-sensor->sub_25[1];	
			
//   sub_34 = sensor->once_uni_ad[3] - sensor->once_uni_ad[4];
//   sub_78 = sensor->once_uni_ad[7] - sensor->once_uni_ad[8];
// 
//   right_angle_val = fabs(sub_25) - fabs(sub_16);                    //??25????????16???????

//  sum_34 = sensor->once_uni_ad[3] + sensor->once_uni_ad[4];
//  sum_78 = sensor->once_uni_ad[7] + sensor->once_uni_ad[8];
  
//  sum_16_25 = sensor->once_uni_ad[1] + sensor->once_uni_ad[5] + sensor->once_uni_ad[2]
//               + sensor->once_uni_ad[6];
 sensor->sum_16_34 = sensor->once_uni_ad[1] + sensor->once_uni_ad[6]
               + sensor->once_uni_ad[3] + sensor->once_uni_ad[4];
//  sum_16_34_25 = sensor->once_uni_ad[1] + sensor->once_uni_ad[6] + sensor->once_uni_ad[3]
//                  + sensor->once_uni_ad[4] + sensor->once_uni_ad[2] + sensor->once_uni_ad[5];
//  
//  multi_78 = (uint16)(sensor->once_uni_ad[7]*sensor->once_uni_ad[8]);
//  multi_25 = (uint16)(sensor->once_uni_ad[2]*sensor->once_uni_ad[5]);
//  multi_34 = (uint16)(sensor->once_uni_ad[3]*sensor->once_uni_ad[4]);
  
  sensor->twice_uni_ad[1] = sensor->once_uni_ad[1] / sensor->sum_16_34;
  sensor->twice_uni_ad[3] = sensor->once_uni_ad[3] / sensor->sum_16_34;
  sensor->twice_uni_ad[4] = sensor->once_uni_ad[4] / sensor->sum_16_34;
  sensor->twice_uni_ad[6] = sensor->once_uni_ad[6] / sensor->sum_16_34;
	


////    for(i = 1;i <= SENSOR_NUM ;i++)  //一次归一化处理
////    {
//        Adc.once_uni_ad[1] = 100*(float)(Adc.ad_avr_val[1]/AD_MAX_VAL);//百分比
//        Adc.once_uni_ad[8] = 100*(float)(Adc.ad_avr_val[8]/AD_MAX_VAL);//百分比
////    }
//    sum_12 = once_uni_ad[1] + once_uni_ad[2];   //电感12的和
//		sub_12 = once_uni_ad[1] - once_uni_ad[2];
//    twice_uni_ad[1] = once_uni_ad[1] / sum_12 ;
//    twice_uni_ad[2] = once_uni_ad[2] / sum_12 ;


//		if((circle_flag))
//		{

//			circle_counter++;
//		}

	if(abs(sensor->sub_25[0])>30)									//200
//	if((abs(sub_25[0])>35&&(sum_16_34>250))&&((circle_counter>200)||!(circle_flag))&&!(circle_in))	//   环 判 断
	{
//		circle_in = 1;
		
		if((sensor->sum_16_34>250)&&abs(sensor->sub_16)>10)
		Bell_Cry(500,500);
		else {
		
		Bell_Cry(500,100);
		}
	
		
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
	
		for(i = 9; i > 0; i--)
			{																	//更新偏差队列
			Servo.error[i] = Servo.error[i-1];
			}
			Servo.error[0] = 25*(sensor->twice_uni_ad[1] - sensor->twice_uni_ad[6]); //求出电感差值
																																							
			
			update_1cm_error();
//	}


}




