#include "headfile.h"
#include "struct.h"
#include "stdbool.h"
#include "math.h"
#include "MYUI.h"

//n/6.16

////////////////////////服务函数标志位////////////////////
uint32_t time_2ms = 0;

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


uint16_t encode_time=0;
uint16_t sensor_time=0;
uint16_t control_time=0;
uint16_t UI_time=12;









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

//////////////////标志位定义////////////////////
float Speed_Ratio = 6.16;




int main(void)
{
	  get_clk();              //获取时钟频率 必须执行

		DisableInterrupts ;                  //禁止中断
    OLED_Init();
	

		
    //ADC初始化
		ad_init(); //ad初始化
		motor_init();//电机初始化
		encode_init();//编码器初始化
		Key_Message_Init(); //按键初始化
	
    gpio_init(H6,GPO,1);    //蜂鸣器
	
		gpio_init(H5,GPO,0);    //LED2	
		gpio_init(H2,GPO,1);    //LED4
//
    uart_init(DEBUG_PORT,DEBUG_BAUD);
		
		
		pit_init(pit1,1000);								//定时1000个bus时钟后中断
		set_irq_priority(PIT_CH1_IRQn,2);				//设置优先级,根据自己的需求设置 可设置范围为 0 - 3  越小优先级越高
		enable_irq(PIT_CH1_IRQn);							//打开pit0的中断开关

		pit_init_ms(pit0,2);                            //初始化pit0 周期设置为1ms
    set_irq_priority(PIT_CH0_IRQn,1);	            //设置pit0优先级
    enable_irq(PIT_CH0_IRQn);			            //开启pit0中断
    EnableInterrupts;

	/*******参数初始化*******/
	  //速度                //
	Speed.cross_speed_val = 8;						//20;                 //20  6
	Speed.wandao_speed_val =8;						// 28;                //16  4
	Speed.zhidao_speed_val =10;						// 25;                //25  8
							
	Servo.kp = 1.22f	;//1.220f;//1.620f;//7.20f;	//5.2 4.2
	Servo.kd = 1.0f;//2.5f;   // 1.0  0.2	2.0
	Servo.max_dis_err = 0.0;
	Servo.distance_err_max_val = 10;	//12.0;
	Servo.max_dis_err_d = 0.0;
	Servo.distance_err_d_max_val = 4.0;	//5
		
		Motor_control.Motor_Left_pid.kvff = 1.0;
		Motor_control.Motor_Left_pid.kaff =2.0;
		Motor_control.Motor_Left_pid.kd = 2.0;
		Motor_control.Motor_Left_pid.ki = 8.0;
		Motor_control.Motor_Left_pid.kp = 15.0;

		Motor_control.Motor_Right_pid.kvff = 1.0;
		Motor_control.Motor_Right_pid.kaff = 2.0;
		Motor_control.Motor_Right_pid.kd = 2.0;
		Motor_control.Motor_Right_pid.ki = 8.0;
		Motor_control.Motor_Right_pid.kp = 15.0;   //1 1 3 2 9
//		Motor_control.Motor_Left_pid.set_value[0]=20;
//		Motor_control.Motor_Right_pid.set_value[0]=20;
	Speed.set_speed_val=20;
	
    while(1)
    {
			
		
		if(time_10ms){time_10ms--;time_10ms_serve();}
		if(time_20ms){time_20ms--;time_20ms_serve();}
	
    }
}





//普通计时服务

void time_20ms_serve(void)
{
//	uint8 i;
//	ANO_DT_Data_Exchange_ToMe();		//匿名上位机对应
//	float send_buf[6];
//	for(i=6;i>0;i--)
//	{
//				send_buf[i-1]=Sensor.once_uni_ad[i];

//	}	
	

//	int32_t send_buf[2];
//	send_buf[1]=Servo.error[0];
//	send_buf[0]=Servo.error[0]-Servo.error[1];		

	
	int16_t send_buf[6];
	
	pit_time_start(pit1);
	
	send_buf[5]=(int16_t)Servo.error[0];
	send_buf[4]=(int16_t)Servo.output;
	send_buf[3]=Motor_control.Motor_Left_pid.set_value[0];
	send_buf[2]=getCountNum_L;
	send_buf[1]=Motor_control.Motor_Right_pid.set_value[0];
	send_buf[0]=getCountNum_R;
	
	
	vcan_sendware(send_buf,sizeof(send_buf));
	
	control_time = (uint16_t)(pit_time_get(pit1)*1000/(bus_clk_khz));
	pit_close(pit1);
	
	
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
	
	
	
}


