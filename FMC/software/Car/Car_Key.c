#include "Car_Key.h"

/********** Key���� *********/
uint8_t Key_data[9]= {0,0,0,0,0,0,0,0,0};		//״̬
uint16_t Key_time[9]= {0,0,0,0,0,0,0,0,0};		//Time
uint8_t Key_GD[9] = {0,0,0,0,0,0,0,0,0};		//ʵʱ��ƽ


/****************************************************************
**	Key��ʼ��
**
**
****************************************************************/
void Key_Message_Init(void)
{
    GPIOA->PIDR &= ~((uint32)1<<D5);	//ȡ����������
    GPIOA->PDDR &= ~((uint32)1<<D5);	//���˿�����Ϊ����					Key_Flag1
    PORT_PUE0 |= (uint32)1<<D5;		//����ʹ��

    GPIOC->PIDR &= ~((uint32)1<<(I6-64));	//ȡ����������
    GPIOC->PDDR &= ~((uint32)1<<(I6-64));	//���˿�����Ϊ�������� 	Key_Flag2
    PORT_PUE2 |= (uint32)1<<(I6-64);		//����ʹ��

    GPIOC->PIDR &= ~((uint32)1<<(I5-64));	//ȡ����������
    GPIOC->PDDR &= ~((uint32)1<<(I5-64));	//���˿�����Ϊ����					Key_Flag3
    PORT_PUE2 |= (uint32)1<<(I5-64);		//����ʹ��

    GPIOA->PIDR &= ~((uint32)1<<C1);	//ȡ����������
    GPIOA->PDDR &= ~((uint32)1<<C1);	//���˿�����Ϊ�������� 	`Key_Flag4
    PORT_PUE0 |= (uint32)1<<C1;		//����ʹ��

    GPIOA->PIDR &= ~((uint32)1<<D4);	//ȡ����������
    GPIOA->PDDR &= ~((uint32)1<<D4);	//���˿�����Ϊ����						Key5_Up
    PORT_PUE0 |= (uint32)1<<D4;		//����ʹ��

    GPIOA->PIDR &= ~((uint32)1<<D2);	//ȡ����������
    GPIOA->PDDR &= ~((uint32)1<<D2);	//���˿�����Ϊ�������� 		Key5_Down
    PORT_PUE0 |= (uint32)1<<D2;		//����ʹ��

    GPIOB->PIDR &= ~((uint32)1<<(G6-32));	//ȡ����������
    GPIOB->PDDR &= ~((uint32)1<<(G6-32));	//���˿�����Ϊ����						Key5_Left
    PORT_PUE1 |= (uint32)1<<(G6-32);		//����ʹ��

    GPIOA->PIDR &= ~((uint32)1<<D3);	//ȡ����������
    GPIOA->PDDR &= ~((uint32)1<<D3);	//���˿�����Ϊ�������� 		Key5_Right
    PORT_PUE0 |= (uint32)1<<D3;		//����ʹ��

    GPIOB->PIDR &= ~((uint32)1<<(G5-32));	//ȡ����������
    GPIOB->PDDR &= ~((uint32)1<<(G5-32));	//���˿�����Ϊ����						Key5_Middle
    PORT_PUE1 |= (uint32)1<<(G5-32);		//����ʹ��

//		gpio_init(D5,GPO,1);
//		port_pull(D5);
//		
//		gpio_init(I6,GPO,1);
//		port_pull(I6);
//		
//		gpio_init(I5,GPO,1);
//		port_pull(I5);
//		
//		gpio_init(C1,GPO,1);
//		port_pull(C1);
//	
//		gpio_init(D4,GPO,1);
//		port_pull(D4);
//		
//		gpio_init(D3,GPO,1);
//		port_pull(D3);
//		
//		gpio_init(D2,GPO,1);
//		port_pull(D2);
//		
//		gpio_init(G6,GPO,1);
//		port_pull(G6);
//				
//		gpio_init(G5,GPO,1);
//		port_pull(G5);

//	Car_Gather_Data_Key(1);
}


/****************************************************************
**************************	Key�ɼ�  ****************************
****************************************************************/
void Car_Gather_Data_Key(uint8_t time_ms)
{
    uint8_t data_temp = 0;
    uint8_t acc = 0;

    while(acc++<9)
    {
        switch(acc)
        {
        case Key_Flag1_Read:
            Key_GD[Key_Flag1_Read] = data_temp = Key_Flag1;
            break;
        case Key_Flag2_Read:
            Key_GD[Key_Flag2_Read] = data_temp = Key_Flag2;
            break;
        case Key_Flag3_Read:
            Key_GD[Key_Flag3_Read] = data_temp = Key_Flag3;
            break;
        case Key_Flag4_Read:
            Key_GD[Key_Flag4_Read] = data_temp = Key_Flag4;
            break;
        case Key_Up_Read:
            Key_GD[Key_Up_Read] = data_temp = Key5_Up;
            break;
        case Key_Down_Read:
            Key_GD[Key_Down_Read] = data_temp = Key5_Down;
            break;
        case Key_Left_Read:
            Key_GD[Key_Left_Read] = data_temp = Key5_Left;
            break;
        case Key_Right_Read:
            Key_GD[Key_Right_Read] = data_temp = Key5_Right;
            break;
        case Key_Middle_Read:
            Key_GD[Key_Middle_Read] = data_temp = Key5_Middle;
            break;
        }

        switch(Key_data[acc]) {
        case Key_bit_High://Key״̬-����
            if(data_temp==0) {
                Key_data[acc] = Key_bit_Low_;
                Key_time[acc] = 0;
            }
            break;
        case Key_bit_Low_://Key״̬-����ȥȥ�����
            if(data_temp==0) {
                Key_time[acc] += time_ms;
                if(Key_time[acc] > 20)
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
        case Key_bit_low://Key״̬-����
            if(data_temp==0) {
                Key_time[acc] += time_ms;
                if(Key_time[acc] > 500)
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
        case Key_bit_Acc://Key����״̬
            if(data_temp==0) {
                Key_data[acc] = Key_bit_Acc;
            }
            else {
                Key_data[acc] = Key_bit_ACCRise;
            }
            break;
        case Key_bit_Drop://Key������
            if(data_temp==0) {
                Key_data[acc] = Key_bit_low;
                Key_time[acc] = 0;
            }
            else {
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

