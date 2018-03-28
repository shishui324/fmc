#include "headfile.h"
#include "stdbool.h"
#include "math.h"

#include "Car_Key.h"
#define   DEBUG_ON    0
#define 	Protect_ON		0
//n/6.16

//uint16 val[4];

//bool STOP_CAR_FLAG = true;
//bool isLeftlose = false;
//bool isLeftturn = false;
//bool isRightlose = false;
//bool isRightturn = false;
////////////////////////服务函数标志位////////////////////
uint32_t time_1ms = 0;
uint32_t time_5ms = 0;
uint32_t time_10ms = 0;
uint32_t time_20ms = 0;
uint32_t time_50ms = 0;
uint32_t time_100ms = 0;
uint32_t time_200ms = 0;
uint32_t time_500ms = 0;
uint32_t time_1s = 0;
uint32_t time_2s = 0;
uint32_t time_5s = 0;
uint32_t time_10s = 0;





////////////////函数声明///////////////////////
void time_10ms_serve(void);
void time_5ms_serve(void);
void time_20ms_serve(void);
void time_50ms_serve(void);
void time_100ms_serve(void);
void time_200ms_serve(void);
void time_500ms_serve(void);
void time_1s_serve(void);
void time_2s_serve(void);
void time_5s_serve(void);
void time_10s_serve(void);

/////调用外部函数声明


extern void control(void);
//extern void speed_control(void);
extern void Car_Gather_Data_Key(uint8_t time_ms);	
/////////////////宏定义////////////////////////////
//#define DIR_CONTROL 1

//////////////////标志位定义////////////////////
float Speed_Ratio = 6.16;
int8_t KEY_FLAG = 0;


///*************speed_control***************/
//uint8_t g_fLeftMotorSpeed = 0;	//左电机速度(测量量)		cm/s
//uint8_t g_fRightMotorSpeed = 0;	//右电机速度(测量量)		cm/s
////姿态数据
//uint8_t g_fCarSpeed = 0;				//速度(测量)
//uint8_t g_fLeft_Tyre_Speed = 0;
//uint8_t g_fRight_Tyre_Speed = 0;
//uint8_t ControlSpeed = 0;				//迟滞速度
//uint8_t LeftControlSpeed = 0;				//迟滞速度
//uint8_t RightControlSpeed = 0;				//迟滞速度

////速度控制项输出
//uint8_t g_fSpeedControlOut = 0;		//速度控制输出
//uint8_t g_fLeftMotorOut = 0;			//左电机输出
//uint8_t g_fRightMotorOut = 0;			//右电机输出
/*****************************************/


uint8 sector = FLASH_SECTOR_NUM - 1;



int main(void)
{
	  get_clk();              //获取时钟频率 必须执行

		DisableInterrupts ;                  //禁止中断
    OLED_Init();
	
		FLASH_Init();
		
    //ADC初始化
		 ad_init(); //ad初始化
  
		motor_init();//电机初始化

		encode_init();//编码器初始化
		Key_Message_Init(); //按键初始化
	
	
    
    gpio_init(H6,GPO,0);    //LED1
		gpio_init(H5,GPO,0);    //LED2	
		gpio_init(H2,GPO,1);    //LED4
//		
    uart_init(DEBUG_PORT,DEBUG_BAUD);

		pit_init_ms(pit0,1);                            //初始化pit0 周期设置为1ms
    set_irq_priority(PIT_CH0_IRQn,1);	            //设置pit0优先级
    enable_irq(PIT_CH0_IRQn);			            //开启pit0中断
    EnableInterrupts;
    
	Cache_OLED_P6x8Str(0,0,"ALL_INIT");
    Cache_Update_OLED();
    pit_delay_ms(pit1,1000);

	/*******参数初始化*******/
	  //速度
	Speed.cross_speed_val = 20;                 //20  6
	Speed.wandao_speed_val = 28;                //16  4
	Speed.zhidao_speed_val = 25;                //25  8
	
	Servo.kp = 1.300f	;//1.220f;//1.620f;//7.20f;	//5.2 4.2
  Servo.kd = 0.55f;//2.5f;   // 1.0  0.2	2.0
	Servo.max_dis_err = 0.0;
	Servo.distance_err_max_val = 10;	//12.0;
	Servo.max_dis_err_d = 0.0;
	Servo.distance_err_d_max_val = 4.0;	//5
	
	Motor.kvff =1.00;            //20
	Motor.kaff = 10.00;           //10
	Motor.kd = 3.00;
	Motor.ki = 2.00;
	Motor.kp = 9.00;            //60
	
	Motor_control.Motor_Left_pid.kvff = 1.0;
	Motor_control.Motor_Left_pid.kaff = 1.0;
	Motor_control.Motor_Left_pid.kd = 2.0;
	Motor_control.Motor_Left_pid.ki = 4.0;
	Motor_control.Motor_Left_pid.kp = 12.0;
	
	Motor_control.Motor_Right_pid.kvff = 1.0;
	Motor_control.Motor_Right_pid.kaff = 1.0;
	Motor_control.Motor_Right_pid.kd = 2.0;
	Motor_control.Motor_Right_pid.ki = 4.0;
	Motor_control.Motor_Right_pid.kp = 12.0;   //1 1 3 2 9

    while(1)
    {
			
		if(time_5ms){time_5ms--;time_5ms_serve();}
		if(time_10ms){time_10ms--;time_10ms_serve();}
		if(time_20ms){time_20ms--;time_20ms_serve();}
		if(time_50ms){time_50ms--;time_50ms_serve();}
		if(time_100ms){time_100ms--;time_100ms_serve();}
		if(time_200ms){time_200ms--;time_200ms_serve();}
		if(time_500ms){time_500ms--;time_500ms_serve();}
		if(time_1s){time_1s--;time_1s_serve();}
		if(time_2s){time_2s--;time_2s_serve();}
		if(time_5s){time_5s--;time_5s_serve();}
		if(time_10s){time_10s--;time_10s_serve();}
    }
}





