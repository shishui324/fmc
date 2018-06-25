#include "headfile.h"
#include "initial.h"
#include "fuzzy.h"

//n/6.16

extern uint8 ramp_in;
extern uint8 circle_in;
extern uint16 mid_sensor_max_distence;
extern uint8 circle_left_flag;
extern uint8 circle_right_flag;

uint8 start_car_flag = 0;
uint8 start_car = 0;
uint8 start_check_stop = 0;
uint32 start_check_distence = 0;
uint16 start_count = 0;
uint8 stop_car = 0;
uint8 MOTOR_EN = 0;
extern int error_24;

uint16 	left_pulse_temp[5] 	= 	{0};           //左编码器读数缓冲区
uint16 	right_pulse_temp[5] = 	{0};           //右编码器读数缓冲区

uint16_t	Sensor_Time = 0;
uint16_t	Direction_Time = 0;
uint16_t	Speed_Time = 0;
uint16_t	Gyro_Time = 0;

extern char Position;
extern float sum_16_34;

////////////////////////服务函数标志位////////////////////  
uint32_t time_1ms = 0;
uint32_t time_5ms = 0;
uint32_t time_10ms = 0;
uint32_t time_20ms = 0;
uint32_t time_50ms = 0;
uint32_t time_100ms = 0;

////////////////函数声明///////////////////////
void time_100ms_serve(void);
void time_50ms_serve(void);
void time_20ms_serve(void);
void time_10ms_serve(void);
void time_5ms_serve(void);

/////////////////宏定义////////////////////////////
//#define DIR_CONTROL 1

//////////////////标志位定义////////////////////

extern uint16 ad_avr_val[10];

int16 aac_x = 0,aac_y = 0,aac_z = 0;
int16 gyro_x = 0,gyro_y = 0,gyro_z = 0;


float Speed_Ratio = 6.16;
int8_t KEY_FLAG = 0;

uint8 sector = FLASH_SECTOR_NUM - 20;

int main(void)
{
	fmc_all_init();		//初始化zzz
	someParaInit();		//参数初始化
	view_start_up();	//启动界面

	enable_irq(IRQ_IRQn);
	enable_irq(PIT_CH0_IRQn);			//开启pit0中断
    EnableInterrupts;
	
    while(1)
    {	
		if(time_5ms){time_5ms--;time_5ms_serve();}
		if(time_10ms){time_10ms--;time_10ms_serve();}
		if(time_10ms){time_10ms--;time_10ms_serve();}
		if(time_20ms){time_20ms--;time_20ms_serve();}
		if(time_50ms){time_50ms--;time_50ms_serve();}
		if(time_100ms){time_100ms--;time_100ms_serve();}
    }
}

