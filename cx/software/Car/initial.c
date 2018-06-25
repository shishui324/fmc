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
	get_clk();              	//��ȡʱ��Ƶ�� ����ִ��
	
	DisableInterrupts ;         //��ֹ�ж�
    OLED_Init();				//OLED��ʼ��
	
	FLASH_Init();				//FLASH��ʼ��
//FLASH_EraseSector(sector);  
	ad_init(); 					//adc��ʼ��
	motor_init();				//�����ʼ��
	pulse_acc_init();			//������������ʼ��
	Key_Message_Init(); 		//������ʼ��
    uart_init(DEBUG_PORT,DEBUG_BAUD);	//����1��ʼ��
    PIT0_Init();
	
	gpio_init(I0,GPI,1);		//��ż��IO�ڳ�ʼ��
	port_pull(I0);				//��ż��IO������
	
	FGPIOB_PDDR |= 1<<30;		//������
	FGPIOB_PDOR |= 1<<30;
	
//	gpio_init(H5,GPO,0);		//LED1
	gpio_init(C7,GPO,1);		//LED1
	gpio_init(C6,GPO,1);		//LED2
	gpio_init(I3,GPO,1);		//LED3
	gpio_init(I2,GPO,1);		//LED4
	
	MPU_IIC_Init();				//IIC��ʼ��
	MPU_Init();					//MPU6050��ʼ��
	irq_init();					//�ⲿ�жϳ�ʼ��
	set_irq_priority(IRQ_IRQn,1);		
}


//PIT0��ʼ��
void PIT0_Init(void)
{
  pit_init_ms(pit0,1); //1ms��ʱ�ж�
  set_irq_priority(PIT_CH0_IRQn,0); //�����жϷ��������ж�������
}

//��ֵ������ʼ��,���flash����������������
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
//  //���ϵ��
////  Sensor.PARAM_A = 26.0;
////  Sensor.PARAM_B = 2.0;
////  Sensor.PARAM_CROSS = 12.0;
//  
//  //������ֵ
//  LoseLine.lose_line_val = 100;
//  
//   //ʮ��
//  CrossRoad.cross_fazhi_in = 3000;
//  CrossRoad.cross_fazhi_out = 500;
//  CrossRoad.cross_fazhi_out_in = 350;
//  
//  //���
//  Servo.distance_err_d_max_val = 5.0;
//  Servo.distance_err_max_val = 12.0;
//  Servo.kd = 38.0;
//  Servo.kp = 60.0;
//  
//  Servo.max_dis_err = 0;
//  Servo.max_dis_err_d = 0;
////  Servo.mode_select = 0;
//  
//  //���
//  Motor.kvff =20.00;  
//  Motor.kaff = 10.00;
//  Motor.kd = 10.00;
//  Motor.ki = 15.00;
//  Motor.kp = 60.00;

//  
//  //�ٶ�
//  Speed.cross_speed_val = 20;
//  Speed.ramp1_speed_val = 12;
//  Speed.ramp2_speed_val = 15;
//  Speed.wandao_speed_val = 16;
//  Speed.zhidao_speed_val = 25;

//  //��ʱͣ��
//  stop_time_value = 60;
// 
//    
//    Speed.speed_high_mode =0;
//    Speed.high_speed_dis = 300;//1m����
    
}
