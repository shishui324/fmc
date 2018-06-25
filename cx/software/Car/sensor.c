#include "headfile.h"

ADC_info        Adc={0};
Sensor_info     Sensor={0};

uint16 summer[9];

uint8 circle_in = 0;
uint8 circle_count = 0;

uint16 mid_sensor_max_distence = 0;

extern int16 aac_x,aac_y,aac_z;
extern int16 gyro_x,gyro_y,gyro_z;

uint8 ramp_in = 0;
uint8 ramp_count = 0;
uint16 ramp_distence = 0;

float sum_12;   //电感12的和
float sub_12;	//电感12的差

float sum_16_34 = 0;
float sub_25 = 0;

float  Power_V;

float once_uni_ad[SENSOR_NUM+1];  //一次归一化
float twice_uni_ad[SENSOR_NUM+1]; //二次归一化
uint32 update_dis1cm_encoder;

 uint16 ad_mid_val[10]; //AD采样中值 
 uint32 ad_add_val[10]; //AD采样中值和
 uint16 ad_avr_val[10]; //AD采样平均值
 
 uint16 ad_max_val[10]; //AD最大值
 uint16 ad_max_temp[10];
 uint16 ad_int_val[10][3];
 uint16 ad_avr_temp[5][10];
 
 int error_24;

//uint16 ad_max_temp[10];
//uint16 ad_int_val[10][3];
//uint16 ad_avr_temp[5][10];
//uint16 ad_mid_val[10]; //AD采样中值
//uint32 ad_add_val[10]; //AD采样中值和

//uint16 ad_avr_val[10]; //AD采样值

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
//	adc_init(Induc_7);
//	adc_init(Induc_6);
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
void get_adc_int_value(void)    //中值滤波  均值滤波   求取平均值
{
		uint8 i=0;
		uint8 k=0;
		uint16 temp = 0;
		for(i=1;i<=7;i++)
		{ad_add_val[i]=0;}  //先对其和进行清零
		
		for(k = 0; k < 3; k++)
		{	
			for(i = 0; i <3 ;i++)
			{
				ad_avr_temp[i][1] = adc_once(Induc_1,ADC_12bit);    
				ad_avr_temp[i][2] = adc_once(Induc_2,ADC_12bit);    
				ad_avr_temp[i][3] = adc_once(Induc_3,ADC_12bit);    
				ad_avr_temp[i][4] = adc_once(Induc_4,ADC_12bit);      
				ad_avr_temp[i][5] = adc_once(Induc_5,ADC_12bit);    
//				ad_avr_temp[i][6] = adc_once(Induc_6,ADC_12bit);
//				ad_avr_temp[i][7] = adc_once(Induc_7,ADC_12bit);
//				ad_avr_temp[i][8] = adc_once(Induc_8,ADC_12bit);
			}
		 //中值滤波
				for(i = 1;i <= 5;i++)
				{
						if(ad_avr_temp[0][i] > ad_avr_temp[2][i]) //确保0 < 2
						{
								temp = Adc.ad_avr_temp[0][i];
								ad_avr_temp[0][i] = ad_avr_temp[2][i];
								ad_avr_temp[2][i] = temp;
						}
						if(ad_avr_temp[0][i] > ad_avr_temp[1][i]) //确保0 < 1
						{
								temp = ad_avr_temp[0][i];
								ad_avr_temp[0][i] = ad_avr_temp[1][i];
								ad_avr_temp[1][i] = temp;
						}
						if(ad_avr_temp[1][i] > ad_avr_temp[2][i]) //确保1 < 2
						{
								temp = Adc.ad_avr_temp[1][i];
								Adc.ad_avr_temp[1][i] = ad_avr_temp[2][i];
								Adc.ad_avr_temp[2][i] = temp;
						}   

						ad_add_val[i] +=  ad_avr_temp[1][i]; //中值和
				}
		}
		for(i = 1; i <= 5; i++)
		{
				ad_avr_val[i] = (uint16)(ad_add_val[i]*0.333);  //均值
		}
				Power_V = (BAT_VAL /4096.0)*5*3;
				
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

	for(i = 1;i <= 6;i ++)
	{
		if(ad_avr_val[i] > ad_max_temp[i])
			ad_max_temp[i] = ad_avr_val[i];
	}
}

