
#include "Car_Key.h"

/********** Key过程 *********/
uint8_t Key_data[9]={0,0,0,0,0,0,0,0,0};		//状态
uint16_t Key_time[9]={0,0,0,0,0,0,0,0,0};		//Time
uint8_t Key_GD[9] = {0,0,0,0,0,0,0,0,0};		//实时电平


/****************************************************************
**	Key初始化
**
**	PD_9~16 / PE_0
****************************************************************/
void Key_Message_Init(void)
{
	GPIOA->PIDR &= ~((uint32)1<<C2);	//PTC2取消禁用输入
	GPIOA->PDDR &= ~((uint32)1<<C2);	//将端口设置为输入或输出 0:输入 1:输出
	PORT_PUE0 |= (uint32)1<<C2;		//PTC2上拉使能
	
	GPIOA->PIDR &= ~((uint32)1<<B5);	//PTB5取消禁用输入
	GPIOA->PDDR &= ~((uint32)1<<B5);	//将端口设置为输入或输出 0:输入 1:输出
	PORT_PUE0 |= (uint32)1<<B5;		//PTC2上拉使能
	
//	Car_Gather_Data_Key(1);
}


/****************************************************************
**************************	Key采集  ****************************
****************************************************************/
void Car_Gather_Data_Key(uint8_t time_ms)
{
	uint8_t data_temp = 0;
	uint8_t acc = 0;
	
	while(acc < 2) 
	{
		switch(acc){
//			case Key_Flag1_Read:	Key_GD[Key_Flag1_Read] = data_temp = Key_Flag1;break;
//			case Key_Flag2_Read:	Key_GD[Key_Flag2_Read] = data_temp = Key_Flag2;break;
//			case Key_Flag3_Read:	Key_GD[Key_Flag3_Read] = data_temp = Key_Flag3;break;
//			case Key_Flag4_Read:	Key_GD[Key_Flag4_Read] = data_temp = Key_Flag4;break;
			case Key_Up_Read:		Key_GD[Key_Up_Read] = data_temp = Key5_Up;break;
			case Key_Down_Read:		Key_GD[Key_Down_Read] = data_temp = Key5_Down;break;
//			case Key_Left_Read:		Key_GD[Key_Left_Read] = data_temp = Key5_Left;break;
//			case Key_Right_Read:	Key_GD[Key_Right_Read] = data_temp = Key5_Right;break;
//			case Key_Middle_Read:	Key_GD[Key_Middle_Read] = data_temp = Key5_Middle;break;
		}

		switch(Key_data[acc]){
			case Key_bit_High://Key状态-弹起
								if(data_temp==0){
									Key_data[acc] = Key_bit_Low_;
									Key_time[acc] = 0;
								}
								break;
			case Key_bit_Low_://Key状态-按下去去抖检测
												if(data_temp==0){
													Key_time[acc] += time_ms;
													if(Key_time[acc] > 20)
													{
														Key_data[acc] = Key_bit_Drop;
														Key_time[acc] = 0;
													}
												}
												else{
													Key_data[acc] = Key_bit_High;
													Key_time[acc] = 0;
												}
												break;
			case Key_bit_low://Key状态-按下
												if(data_temp==0){
													Key_time[acc] += time_ms;
													if(Key_time[acc] > 500)
													{
														Key_data[acc] = Key_bit_Acc;
														Key_time[acc] = 0;
													}
												}
												else{
													Key_data[acc] = Key_bit_Rise;
													Key_time[acc] = 0;
												}
												break;
			case Key_bit_Acc://Key长按状态
												if(data_temp==0){
													Key_data[acc] = Key_bit_Acc;
												}
												else{
													Key_data[acc] = Key_bit_ACCRise;
												}
												break;
			case Key_bit_Drop://Key按下沿
												if(data_temp==0){
													Key_data[acc] = Key_bit_low;
													Key_time[acc] = 0;
												}
												else{
													Key_data[acc] = Key_bit_High;
												}
												break;
			case Key_bit_Rise://Key弹起沿
												Key_data[acc] = Key_bit_High;
												break;
			case Key_bit_ACCRise://长按弹起沿
												Key_data[acc] = Key_bit_High;
												break;
		}
		acc++;
	}
}

/****************************************************************
**************************	Key读取Data
****************************************************************/
uint8_t Key_Inquire_data(uint8_t Key_name)
{
	return Key_data[Key_name];
}

/****************************************************************
**************************	Key读取GD
****************************************************************/
uint8_t Key_Inquire_GD(uint8_t Key_name)
{
	return Key_GD[Key_name];
}




