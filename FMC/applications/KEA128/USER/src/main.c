#include "headfile.h"
#include "stdbool.h"
#include "math.h"

#include "Car_Key.h"

//n/6.16

//uint16 val[4];
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


//bool STOP_CAR_FLAG = true;
//bool isLeftlose = false;
//bool isLeftturn = false;
//bool isRightlose = false;
//bool isRightturn = false;


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


//extern void motor_pid_caculate(Motor_pid_info *motor_info);
//extern void get_adc_int_value(void);
//extern void deal_sensor(void);
//extern void servo_pid_caculate(void);
//extern void control(void);
//extern void motor_set(void);
//extern void speed_control(void);
/////////////////宏定义////////////////////////////
//#define DIR_CONTROL 1


////#define KEY_1_PIN C2
////#define KEY_2_PIN B5
#define KEY1 gpio_get(C2)
#define KEY2 gpio_get(B5)

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

int main(void)
{
    get_clk();              //获取时钟频率 必须执行

//		DisableInterrupts ;                  //禁止中断
    OLED_Init();
    //ADC初始化
    adc_init(ADC0_SE2); //PA6
    adc_init(ADC0_SE3); //PA7
    
    ftm_pwm_init(ftm2,ftm_ch0,15*1000,0);         //电机初始化 通道0 15Khz
    ftm_pwm_init(ftm2,ftm_ch1,15*1000,0);         //电机初始化 通道1 15Khz
    
    ftm_pwm_init(ftm2,ftm_ch2,15*1000,0);         //电机初始化 通道0 15Khz
    ftm_pwm_init(ftm2,ftm_ch3,15*1000,0);         //电机初始化 通道1 15Khz
    
    
    ftm_count_init(ftm0);   //左编码器
    gpio_init(C5,GPI,0);    //DIR初始化
    port_pull(C5);
    
    ftm_count_init(ftm1);   //右编码器
    gpio_init(H5,GPI,0);    //DIR初始化
    port_pull(H5);
		
//	gpio_init(C2,GPI,1);    //按键1初始化
	GPIOA->PIDR &= ~((uint32)1<<C2);	//PTC2取消禁用输入
	GPIOA->PDDR &= ~((uint32)1<<C2);	//将端口设置为输入或输出 0：输入 1：输出
	PORT_PUE0 |= (uint32)1<<C2;		//PTC2上拉使能
//    port_pull(C2);
//		
	gpio_init(B5,GPI,1);    //按键2初始化
    port_pull(B5);

//	Key_Message_Init();
    
    gpio_init(B1,GPO,1);    //LED1


//    
//    uart_init(DEBUG_PORT,DEBUG_BAUD);
    
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
//		ftm_pwm_duty(ftm2,ftm_ch0,val[0]);
//		ftm_pwm_duty(ftm2,ftm_ch1,val[1]);
//		ftm_pwm_duty(ftm2,ftm_ch2,val[2]);
//		ftm_pwm_duty(ftm2,ftm_ch3,val[3]);
    }
}





//普通计时服务
void time_5ms_serve(void)
{
	
   control();
}	
void time_20ms_serve(void)
{
//	ANO_DT_Data_Exchange_ToMe();		//匿名上位机对应
}

void time_10ms_serve(void)
{
	static uint32_t time_20ms_ct = 0;

	time_20ms_ct += 1;
	if (time_20ms_ct >= 50)
	{
		time_20ms_ct = 0;
	}
	
//	Car_Gather_Data_Key((uint8_t)20);		//Key采集
//	UI_OLED_play(20);							//UI处理
	
//			Car_Gather_Data_Key(10);
		if(!((GPIOA->PDIR>>C2)&0x01))
		{
			systick_delay_ms(10);
			if(!KEY1)
			{
				KEY_FLAG++;
				if(KEY_FLAG>=3)
					KEY_FLAG = 0;
				while(!KEY1);
			}
		}
		if(!KEY2)
		{
			systick_delay_ms(10);
			if(!KEY2)
			{
				STOP_CAR_FLAG = !STOP_CAR_FLAG;
				while(!KEY2);
			}
		}

//		if(Key_Inquire_data(Key_Up_Read) == Key_bit_Drop)
//		{
//			KEY_FLAG++;
//			if(KEY_FLAG>=3)
//				KEY_FLAG = 0;
//		}
//		if(Key_Inquire_data(Key_Down_Read) == Key_bit_Drop)
//		{
//			STOP_CAR_FLAG = !STOP_CAR_FLAG;
//		}

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
        if(getCountNum_L>0)
            gpio_set(B1,0);
        else
			gpio_set(B1,1);
		}
		break;
		case 1:
		{
			Cache_OLED_P6x8floatNum(0,0,(float)(getCountNum_L/Speed_Ratio));
			Cache_OLED_P6x8Num(0,1,(int8_t)Servo.output);
			
//			Cache_OLED_P6x8Num(0,2);
		
			
			
		}
		break;
		default:
			break;
		}
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

