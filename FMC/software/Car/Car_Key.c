
#include "Car_Key.h"

/********** Key���� *********/
uint8_t Key_data[9]={0,0,0,0,0,0,0,0,0};		//״̬
uint16_t Key_time[9]={0,0,0,0,0,0,0,0,0};		//Time
uint8_t Key_GD[9] = {0,0,0,0,0,0,0,0,0};		//ʵʱ��ƽ


/****************************************************************
**	Key��ʼ��
**
**	PD_9~16 / PE_0
****************************************************************/
void Key_Message_Init(void)
{
	GPIOA->PIDR &= ~((uint32)1<<C2);	//PTC2ȡ����������
	GPIOA->PDDR &= ~((uint32)1<<C2);	//���˿�����Ϊ�������� 0:���� 1:���
	PORT_PUE0 |= (uint32)1<<C2;		//PTC2����ʹ��
	
	GPIOA->PIDR &= ~((uint32)1<<B5);	//PTB5ȡ����������
	GPIOA->PDDR &= ~((uint32)1<<B5);	//���˿�����Ϊ�������� 0:���� 1:���
	PORT_PUE0 |= (uint32)1<<B5;		//PTC2����ʹ��
	
//	Car_Gather_Data_Key(1);
}


/****************************************************************
**************************	Key�ɼ�  ****************************
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
			case Key_bit_High://Key״̬-����
								if(data_temp==0){
									Key_data[acc] = Key_bit_Low_;
									Key_time[acc] = 0;
								}
								break;
			case Key_bit_Low_://Key״̬-����ȥȥ�����
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
			case Key_bit_low://Key״̬-����
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
			case Key_bit_Acc://Key����״̬
												if(data_temp==0){
													Key_data[acc] = Key_bit_Acc;
												}
												else{
													Key_data[acc] = Key_bit_ACCRise;
												}
												break;
			case Key_bit_Drop://Key������
												if(data_temp==0){
													Key_data[acc] = Key_bit_low;
													Key_time[acc] = 0;
												}
												else{
													Key_data[acc] = Key_bit_High;
												}
												break;
			case Key_bit_Rise://Key������
												Key_data[acc] = Key_bit_High;
												break;
			case Key_bit_ACCRise://����������
												Key_data[acc] = Key_bit_High;
												break;
		}
		acc++;
	}
}

/****************************************************************
**************************	Key��ȡData
****************************************************************/
uint8_t Key_Inquire_data(uint8_t Key_name)
{
	return Key_data[Key_name];
}

/****************************************************************
**************************	Key��ȡGD
****************************************************************/
uint8_t Key_Inquire_GD(uint8_t Key_name)
{
	return Key_GD[Key_name];
}