void find_middle_max_ad(void)
{
//	get_adc_int_value();
	Adc.ad_max_val[3] =  ad_avr_val[3] > ad_max_temp[3]? ad_avr_val[3]:ad_max_temp[3];
}

void update_1cm_error(void)
{
  uint8 i = 0;

  update_dis1cm_encoder += (Motor_control.Motor_Right_pid.present_value[0] + Motor_control.Motor_Left_pid.present_value[0])>>1;//累加当前编码器测得脉冲数
  
  if(update_dis1cm_encoder >= 24) //1cm对应75个脉冲数
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
    get_adc_int_value();	//滤波后AD值
	
	for(i=1;i<=5;i++)  //一次归一化处理
	{
		if(ad_avr_val[i] > 2*Adc.ad_max_val[i])
		   ad_avr_val[i] = 2*Adc.ad_max_val[i];
		sensor->once_uni_ad[i] = (float)((ad_avr_val[i] *100.0)/Adc.ad_max_val[i]);
	}

	for(i = 6; i > 0; i--)													//更新偏差队列
		summer[i] = summer[i-1];
  sum_16_34 = sensor->once_uni_ad[1] + sensor->once_uni_ad[5];
//               + sensor->once_uni_ad[2] + sensor->once_uni_ad[4]
	summer[0] = sum_16_34;

  sensor->twice_uni_ad[1] = sensor->once_uni_ad[1] / sum_16_34;
  sensor->twice_uni_ad[2] = sensor->once_uni_ad[2] / sum_16_34;//(sensor->once_uni_ad[2] + sensor->once_uni_ad[4]);
  sensor->twice_uni_ad[4] = sensor->once_uni_ad[4] / sum_16_34;//(sensor->once_uni_ad[2] + sensor->once_uni_ad[4]);
  sensor->twice_uni_ad[5] = sensor->once_uni_ad[5] / sum_16_34;
  
  if(sensor->once_uni_ad[3]==200)
  {
//	circle_count++;
//	if(circle_count==1)
//	{
//		mid_sensor_max_distence = 0;
//	}
//	circle_count = 2;
	mid_sensor_max_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>1;
	if(mid_sensor_max_distence>60)//&&aac_x<20
	{
		circle_in = 1;
		ramp_in = 0;
		ramp_distence = 0;
//		if(sensor->once_uni_ad[2]>sensor->once_uni_ad[5])
//			circle_left_flag = 1;
//		else
//			circle_right_flag = 1;
	}
	else
		ramp_in = 1;
  }
  if((sensor->once_uni_ad[3]>170||ramp_in)&&circle_in==0) //
  {
	ramp_in = 1;
	ramp_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>>1;
	if(ramp_distence>5500)//aac_x<-5
	{
		ramp_distence = 0;
		ramp_in = 0;
	}
  }
//  if(circle_in)
//  {
//	gpio_turn(H5);
//  }
//
//	if((abs(sub_25>25)&&sum_16_34>250)||circle_in)	//huan pan duan
//	{
//		circle_in = 1;
//		if(sensor->once_uni_ad[2]>sensor->once_uni_ad[5])
//			circle_left_flag = 1;
//		else
//			circle_right_flag = 1;
//	}
//    else
//	{
		for(i = 9; i > 0; i--)													//更新偏差队列
			Servo.error[i] = Servo.error[i-1];
		Servo.error[0] = 25*2*(sensor->twice_uni_ad[1] - sensor->twice_uni_ad[5]); //求出电感差值
		error_24 = 25*2*(sensor->twice_uni_ad[2] - sensor->twice_uni_ad[4]);
//	}
	Servo.middle_val[0] = (uint16)sensor->once_uni_ad[3];
}