//普通计时服务
void time_5ms_serve(void)
{
#if DEBUG_ON 
		motor_set();
//			gpio_set(C0,(uint16 )flag);
//			ftm_pwm_duty(ftm2,ftm_ch5,duty);//左边电机输出
//			gpio_set(H3,(uint16 )flag);                                         
//			ftm_pwm_duty(ftm2,ftm_ch3,duty); //右边电机输出
#else
//	control();

	Motor_control.Motor_Left_pid.present_value[0] = getCountNum_L;
	Motor_control.Motor_Right_pid.present_value[0] = getCountNum_R;
	motor_pid_caculate(&Motor_control.Motor_Left_pid);
	motor_pid_caculate(&Motor_control.Motor_Right_pid);
	
	L_out_value = Motor_control.Motor_Left_pid.output;
	R_out_value = Motor_control.Motor_Right_pid.output;
	motor_set();

#endif   
	
	
}	
void time_20ms_serve(void)
{
	
//	ANO_DT_Data_Exchange_ToMe();		//匿名上位机对应
	uint16 buf[2];
	
	buf[1]=getCountNum_R;
	buf[0]=getCountNum_L;	/////山外上位机

	
	vcan_sendware(buf,sizeof(buf));
	
	
}

void time_10ms_serve(void)
{
	static uint32_t time_20ms_ct = 0;
 

	time_20ms_ct += 1;
	if (time_20ms_ct >= 50)
	{
		time_20ms_ct = 0;
	}

	Car_Gather_Data_Key((uint8_t)10);		//Key采集
	
#if 0
		
		if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop)||(Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
		{
		duty+=5;
		}
		if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop)||(Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
		{
		
				duty-=5;
		}
			
		if(Key_Inquire_data(Key_Left_Read) == Key_bit_Drop)
					{
							KEY_FLAG--;
							if(KEY_FLAG<0)
									KEY_FLAG = 2;
					}
		
		if(Key_Inquire_data(Key_Right_Read) == Key_bit_Drop)
					{
							KEY_FLAG++;
							if(KEY_FLAG>=3)
									KEY_FLAG = 0;
		
					}
		if(Key_Inquire_data(Key_Flag5_Read) == Key_bit_Drop)
					{
						cash=flash_read(sector,4,uint32);
							if(0==flag)
								flag=1;
							else flag=0;
					}
		if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Drop)
					{
//							
							FLASH_EraseSector(sector);
					flag=FLASH_WriteSector(sector,(const uint8 *)&duty,8,4); 
				if(0==flag)
								flag=1;
							else flag=0;
						
					}
					

	switch(KEY_FLAG)
		{
			case 0:
			{
		
//        Cache_OLED_P6x8Num(90,0,ad_avr_val[2]);
//				Cache_OLED_P6x8floatNum(80,0,(float)(3.24));
        Cache_OLED_Rectangle(0,  (63-((uint16_t)(ad_avr_val[1])*63/4095)),  8,   63);
        Cache_OLED_Rectangle(14, (63-((uint16_t)(L_out_value*63/1000))),  22,  63);
//        Cache_OLED_Rectangle(28, (63-((uint16_t)elec_data.Voltage_V_LL)/100), 36,  63);
//        Cache_OLED_Rectangle(42, (63-(uint16_t)((twice_uni_ad[1])*63)),  50,  63);
//        Cache_OLED_Rectangle(56, (uint16_t)(63-((twice_uni_ad[2])*63)),  64,  63);
//        Cache_OLED_Rectangle(70, (63-((uint16_t)elec_data.Voltage_V_RR)/100), 78,  63);
        Cache_OLED_Rectangle(84, (63-((uint16_t)(R_out_value*63/1000))),  92,  63);
        Cache_OLED_Rectangle(98, (63-((uint16_t)(ad_avr_val[2])*63/4095)),  106, 63);
				
				
				
      Cache_OLED_P6x8Num(0,0,getCountNum_L);
			Cache_OLED_P6x8Num(0,1,once_uni_ad[1]);
			Cache_OLED_P6x8Num(90,0,getCountNum_R);
			Cache_OLED_P6x8Num(90,1,once_uni_ad[2]);
//        Cache_OLED_P6x8Num(90,0,ad_avr_val[2]);
        
        Cache_OLED_P6x8floatNum(40,0,Servo.error[0]);
        Cache_OLED_P6x8floatNum(40,1,once_uni_ad[1]);
        Cache_OLED_P6x8floatNum(40,2,sum_12);
		
		
				
		}
		break;    
		case 1:
		{
			Cache_OLED_P6x8floatNum(0,0,(float)(getCountNum_L/Speed_Ratio));
			Cache_OLED_P6x8Num(0,1,(int8_t)Servo.output);
			
			 Cache_OLED_P6x8floatNum(40,0,ad_avr_val[0]);
			Cache_OLED_P6x8floatNum(40,0,ad_avr_val[1]);
			
//			Cache_OLED_P6x8Num(0,2);
		
			
		}
		break;
		default:
		{

		}
			break;
		}
		
		
		
		
#endif

		
		
			OLED_ConfigParameter();
			Cache_Update_OLED();	

}

void time_50ms_serve(void)
{

}

void time_100ms_serve(void)
{

}

void time_200ms_serve(void)
{

}

void time_500ms_serve(void)
{

}

void time_1s_serve(void)
{

}

void time_2s_serve(void)
{

}

void time_5s_serve(void)
{

}

void time_10s_serve(void)
{

}

