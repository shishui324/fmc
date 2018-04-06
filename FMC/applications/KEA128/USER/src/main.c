#include "headfile.h"
#include "stdbool.h"
#include "math.h"

#include "Car_Key.h"

//n/6.16

//uint16 val[4];

//bool STOP_CAR_FLAG = true;
//bool isLeftlose = false;
//bool isLeftturn = false;
//bool isRightlose = false;
//bool isRightturn = false;
extern uint16 ad_max_val[10];
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
	
//		FLASH_Init();
		
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

	/*******参数初始化*******/
	  //速度                //
	Speed.cross_speed_val = 10;//20;                 //20  6
	Speed.wandao_speed_val =12;// 28;                //16  4
	Speed.zhidao_speed_val =12;// 25;                //25  8
	
	Servo.kp = 1.22f	;//1.220f;//1.620f;//7.20f;	//5.2 4.2
	Servo.kd = 2.2f;//2.5f;   // 1.0  0.2	2.0
	Servo.max_dis_err = 0.0;
	Servo.distance_err_max_val = 10;	//12.0;
	Servo.max_dis_err_d = 0.0;
	Servo.distance_err_d_max_val = 4.0;	//5
	
	
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
//	Motor.kp=10.0;
//	Motor.set_value[0]=11;
	uint8 i=1;
	for(i=1;i<=6;i++)
		ad_max_val[i]=0xfff;

		
		Motor_control.Motor_Left_pid.present_value[0] = 0;

    while(1)
    {
		
		
		if(time_5ms){time_5ms--;time_5ms_serve();}
		if(time_10ms){time_10ms--;time_10ms_serve();}
		if(time_20ms){time_20ms--;time_20ms_serve();}
    }
}





//普通计时服务
void time_5ms_serve(void)
{
	
//	control();
}	
void time_20ms_serve(void)
{
//	uint8 i;
////	ANO_DT_Data_Exchange_ToMe();		//匿名上位机对应
//	float send_buf[6];
//	for(i=6;i>0;i--)
//	{
//				send_buf[i-1]=Sensor.once_uni_ad[i];

//	}	
//		

//	
//	
//	vcan_sendware(send_buf,sizeof(send_buf));
//	
	
}

void time_10ms_serve(void)
{

	Car_Gather_Data_Key((uint8_t)10);		//Key采集
	if(!((Key_Flag2)||(Key_Flag3)||(Key_Flag4)))		//当拨码开关2,3,4全关上时才能发车，一可不关
	{
		if((Key_Inquire_data(Key_Flag5_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Flag5_Read) == Key_bit_Acc))
		{
			STOP_CAR_FLAG=false;
			motor_protect_time = 0;	
		}
		if(Key_Flag1)
		{	
		OLED_ConfigParameter();
		Cache_Update_OLED();	
		}			
	}
	else
	{
		OLED_ConfigParameter();
		Cache_Update_OLED();	
	}
	
	

//	Car_Gather_Data_Key((uint8_t)10);		//Key采集
//	OLED_ConfigParameter();
//	Cache_Update_OLED();	
	
}


