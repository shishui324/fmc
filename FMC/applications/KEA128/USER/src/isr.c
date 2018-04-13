#include "isr.h"

extern uint32_t time_1ms;
extern uint32_t time_5ms;
extern uint32_t time_10ms;
extern uint32_t time_20ms;
extern uint32_t time_500ms;
// uint32_t time_50ms;
//extern uint32_t time_100ms;
//extern uint32_t time_200ms;
//extern uint32_t time_500ms;
//extern uint32_t time_1s;
//extern uint32_t time_2s;
//extern uint32_t time_5s;
//extern uint32_t time_10s;
extern uint16_t encode_time;
extern uint16_t sensor_time;
extern uint16_t control_time;



extern void get_adc_int_value(void);
extern void control(void);
void PIT_CH0_IRQHandler(void)
{
	
	
  PIT_FlAG_CLR(pit0);                     //清pit0标志位
	
		
	Bell_Play();
	pit_time_start(pit1);
	get_num();	 //获取编码器值
	encode_time = (uint16_t)(pit_time_get(pit1)*1000/(bus_clk_khz));
	pit_close(pit1);
	
	
	pit_time_start(pit1);
	get_adc_int_value();	//滤波后AD值
	deal_sensor(&Sensor);
	sensor_time = (uint16_t)(pit_time_get(pit1)*1000/(bus_clk_khz));
	pit_close(pit1);
	
	pit_time_start(pit1);
	control();
	control_time = (uint16_t)(pit_time_get(pit1)*1000/(bus_clk_khz));
	pit_close(pit1);
	
	
	
	
//	motor_pid_caculate(&Motor_control.Motor_Left_pid);
//	L_out_value = Motor_control.Motor_Left_pid.output;
//	motor_set();

		time_5ms++;
	
			if(!(time_5ms % 2))
			{
				time_10ms++;
			}
			if(!(time_5ms % 4))
			{
				time_20ms++;
			time_5ms = 0;}
	
/*
		if(!(time_1ms % 50))
		{time_50ms++;
			if(!(time_1ms % 100))
			{time_100ms++;
				if(!(time_1ms % 200))
					time_200ms++;

				if(!(time_1ms % 500))
				{time_500ms++;
					if(!(time_1ms % 1000))
					{time_1s++;
						if(!(time_1ms % 2000))
							time_2s++;

						if(!(time_1ms % 5000))
						{time_5s++;
							if(!(time_1ms % 10000))
							{time_10s++;
								time_1ms = 0;
							}
						}
					}
				}
			}
		}
	}
} */
//		pit0RunTime=pit_time_get(pit1);
}


void PIT_CH1_IRQHandler(void)
{
	
	 PIT_FlAG_CLR(pit1); 
	
}
void IRQ_IRQHandler(void)
{
    CLEAR_IRQ_FLAG;
    
}


void KBI0_IRQHandler(void)
{
    CLEAN_KBI0_FLAG;
    
}





/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器 通道0得中断
void PIT_CH0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位

FTMRE_IRQHandler      
PMC_IRQHandler        
IRQ_IRQHandler        
I2C0_IRQHandler       
I2C1_IRQHandler       
SPI0_IRQHandler       
SPI1_IRQHandler       
UART0_IRQHandler 
UART1_IRQHandler 
UART2_IRQHandler 
ADC0_IRQHandler       
ACMP0_IRQHandler      
FTM0_IRQHandler       
FTM1_IRQHandler       
FTM2_IRQHandler       
RTC_IRQHandler        
ACMP1_IRQHandler      
PIT_CH0_IRQHandler    
PIT_CH1_IRQHandler    
KBI0_IRQHandler       
KBI1_IRQHandler       
Reserved26_IRQHandler 
ICS_IRQHandler        
WDG_IRQHandler        
PWT_IRQHandler        
MSCAN_Rx_IRQHandler   
MSCAN_Tx_IRQHandler   
*/



