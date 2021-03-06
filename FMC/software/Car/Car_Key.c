#include "Car_Key.h"

/********** Key过程 *********/
uint8_t 	Key_data[6]		= 	{0,0,0,0,0,0};		//状态
uint16_t 	Key_time[6]		= 	{0,0,0,0,0,0};		//Time
uint8_t 	Key_GD[6] 		= 	{0,0,0,0,0,0};		//实时电平


/****************************************************************
**	Key初始化
** A,B,C,D--GPIOA,PUE0  不需要减
** E,F,G,H--GPIOB,PUE1 需减32
** I      --GPIOC,PUE2  需减64
**
****************************************************************/
void Key_Message_Init(void)
{
	
	///////拨码开关
		GPIOA->PIDR &= ~((uint32)1<<D5);	//取消禁用输入
    GPIOA->PDDR &= ~((uint32)1<<D5);	//将端口设置为输入					Key_Flag1
    PORT_PUE0 |= (uint32)1<<D5;		//上拉使能

    GPIOC->PIDR &= ~((uint32)1<<(I6-64));	//取消禁用输入
    GPIOC->PDDR &= ~((uint32)1<<(I6-64));	//将端口设置为输入或输出 	Key_Flag2
    PORT_PUE2 |= (uint32)1<<(I6-64);		//上拉使能

    GPIOC->PIDR &= ~((uint32)1<<(I5-64));	//取消禁用输入
    GPIOC->PDDR &= ~((uint32)1<<(I5-64));	//将端口设置为输入					Key_Flag3
    PORT_PUE2 |= (uint32)1<<(I5-64);		//上拉使能

    GPIOA->PIDR &= ~((uint32)1<<C1);	//取消禁用输入
    GPIOA->PDDR &= ~((uint32)1<<C1);	//将端口设置为输入或输出 	`Key_Flag4
    PORT_PUE0 |= (uint32)1<<C1;		//上拉使能
	/////////独立按键
	
	  GPIOA->PIDR &= ~((uint32)1<<(D6));	//取消禁用输入
    GPIOA->PDDR &= ~((uint32)1<<(D6));	//将端口设置为输入或输出 	`Key_Flag4
    PORT_PUE0 |= (uint32)1<<(D6);		//上拉使能
		
	

		//////五向开关
    GPIOA->PIDR &= ~((uint32)1<<D4);	//取消禁用输入
    GPIOA->PDDR &= ~((uint32)1<<D4);	//将端口设置为输入						Key_Front
    PORT_PUE0 |= (uint32)1<<D4;		//上拉使能
			

    GPIOA->PIDR &= ~((uint32)1<<D2);	//取消禁用输入
    GPIOA->PDDR &= ~((uint32)1<<D2);	//将端口设置为输入或输出 		Key_Back
    PORT_PUE0 |= (uint32)1<<D2;		//上拉使能

    GPIOB->PIDR &= ~((uint32)1<<(G6-32));	//取消禁用输入
    GPIOB->PDDR &= ~((uint32)1<<(G6-32));	//将端口设置为输入						Key5_Left
    PORT_PUE1 |= (uint32)1<<(G6-32);		//上拉使能

    GPIOA->PIDR &= ~((uint32)1<<D3);	//取消禁用输入
    GPIOA->PDDR &= ~((uint32)1<<D3);	//将端口设置为输入或输出 		Key5_Right
    PORT_PUE0 |= (uint32)1<<D3;		//上拉使能

    GPIOB->PIDR &= ~((uint32)1<<(G5-32));	//取消禁用输入
    GPIOB->PDDR &= ~((uint32)1<<(G5-32));	//将端口设置为输入						Key5_Middle
    PORT_PUE1 |= (uint32)1<<(G5-32);		//上拉使能
	

	Car_Gather_Data_Key(1);

}




/****************************************************************
**************************	Key采集  ****************************
****************************************************************/
void Car_Gather_Data_Key(uint8_t time_ms)
{
    uint8_t data_temp = 0;
    uint8_t acc = 0;

    while(acc<6)
    {
        switch(acc)
        {
        case Key_Front_Read:
            Key_GD[Key_Front_Read] = data_temp = Key_Front;
            break;
        case Key_Back_Read:
            Key_GD[Key_Back_Read] = data_temp = Key_Back;
            break;
        case Key_Left_Read:
            Key_GD[Key_Left_Read] = data_temp = Key_Left;
            break;
        case Key_Right_Read:
            Key_GD[Key_Right_Read] = data_temp = Key_Right;
            break;
        case Key_Middle_Read:
					Key_GD[Key_Middle_Read] = data_temp = Key_Middle;
            break;
        case Key_Flag5_Read:
					Key_GD[Key_Flag5_Read] = data_temp = Key_Flag5;
            break;
				default :	break;
        }

        switch(Key_data[acc]) 
					{
        case Key_bit_High://Key状态-弹起
            if(data_temp==0) {
                Key_data[acc] = Key_bit_Low_;
                Key_time[acc] = 0;
            }
            break;
        case Key_bit_Low_://Key状态-按下去去抖检测
            if(data_temp==0) {
                Key_time[acc] += time_ms;
                if(Key_time[acc] > 10)
                {
                    Key_data[acc] = Key_bit_Drop;
                    Key_time[acc] = 0;
                }
            }
            else {
                Key_data[acc] = Key_bit_High;
                Key_time[acc] = 0;
            }
            break;
        case Key_bit_low://Key状态-按下
            if(data_temp==0) {
                Key_time[acc] += time_ms;
                if(Key_time[acc] > 100)
                {
                    Key_data[acc] = Key_bit_Acc;
                    Key_time[acc] = 0;
                }
            }
            else {
                Key_data[acc] = Key_bit_Rise;
                Key_time[acc] = 0;
            }
            break;
        case Key_bit_Acc://Key长按状态
            if(data_temp==0) {
                Key_data[acc] = Key_bit_Acc;
            }
            else {
                Key_data[acc] = Key_bit_ACCRise;
            }
            break;
        case Key_bit_Drop://Key按下沿
            if(data_temp==0) {
                Key_data[acc] = Key_bit_low;
                Key_time[acc] = 0;
            }
            else {
                Key_data[acc] = Key_bit_High;
            }
            break;
        case Key_bit_Rise://Key弹起沿
            Key_data[acc] = Key_bit_High;
            break;
        case Key_bit_ACCRise://长按弹起沿
            Key_data[acc] = Key_bit_High;
            break;
				default :	break;
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




