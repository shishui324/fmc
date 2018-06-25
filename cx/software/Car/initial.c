#include "initial.h"
#include "headfile.h"
#include "myflash.h"


void view_start_up(void)
{
	Draw_LOGO();
	while(1)
	{
		Car_Gather_Data_Key((uint8_t)50);
		if((Key_Inquire_data(Key_Middle_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc))
		{
			OLED_CLS();
			OLED_P8x16Str(6,3,"Reading Flash.."); //flash
			flash_read_data();
			systick_delay_ms(1000);
			OLED_CLS();
			break;
		}
		if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
		{
			OLED_CLS();
			OLED_P8x16Str(6,3,"Load Default...");
			systick_delay_ms(1000);
			OLED_CLS();
			break;
		}
		if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		{
			OLED_CLS();
			flash_write_data();
			OLED_P8x16Str(6,3,"Write flash..");
			systick_delay_ms(1000);
			OLED_CLS();
			break;
		}
	}
}

void fmc_all_init(void)
{ 
	get_clk();              	//获取时钟频率 必须执行
	
	DisableInterrupts ;         //禁止中断
    OLED_Init();				//OLED初始化
	
	FLASH_Init();				//FLASH初始化
//FLASH_EraseSector(sector);  
	ad_init(); 					//adc初始化
	motor_init();				//电机初始化
	pulse_acc_init();			//编码器计数初始化
	Key_Message_Init(); 		//按键初始化
    uart_init(DEBUG_PORT,DEBUG_BAUD);	//串口1初始化
    PIT0_Init();
	
	gpio_init(I0,GPI,1);		//电磁检测IO口初始化
	port_pull(I0);				//电磁检测IO口上拉
	
	FGPIOB_PDDR |= 1<<30;		//蜂鸣器
	FGPIOB_PDOR |= 1<<30;
	
//	gpio_init(H5,GPO,0);		//LED1
	gpio_init(C7,GPO,1);		//LED1
	gpio_init(C6,GPO,1);		//LED2
	gpio_init(I3,GPO,1);		//LED3
	gpio_init(I2,GPO,1);		//LED4
	
	MPU_IIC_Init();				//IIC初始化
	MPU_Init();					//MPU6050初始化
	irq_init();					//外部中断初始化
	set_irq_priority(IRQ_IRQn,1);		
}


//PIT0初始化
void PIT0_Init(void)
{
  pit_init_ms(pit0,1); //1ms定时中断
  set_irq_priority(PIT_CH0_IRQn,0); //设置中断服务函数到中断向量表
}

//阈值参数初始化,如果flash数据有问题就用这个
void someParaInit(void)
{
	uint8 i;
	Speed.cross_speed_val = 14 ;                 //20  6
	Speed.wandao_speed_val = 12;                //16  4
	Speed.zhidao_speed_val = 17;                //25  8  18
	
	Servo.kp = 1.220f;//1.220f;//1.620f;//7.20f;	//5.2 4.2    1.220f
	Servo.kd = 2.8f;//2.5f;   // 1.0  0.2	2.0
	Servo.max_dis_err = 0.0;
	Servo.distance_err_max_val = 12;	//12.0;
	Servo.max_dis_err_d = 0.0;
	Servo.distance_err_d_max_val = 3.0;	//5
	
	
	Motor_control.Motor_Left_pid.kvff = 1.0;
	Motor_control.Motor_Left_pid.kaff =  1.0;
	Motor_control.Motor_Left_pid.kd = 10.0;  //2
	Motor_control.Motor_Left_pid.ki = 4.05;  // 5 3
	Motor_control.Motor_Left_pid.kp = 23.5f; //12 //8  6 40
	
	Motor_control.Motor_Right_pid.kvff = 1.0;
	Motor_control.Motor_Right_pid.kaff = 1.0;
	Motor_control.Motor_Right_pid.kd = 10.0;   //3
	Motor_control.Motor_Right_pid.ki = 4.05;		//5 3
	Motor_control.Motor_Right_pid.kp = 21.4f; //12 1 1 3 2 9 12
	for(i=1;i<=6;i++)
		Adc.ad_max_val[i]=4095;
		
		Adc.ad_max_val[3]=1460;
//	flash_write_data();
//	flash_read_data();
//  //拟合系数
////  Sensor.PARAM_A = 26.0;
////  Sensor.PARAM_B = 2.0;
////  Sensor.PARAM_CROSS = 12.0;
//  
//  //丢线阈值
//  LoseLine.lose_line_val = 100;
//  
//   //十字
//  CrossRoad.cross_fazhi_in = 3000;
//  CrossRoad.cross_fazhi_out = 500;
//  CrossRoad.cross_fazhi_out_in = 350;
//  
//  //舵机
//  Servo.distance_err_d_max_val = 5.0;
//  Servo.distance_err_max_val = 12.0;
//  Servo.kd = 38.0;
//  Servo.kp = 60.0;
//  
//  Servo.max_dis_err = 0;
//  Servo.max_dis_err_d = 0;
////  Servo.mode_select = 0;
//  
//  //电机
//  Motor.kvff =20.00;  
//  Motor.kaff = 10.00;
//  Motor.kd = 10.00;
//  Motor.ki = 15.00;
//  Motor.kp = 60.00;

//  
//  //速度
//  Speed.cross_speed_val = 20;
//  Speed.ramp1_speed_val = 12;
//  Speed.ramp2_speed_val = 15;
//  Speed.wandao_speed_val = 16;
//  Speed.zhidao_speed_val = 25;

//  //定时停车
//  stop_time_value = 60;
// 
//    
//    Speed.speed_high_mode =0;
//    Speed.high_speed_dis = 300;//1m高速
    
}