void PIT_CH0_IRQHandler(void)
{
	PIT->CHANNEL->TFLG = 1;
	
	if(start_car_flag)
	{
		start_count++;
		BB_Cry(500,500);
		bling_bling_led0(200,200);
		bling_bling_led1(300,200);
		BB_Play();
		if(start_count>3000)
		{
			MOTOR_EN = 1;
			start_count = 0;
			start_car_flag = 0;
		}
	}
	
	if(MOTOR_EN == 1&& start_check_stop == 0)
	{
		start_check_distence+=(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>>1;
		if(start_check_distence>12000)
			start_check_stop = 1;
	}
//	pit_time_start(pit1);
//	MPU_Get_Gyroscope(&gyro_x,&gyro_y,&gyro_z);
//	MPU_Get_Accelerometer(&aac_x,&aac_y,&aac_z);
//	aac_x=aac_x*90/16500;
//	Gyro_Time = (uint16_t)(pit_time_get(pit1)*1000/bus_clk_khz);
//	pit_close(pit1);
	
//	pit_time_start(pit1);
	deal_sensor(&Sensor);					//电感处理
//	Sensor_Time = (uint16_t)(pit_time_get(pit1)*1000/bus_clk_khz);
//	pit_close(pit1);
	
//	pit_time_start(pit1);
//	speed_control();
//	Servo.kp = fuzzy_kp(Servo.error[0],(Servo.error[0]-Servo.error[9]));
	if(abs(Servo.error[0])<=8)
		Servo.kp = 0.820;	//0.820
	else
	{	
		BB_Cry(1996,4);
		Servo.kp = 1.220;	//1.920  1.120 1.620
	}
//	
//	Servo.kp = 0.3 * Servo.kp;
	
	servo_pid_caculate();					//转向PID
//	Direction_Time = (uint16_t)(pit_time_get(pit1)*1000/bus_clk_khz);
//	pit_close(pit1);
	
//	pit_time_start(pit1);
//	speed_control();
	speed_dif();							//差速
	update_1cm_error();         			//更新1cm道路偏差
	
//		pit_time_start(pit1);
	/********电机控制********/
	//左编码器
	left_pulse_temp[0] = 0;
	left_pulse_temp[0] = FTM0->CNT;		//读取左编码器脉冲数
	FTM0->CNT=0;						//清左编码器值
	if(gpio_get(C5))
		Motor_control.Motor_Left_pid.present_value[0] = (left_pulse_temp[0]+left_pulse_temp[1]+left_pulse_temp[2]+left_pulse_temp[3]+left_pulse_temp[4])/5;	//反转
	else
		Motor_control.Motor_Left_pid.present_value[0] =  -(left_pulse_temp[0]+left_pulse_temp[1]+left_pulse_temp[2]+left_pulse_temp[3]+left_pulse_temp[4])/5;	//正转
	
	//右编码器
	right_pulse_temp[0] = 0;
	right_pulse_temp[0] = FTM1->CNT;	//读取右编码器脉冲数
	FTM1->CNT=0;		               	//清右编码器值
	if(gpio_get(H5))
		Motor_control.Motor_Right_pid.present_value[0] =   -(right_pulse_temp[0]+right_pulse_temp[1]+right_pulse_temp[2]+right_pulse_temp[3]+right_pulse_temp[4])/5;       //正转
	else
		Motor_control.Motor_Right_pid.present_value[0] =  (right_pulse_temp[0]+right_pulse_temp[1]+right_pulse_temp[2]+right_pulse_temp[3]+right_pulse_temp[4])/5;       //反转
	
	
	motor_pid_caculate(&Motor_control.Motor_Left_pid);		//左电机pid计算
	motor_pid_caculate(&Motor_control.Motor_Right_pid);		//右电机pid计算
	
	motor_set();
	
	
	//更新编码器队列
	left_pulse_temp[4] 	= left_pulse_temp[3];
	left_pulse_temp[3] 	= left_pulse_temp[2];
	left_pulse_temp[2] 	= left_pulse_temp[1];
	left_pulse_temp[1] 	= left_pulse_temp[0];
	
	right_pulse_temp[4] = right_pulse_temp[3];
	right_pulse_temp[3] = right_pulse_temp[2];
	right_pulse_temp[2] = right_pulse_temp[1];
	right_pulse_temp[1] = right_pulse_temp[0];
//	Speed_Time = (uint16_t)(pit_time_get(pit1)*1000/bus_clk_khz);
//	pit_close(pit1);
	
	
	bling_bling_led2(20,500);
	bling_bling_led3(20,500);
	if(MOTOR_EN)
//		BB_LED_PLAY();
		BB_Play();
	bling_bling_led0_play();
	bling_bling_led1_play();
	bling_bling_led2_play();
	bling_bling_led3_play();

	//计时服务
	time_1ms++;
	if(!(time_1ms % 10))
	{time_10ms++;
		if(!(time_1ms % 20))
			time_20ms++;
			if(!(time_1ms % 50))
			{time_50ms++;
				if(!(time_1ms % 100))
				{
					time_100ms++;
					time_1ms = 0;
				}
		}
	}
}



//普通计时服务
void time_5ms_serve(void)
{
//	Car_Gather_Data_Key((uint8_t)50);		//Key采集
	
//	ANO_DT_Send_Senser(ad_avr_val[1],ad_avr_val[2],ad_avr_val[3],ad_avr_val[4],ad_avr_val[5],ad_avr_val[6],0,0,0);
//	ANO_DT_Send_Power(Power_V*100,0);
}

void time_20ms_serve(void)
{
//	ANO_rerror[0] - Servo.error[9]),Servo.output,Sensor.once_uni_ad[2]-Sensor.once_uni_ad[5],Sensor.once_uni_ad[2]+Sensor.once_uni_ad[5],sum_16_34,Servo.error[0],Sensor.once_uni_ad[7],(Motor_control.Motor_Left_pid.present_value[0]+Motor_control.Motor_Right_pid.present_value[0])>1,0);
//	ANO_DT_Data_Exchange_ToMe();		//匿名上位机对应

}

void time_10ms_serve(void)
{
	if(0 == MOTOR_EN)
	{
		while(gpio_get(D5)==0)
		{	
			OLED_CLS();
			while(gpio_get(D5)==0)
			{
				Car_Gather_Data_Key((uint8_t)50);
				OLED_P6x8Str(0,0,"Mid:       ");
				OLED_P6x8Num(30,0,ad_avr_val[3]);
				OLED_P6x8Str(0,1,"MidMax:    ");
				OLED_P6x8Num(45,1,Adc.ad_max_val[3]);
				OLED_P6x8Str(0,2,"MidAdc:    ");
				OLED_P6x8Num(45,2,Sensor.once_uni_ad[3]);
				
				OLED_P6x8Str(75,2,"L:    ");
				OLED_P6x8Num(90,2,Motor_control.Motor_Left_pid.present_value[0]);
				OLED_P6x8Str(75,3,"R:    ");
				OLED_P6x8Num(90,3,Motor_control.Motor_Right_pid.present_value[0]);
				
				
				if(Position)
					OLED_P6x8Str(0,4,"Right");
				else
					OLED_P6x8Str(0,4,"Left  ");
				if((Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc))
					find_middle_max_ad();
				if(ramp_in)
					OLED_P6x8Str(0,5,"ramp  ");
				else if(circle_in)
					OLED_P6x8Str(0,5,"circle   ");
				else
					OLED_P6x8Str(0,5,"no  ");
					
					OLED_P6x8Str(0,6,"M_D:   ");
					OLED_P6x8Num(26,6,mid_sensor_max_distence);
			}
		}
		while(gpio_get(I6)==0)
		{
			OLED_CLS();
			while(gpio_get(I6)==0)
			{
				OLED_P6x8Str(0,0,"err:       ");
				OLED_P6x8Num(45,0,Servo.error[0]);
				OLED_P6x8Str(0,1,"err24:       ");
				OLED_P6x8Num(45,1,error_24);
//				OLED_P6x8Str(0,1,"R_VAL:       ");
//				OLED_P6x8Num(45,1,Sensor.once_uni_ad[5]);
//				OLED_P6x8Str(0,2,"SUB:         ");
//				OLED_P6x8Num(45,2,Sensor.once_uni_ad[2]-Sensor.once_uni_ad[5]);
//				OLED_P6x8Str(0,3,"SUM:         ");
//				OLED_P6x8Num(45,3,Sensor.once_uni_ad[2]+Sensor.once_uni_ad[5]);
				OLED_P6x8Str(0,4,"SUM1346:         ");
				OLED_P6x8Num(60,4,sum_16_34);
			}
		}

		if((Key_Inquire_data(Key_Flag5_Read) == Key_bit_Acc)||(Key_Inquire_data(Key_Flag5_Read) == Key_bit_Drop)) 
		{
//			MOTOR_EN = 1;				//开电机
			start_car_flag = 1;
	//		gpio_set(H4,1);				//开电机
			stop_car = 0;
		}
		
		if((Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc))
		{
			OLED_CLS();
//			flash_read_data();
			if(flash_write_data())
				OLED_P6x8Str(0,0,"save fail");
			else
				OLED_P6x8Str(0,0,"save ok");
			systick_delay_ms(500);
		}

		Car_Gather_Data_Key((uint8_t)20);		//Key采集     ps:改时间
		OLED_ConfigParameter();
		Cache_Update_OLED();
	}
	else
	{
		OLED_CLS();
	}
}

void time_50ms_serve(void)
{
	
}

void time_100ms_serve(void)
{
	
}

void IRQ_IRQHandler(void)
{
    CLEAR_IRQ_FLAG;
	if(start_check_stop)
		stop_car = 1;
//	}
//	else
//		gpio_set(H6,0);
}

