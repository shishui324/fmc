#include "Car_Key.h"

/********** Key���� *********/
uint8_t 	Key_data[6]		= 	{0,0,0,0,0,0};		//״̬
uint16_t 	Key_time[6]		= 	{0,0,0,0,0,0};		//Time
uint8_t 	Key_GD[6] 		= 	{0,0,0,0,0,0};		//ʵʱ��ƽ


/****************************************************************
**	Key��ʼ��
** A,B,C,D--GPIOA,PUE0  ����Ҫ��
** E,F,G,H--GPIOB,PUE1 ���32
** I      --GPIOC,PUE2  ���64
**
****************************************************************/
void Key_Message_Init(void)
{
	///////���뿪��
//	GPIOA->PIDR &= ~((uint32)1<<D5);	//ȡ����������
//    GPIOA->PDDR &= ~((uint32)1<<D5);	//���˿�����Ϊ����					Key_Flag1
//    PORT_PUE0 |= (uint32)1<<D5;		//����ʹ��

//    GPIOC->PIDR &= ~((uint32)1<<(I6-64));	//ȡ����������
//    GPIOC->PDDR &= ~((uint32)1<<(I6-64));	//���˿�����Ϊ�������� 	Key_Flag2
//    PORT_PUE2 |= (uint32)1<<(I6-64);		//����ʹ��

//    GPIOC->PIDR &= ~((uint32)1<<(I5-64));	//ȡ����������
//    GPIOC->PDDR &= ~((uint32)1<<(I5-64));	//���˿�����Ϊ����					Key_Flag3
//    PORT_PUE2 |= (uint32)1<<(I5-64);		//����ʹ��

//    GPIOA->PIDR &= ~((uint32)1<<C1);	//ȡ����������
//    GPIOA->PDDR &= ~((uint32)1<<C1);	//���˿�����Ϊ�������� 	`Key_Flag4
//    PORT_PUE0 |= (uint32)1<<C1;		//����ʹ��
//	/////////��������
//	
//	  GPIOA->PIDR &= ~((uint32)1<<(D6));	//ȡ����������
//    GPIOA->PDDR &= ~((uint32)1<<(D6));	//���˿�����Ϊ�������� 	`Key_Flag5
//    PORT_PUE0 |= (uint32)1<<(D6);		//����ʹ��
//		
//	

//		//////���򿪹�
//    GPIOA->PIDR &= ~((uint32)1<<D4);	//ȡ����������
//    GPIOA->PDDR &= ~((uint32)1<<D4);	//���˿�����Ϊ����						Key_Front
//    PORT_PUE0 |= (uint32)1<<D4;		//����ʹ��
//			

//    GPIOA->PIDR &= ~((uint32)1<<D2);	//ȡ����������
//    GPIOA->PDDR &= ~((uint32)1<<D2);	//���˿�����Ϊ�������� 		Key_Back
//    PORT_PUE0 |= (uint32)1<<D2;		//����ʹ��

//    GPIOB->PIDR &= ~((uint32)1<<(G6-32));	//ȡ����������
//    GPIOB->PDDR &= ~((uint32)1<<(G6-32));	//���˿�����Ϊ����						Key5_Left
//    PORT_PUE1 |= (uint32)1<<(G6-32);		//����ʹ��

//    GPIOA->PIDR &= ~((uint32)1<<D3);	//ȡ����������
//    GPIOA->PDDR &= ~((uint32)1<<D3);	//���˿�����Ϊ�������� 		Key5_Right
//    PORT_PUE0 |= (uint32)1<<D3;		//����ʹ��

//    GPIOB->PIDR &= ~((uint32)1<<(G5-32));	//ȡ����������
//    GPIOB->PDDR &= ~((uint32)1<<(G5-32));	//���˿�����Ϊ����						Key5_Middle
//    PORT_PUE1 |= (uint32)1<<(G5-32);		//����ʹ��

	gpio_init(I6,GPI,0);	//K1
	port_pull(I6);
	
	gpio_init(D5,GPI,0);	//K2
	port_pull(D5);
	
	gpio_init(D6,GPI,0);	//K3
	port_pull(D6);
	
	gpio_init(D7,GPI,0);	//K4
	port_pull(D7);
	
	gpio_init(E1,GPI,0);	//K5
	port_pull(E1);
	
	gpio_init(C3,GPI,0);	//up
	port_pull(C3);
	
	gpio_init(E6,GPI,0);	//down
	port_pull(E6);
	
	gpio_init(E5,GPI,0);	//middle
	port_pull(E5);
	
	gpio_init(B5,GPI,0);	//left
	port_pull(B5);
	
	gpio_init(C2,GPI,0);	//right
	port_pull(C2);
	

	Car_Gather_Data_Key(1);

}




/****************************************************************
**************************	Key�ɼ�  ****************************
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
        case Key_bit_High://Key״̬-����
            if(data_temp==0) {
                Key_data[acc] = Key_bit_Low_;
                Key_time[acc] = 0;
            }
            break;
        case Key_bit_Low_://Key״̬-����ȥȥ�����
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
        case Key_bit_low://Key״̬-����
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
				default :	break;
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




