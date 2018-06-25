#if 0

#include "UI.h"

#include "OLED.h"

//Car
#include "Car.h"
#include "Car_Key.h"
#include "Car_ADC.h"
#include "Car_Steering.h"
#include "Car_Motor.h"
#include "Car_Pulse.h"			//LED & Hoare
#include "Car_Ultrasonic.h"		//超声波
#include "Car_Stockpile.h"		//存储
#include "Car_Pulse.h"

//Modules
#include "Direction_Controller.h"
#include "Speed_Controller.h"
#include "Transmission.h"
#include "Dual_System.h"

//Math
#include "matrix.h"
#include "Me_Math.h"

//基本输入输出
#include <string.h>
#include <stdio.h>


void UI_Data_Read(void);
void UI_Data_Write(void);


/****************************************************************************************************/
/**********************************************类内存空间********************************************/
/****************************************************************************************************/
enum Kind{
	UI_Kind_catalog_Go,						//发车UI
	UI_Kind_catalog_Show_1,					//显示类(杂类数据)
	UI_Kind_catalog_Show_2,					//显示类(方向控制器数据)
	UI_Kind_catalog_Show_3,					//显示类(速度控制器数据)
	UI_Kind_catalog_Show_Annulus_50cm,		//显示类(环形匹配控制器数据)
	UI_Kind_catalog_Show_Annulus_100cm,		//显示类(环形匹配控制器数据)
	UI_Kind_catalog_Show_Annulus_200cm,		//显示类(环形匹配控制器数据)
	UI_Kind_catalog_Order,					//命令类
	UI_Kind_catalog_PC,						//调参类
}UI_Kind_Catalog;

uint16_t UI_time = 0;
uint8_t UI_time_dida = 0;
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/******************************************  Car_Go  ************************************************/
/****************************************************************************************************/
/*
 *	UI_Car_Go
 */
void UI_Car_Go(uint16_t time)
{
	static uint8_t Go_state = 0;
	static uint16_t Go_time = 0;
	static uint8_t temp = 0;

	temp++;

	//车在断电态
	if(Car_State == Car_State_Outage)
	{
		Cache_OLED_P6x8Str(0,1,"Car_state:");
		Cache_OLED_P6x8Num(64,1,Car_State);			//运行状态

		Cache_OLED_P6x8Str(0,2,"Go_time:");
		Cache_OLED_P6x8Num(64,2,Go_time);			//发车计时
		
		Cache_OLED_P6x8Str(0,3,"form:");
		Cache_OLED_P6x8Num(64,3,form);							//逻辑状态			
		Cache_OLED_P6x8Str(0,4,"Dual_Me_S(0X):");
		Cache_OLED_P6x8Num_0X(100,4,Dual_Me_Status);				//逻辑状态			
		Cache_OLED_P6x8Str(0,5,"Dual_She_S(0X):");
		Cache_OLED_P6x8Num_0X(100,5,Dual_She_Status);				//逻辑状态		
		Cache_OLED_P6x8Str(0,6,"Dual_Dist");
		Cache_OLED_P6x8floatNum(64,6,Dual_Distance);				//逻辑状态
		
		
		

//		Cache_OLED_P6x8Str(0,4,"Outage_f:");
//		Cache_OLED_P6x8Num(64,4,Car_outage_flag);

//		Cache_OLED_P6x8Str(0,5,"Depart_f:");
//		Cache_OLED_P6x8Num(64,5,Car_depart_flag);

//		Cache_OLED_P6x8Str(0,6,"Park_f_:");
//		Cache_OLED_P6x8Num(64,6,Car_park_flag);

		Cache_OLED_P6x8Str(0,7,"Power:");
		Cache_OLED_P6x8floatNum(64,7,f_Car_Voltage);

		Cache_Update_OLED();

		if(Go_state == 0)
		{
			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc)
			{
				Go_state = 1;
				Bell_Cry(0,100);
			}
		}
		else if(Go_state == 1)
		{
			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Low)
			{
				Go_state = 2;
				Go_time = 0;
			}
		}
		else if(Go_state == 2)
		{
			Go_time += time;
			if(Go_time == 500)
				Bell_Cry(0,100);
			if(Go_time >= 1000)
			{
				Go_state = 0;
				OLED_CLS();		//起步时熄屏
				Car_depart_flag = 1;			//发车信号
			}
		}

		if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
			UI_Kind_Catalog++;
		if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
			UI_Kind_Catalog--;
	}
	//车在其他运行态
	else
	{
		if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Drop)
		{
			Bell_Cry(0,200);
			Car_outage_flag = 1;
		}
		if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
			UI_Kind_Catalog++;
		if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
			UI_Kind_Catalog--;
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/**********************************************  Order  *********************************************/
/****************************************************************************************************/
enum Order{
	UI_Order_Data_Stockpile,				//数据存储页面
	UI_Order_Data_Recover,					//数据恢复页面
	UI_Order_ADC_Demarcate_All,				//电感标定
	UI_Order_ADC_Demarcate_V_LL,			//电感标定
	UI_Order_ADC_Demarcate_J_L,				//电感标定
	UI_Order_ADC_Demarcate_Z_Z,				//电感标定
	UI_Order_ADC_Demarcate_V_RR,			//电感标定
	UI_Order_ADC_Demarcate_J_R,				//电感标定
	UI_Order_ADC_Demarcate_H_L,				//电感标定
	UI_Order_ADC_Demarcate_H_R,				//电感标定
	UI_Order_ADC_Demarcate_Z_H,				//电感标定
	UI_Order_Annulus_Matching_Clear_50cm,	//环形匹配数据清除
	UI_Order_Annulus_Matching_Add_50cm,		//环形匹配数据添加
	UI_Order_Annulus_Matching_Clear_100cm,	//环形匹配数据清除
	UI_Order_Annulus_Matching_Add_100cm,	//环形匹配数据添加
	UI_Order_Annulus_Matching_Clear_200cm,	//环形匹配数据清除
	UI_Order_Annulus_Matching_Add_200cm,	//环形匹配数据添加
}UI_Order_catalog;


/*
 * 命令
 */
void UI_Order(void)
{
	//UI操作
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;
	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		UI_Order_catalog--;
	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		UI_Order_catalog++;

	//
	switch(UI_Order_catalog){
		case UI_Order_Data_Stockpile://保存PC数据到EEPROM
		{
			Cache_OLED_P6x8Str(10,2,"Press the confirm");
			Cache_OLED_P6x8Str(10,3,"To store data");
			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{//EEPROM写入数据
				UI_Data_Write();
				Bell_Cry(0,100);
				Cache_OLED_P6x8Str(30,4,"Store  OK!!!");
			}
		}
		break;
		case UI_Order_Data_Recover://恢复默认数据
		{
			Cache_OLED_P6x8Str(10,2,"Data_Recover");
			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{

			}
		}
		break;
		case UI_Order_ADC_Demarcate_All://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_All");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_All);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_All = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_V_LL://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_V_LL");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_V_LL);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_V_LL = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_V_RR://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_V_RR");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_V_RR);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_V_RR = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_H_L://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_H_L");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_H_L);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_H_L = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_H_R://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_H_R");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_H_R);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_H_R = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_J_L://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_J_L");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_J_L);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_J_L = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_J_R://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_J_R");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_J_R);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_J_R = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_Z_Z://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_Z_Z");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_Z_Z);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_Z_Z = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_ADC_Demarcate_Z_H://标定数据
		{
			Cache_OLED_P6x8Str(10,2,"Demarcate_Z_H");
			Cache_OLED_P6x8floatNum(64,3,elec_data.Demarcate_Rate_Z_H);
			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				Demarcate_Order_Z_H = 1;
				Bell_Cry(0, 200);
			}
		}
		break;
		case UI_Order_Annulus_Matching_Clear_50cm://清除匹配数据
		{
			Cache_OLED_P6x8Str(0,2,"Matching_Clear_50cm");
			Cache_OLED_P6x8Str(0,3,"Valid_Data_50cm");
			Cache_OLED_P6x8Num(64,4,Annulus_Valid_Data_50cm);
			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_ACCRise)
			{
				Direction_Annulus_Valid_Data_Clear_50cm();
				Bell_Cry(0,100);
				Cache_OLED_P6x8Str(30,5,"Clear  OK!!!");
			}
		}
		break;
		case UI_Order_Annulus_Matching_Add_50cm://增加匹配数据
		{
			Cache_OLED_P6x8Str(0,1,"Matching_Add_50cm");
			Cache_OLED_P6x8Str(0,2,"Valid_Data_50cm");
			Cache_OLED_P6x8Num(64,3,Annulus_Valid_Data_50cm);

			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);
			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				if(Direction_Annulus_Valid_Data_Add_50cm())
				{
					Cache_OLED_P6x8Str(30,4,"Add  ERROR!!!");
					Bell_Cry(0,1000);
				}
				else
				{
					Cache_OLED_P6x8Str(30,4,"Add  OK!!!");
					Bell_Cry(0,100);
				}
			}
			else if(Key_Inquire_data(Key_Middle_Read) == Key_bit_ACCRise)
			{
				Direction_Annulus_Valid_Data_Save_50cm();
				Bell_Cry(0,300);
				Cache_OLED_P6x8Str(30,4,"Save  OK!!!");
			}
		}
		break;
		case UI_Order_Annulus_Matching_Clear_100cm://清除匹配数据
		{
			Cache_OLED_P6x8Str(0,2,"Matching_Clear_100cm");
			Cache_OLED_P6x8Str(0,3,"Valid_Data_100cm");
			Cache_OLED_P6x8Num(64,4,Annulus_Valid_Data_100cm);
			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_ACCRise)
			{
				Direction_Annulus_Valid_Data_Clear_100cm();
				Bell_Cry(0,100);
				Cache_OLED_P6x8Str(30,5,"Clear  OK!!!");
			}
		}
		break;
		case UI_Order_Annulus_Matching_Add_100cm://增加匹配数据
		{
			Cache_OLED_P6x8Str(0,1,"Matching_Add_100cm");
			Cache_OLED_P6x8Str(0,2,"Valid_Data_100cm");
			Cache_OLED_P6x8Num(64,3,Annulus_Valid_Data_100cm);

			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				if(Direction_Annulus_Valid_Data_Add_100cm())
				{
					Cache_OLED_P6x8Str(30,4,"Add  ERROR!!!");
					Bell_Cry(0,1000);
				}
				else
				{
					Cache_OLED_P6x8Str(30,4,"Add  OK!!!");
					Bell_Cry(0,100);
				}
			}
			else if(Key_Inquire_data(Key_Middle_Read) == Key_bit_ACCRise)
			{
				Direction_Annulus_Valid_Data_Save_100cm();
				Bell_Cry(0,300);
				Cache_OLED_P6x8Str(30,4,"Save  OK!!!");
			}
		}
		break;
		case UI_Order_Annulus_Matching_Clear_200cm://清除匹配数据
				Cache_OLED_P6x8Str(0,2,"Matching_Clear_200cm");
				Cache_OLED_P6x8Str(0,3,"Valid_Data_200cm");
				Cache_OLED_P6x8Num(64,4,Annulus_Valid_Data_200cm);

				if(Key_Inquire_data(Key_Middle_Read) == Key_bit_ACCRise)
				{
					Direction_Annulus_Valid_Data_Clear_200cm();
					Bell_Cry(0,100);
					Cache_OLED_P6x8Str(30,5,"Clear  OK!!!");
				}
		break;
		case UI_Order_Annulus_Matching_Add_200cm://增加匹配数据
		{
			Cache_OLED_P6x8Str(0,1,"Matching_Add_200cm");
			Cache_OLED_P6x8Str(0,2,"Valid_Data_200cm");
			Cache_OLED_P6x8Num(64,3,Annulus_Valid_Data_200cm);

			Cache_OLED_P6x8Num(10,5,elec_data.Voltage_V_LL);
			Cache_OLED_P6x8Num(55,5,elec_data.Voltage_Z_Z);
			Cache_OLED_P6x8Num(100,5,elec_data.Voltage_V_RR);
			Cache_OLED_P6x8Num(10,6,elec_data.Voltage_H_L);
			Cache_OLED_P6x8Num(55,6,elec_data.Voltage_Z_H);
			Cache_OLED_P6x8Num(100,6,elec_data.Voltage_H_R);
			Cache_OLED_P6x8Num(10,7,elec_data.Voltage_J_L);
			Cache_OLED_P6x8Num(100,7,elec_data.Voltage_J_R);

			if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			{
				if(Direction_Annulus_Valid_Data_Add_200cm())
				{
					Cache_OLED_P6x8Str(30,4,"Add  ERROR!!!");
					Bell_Cry(0,1000);
				}
				else
				{
					Cache_OLED_P6x8Str(30,4,"Add  OK!!!");
					Bell_Cry(0,100);
				}
			}
			else if(Key_Inquire_data(Key_Middle_Read) == Key_bit_ACCRise)
			{
				Direction_Annulus_Valid_Data_Save_200cm();
				Bell_Cry(0,300);
				Cache_OLED_P6x8Str(30,4,"Save  OK!!!");
			}
		}break;
		default:

		break;
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/******************************************  Show 1 *************************************************/
/****************************************************************************************************/
enum Show_1{
	UI_Show_1_catalog_Time,				//计时查询
	UI_Show_1_catalog_Key,				//Key数据
	UI_Show_1_catalog_Ultrasonic,		//超声波数据
	UI_Show_1_catalog_Dual_Packet,		//双车包状态
	UI_Show_1_catalog_Dual_Data,		//双车包数据
	UI_Show_1_catalog_Hoare_Data,		//霍尔数据
}UI_Show_1_catalog;

/*
 *	数据显示_1
 */
void UI_Show_1(void)
{	
	//UI操作
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;
	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		UI_Show_1_catalog--;
	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		UI_Show_1_catalog++;

	//UI_Show_1_catalog_Time计时查询
	int16_t Break_Time,Leisure_Time;

	switch (UI_Show_1_catalog){
		case UI_Show_1_catalog_Time://程序占用CPU时间
				Break_Time = (Gather_Data_Time + Direction_Time + Speed_Time + Dual_Time);
				Leisure_Time = 1000 - Break_Time;

				Cache_OLED_P6x8Str(0,1,"Time");

				Cache_OLED_P6x8Str(0,2,"Gather   :");
				Cache_OLED_P6x8Str(0,3,"Direction:");
				Cache_OLED_P6x8Str(0,4,"Speed    :");
				Cache_OLED_P6x8Str(0,5,"Dual     :");
				Cache_OLED_P6x8Str(0,6,"Break    :");
				Cache_OLED_P6x8Str(0,7,"Leisure  :");
				Cache_OLED_P6x8Str(98,2,"us");
				Cache_OLED_P6x8Str(98,3,"us");
				Cache_OLED_P6x8Str(98,4,"us");
				Cache_OLED_P6x8Str(98,5,"us");
				Cache_OLED_P6x8Str(98,6,"us");
				Cache_OLED_P6x8Str(98,7,"us");
				Cache_OLED_P6x8Num(64,2,Gather_Data_Time);
				Cache_OLED_P6x8Num(64,3,Direction_Time);
				Cache_OLED_P6x8Num(64,4,Speed_Time);
				Cache_OLED_P6x8Num(64,5,Dual_Time);
				Cache_OLED_P6x8Num(64,6,Break_Time);
				Cache_OLED_P6x8Num(64,7,Leisure_Time);
				if(Leisure_Time < 0)
					Bell_Cry(0,100);
		break;
		case UI_Show_1_catalog_Key://Key数据
				Cache_OLED_P6x8Str(0,1,"Key_Read");
				Cache_OLED_P6x8Num(0,3,Key_Inquire_data(Key_Up_Read));
				Cache_OLED_P6x8Num(0,4,Key_Inquire_data(Key_Down_Read));
				Cache_OLED_P6x8Num(0,5,Key_Inquire_data(Key_Left_Read));
				Cache_OLED_P6x8Num(0,6,Key_Inquire_data(Key_Right_Read));
				Cache_OLED_P6x8Num(0,7,Key_Inquire_data(Key_Middle_Read));
				Cache_OLED_P6x8Num(64,4,Key_Inquire_data(Key_Flag1_Read));
				Cache_OLED_P6x8Num(64,5,Key_Inquire_data(Key_Flag2_Read));
				Cache_OLED_P6x8Num(64,6,Key_Inquire_data(Key_Flag3_Read));
				Cache_OLED_P6x8Num(64,7,Key_Inquire_data(Key_Flag4_Read));
		break;
		case UI_Show_1_catalog_Ultrasonic://超声波数据
				Cache_OLED_P6x8Str(0,1,"Ultrasonic_Data");	//超声波数据
		
				Cache_OLED_P6x8Str(0,3,"G_time");	//高电平时间
				Cache_OLED_P6x8Str(0,4,"D_time");	//低
				Cache_OLED_P6x8Str(0,5,"Distance");	//距离
				Cache_OLED_P6x8Str(0,6,"Valid");	//有效
				Cache_OLED_P6x8Str(0,7,"Time");		//计次
		
				Cache_OLED_P6x8Num(64,3,Ultrasonic_G_Time);
				Cache_OLED_P6x8Num(64,4,Ultrasonic_D_Time);
				Cache_OLED_P6x8Num(64,5,Ultrasonic_Distance);
				Cache_OLED_P6x8Num(64,6,Ultrasonic_Valid);
				Cache_OLED_P6x8Num(64,7,Ultrasonic_Time);
		break;
		case UI_Show_1_catalog_Dual_Packet://双车包状态
				Cache_OLED_P6x8Str(0,1,"Dual_Packet");	//超声波数据

				Cache_OLED_P6x8Str(80,1,"Tx");	//超声波数据
				Cache_OLED_P6x8Str(100,1,"Rx");	//超声波数据

			
				Cache_OLED_P6x8Str(0,2,"Ststus");	//状态
				Cache_OLED_P6x8Str(0,3,"Count");	//总计次
				Cache_OLED_P6x8Str(0,4,"OK");	//成功计次
				Cache_OLED_P6x8Str(0,5,"Error");	//错误计次
				Cache_OLED_P6x8Str(0,6,"Loss");	//丢包率
				Cache_OLED_P6x8Str(0,7,"Speed");	//丢包率
		
				Cache_OLED_P6x8Num(40,2,Tx_STATUS);
				Cache_OLED_P6x8Num(40,3,Tx_Count);
				Cache_OLED_P6x8Num(40,4,Tx_OK_Count);
				Cache_OLED_P6x8Num(40,5,Tx_Error_Count);
				Cache_OLED_P6x8Num(40,6,Tx_Packet_Loss_Probability);
				Cache_OLED_P6x8Num(40,7,Tx_Speed);

				Cache_OLED_P6x8Num(80,2,Rx_STATUS);
				Cache_OLED_P6x8Num(80,3,Rx_Count);
				Cache_OLED_P6x8Num(80,4,Rx_OK_Count);
				Cache_OLED_P6x8Num(80,5,Rx_Error_Count);
				Cache_OLED_P6x8Num(80,6,Rx_Packet_Loss_Probability);
				Cache_OLED_P6x8Num(80,7,Rx_Speed);

		break;
		case UI_Show_1_catalog_Dual_Data://双车数据
				Cache_OLED_P6x8Str(0,1,"Dual_Data");	//超声波数据

				Cache_OLED_P6x8Str(30,2,"Me");	//超声波数据
				Cache_OLED_P6x8Str(80,2,"She");	//超声波数据

				Cache_OLED_P6x8Str(0,3,"ZT(0X)");	//双车状态
				Cache_OLED_P6x8Str(0,4,"SD");	//双车速度
				Cache_OLED_P6x8Str(0,5,"FX");	//双车方向
				Cache_OLED_P6x8Str(0,6,"JL");	//双车距离
				Cache_OLED_P6x8Str(0,7,"CS");	//双车环形调度次数	

				Cache_OLED_P6x8Num_0X(48,3,Dual_Me_Status);
				Cache_OLED_P6x8Num(28,4,Dual_Me_Speed);
				Cache_OLED_P6x8Num(28,5,Dual_Me_Direction);
				Cache_OLED_P6x8Num(28,6,Dual_Me_Distance);
				Cache_OLED_P6x8Num(28,7,Dual_Me_Number);
		
				Cache_OLED_P6x8Num_0X(88,3,Dual_She_Status);
				Cache_OLED_P6x8Num(78,4,Dual_She_Speed);
				Cache_OLED_P6x8Num(78,5,Dual_She_Direction);
				Cache_OLED_P6x8Num(78,6,Dual_She_Distance);
				Cache_OLED_P6x8Num(78,7,Dual_She_Number);		
		break;
		case UI_Show_1_catalog_Hoare_Data://霍尔数据
				Cache_OLED_P6x8Str(0,1,"Hoare_1");	//超声波数据
				Cache_OLED_P6x8Str(0,2,"Hoare_2");	//超声波数据
				Cache_OLED_P6x8Str(0,3,"Hoare_3");	//超声波数据
				Cache_OLED_P6x8Str(0,4,"Hoare_4");	//超声波数据
				Cache_OLED_P6x8Str(0,5,"Hoare_5");	//超声波数据
				Cache_OLED_P6x8Str(0,6,"Hoare_6");	//超声波数据
				Cache_OLED_P6x8Str(0,7,"Hoare_7");	//超声波数据
				Cache_OLED_P6x8Num(64,1,Car_Data_Hoare_data[0]);
				Cache_OLED_P6x8Num(64,2,Car_Data_Hoare_data[1]);
				Cache_OLED_P6x8Num(64,3,Car_Data_Hoare_data[2]);
				Cache_OLED_P6x8Num(64,4,Car_Data_Hoare_data[3]);	
				Cache_OLED_P6x8Num(64,5,Car_Data_Hoare_data[4]);
				Cache_OLED_P6x8Num(64,6,Car_Data_Hoare_data[5]);
				Cache_OLED_P6x8Num(64,7,Car_Data_Hoare_data[6]);
		break;
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/

/****************************************************************************************************/
/******************************************  Show 2 *************************************************/
/****************************************************************************************************/
enum Show_2{
	UI_Show_1_catalog_Direction_Controller_Original,			//方向控制器---电感ADC数据
	UI_Show_1_catalog_Direction_Controller_Voltage,				//方向控制器---电感电压
	UI_Show_2_catalog_Direction_Controller_elec_data_1,			//方向控制器---初算数据
	UI_Show_2_catalog_Direction_Controller_elec_data_2,			//方向控制器---初算数据
	UI_Show_2_catalog_Direction_Controller_PWC_1,				//方向控制器---计算法过程数据
	UI_Show_2_catalog_Direction_Controller_PWC_2,				//方向控制器---计算法过程数据
	UI_Show_2_catalog_Direction_Controller_Position_Way,		//方向控制器---赛道定位器输出
	UI_Show_2_catalog_Direction_Controller_Track_1,				//方向控制器---赛道判定器输出
	UI_Show_2_catalog_Direction_Controller_Track_2,				//方向控制器---赛道判定器输出
	UI_Show_2_catalog_Direction_Controller_Track_3,				//方向控制器---赛道判定器输出
	UI_Show_2_catalog_Direction_Controller_Out,					//方向控制器---控制器输出
}UI_Show_2_catalog;

/*
 *	数据显示_2
 */
void UI_Show_2(void)
{	
	//UI操作
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;
	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		UI_Show_2_catalog--;
	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		UI_Show_2_catalog++;

	uint8_t x_L_l = 64 - (int16_t)PWC_E_L_l/20;
	uint8_t x_L_r = 64 - (int16_t)PWC_E_L_r/20;
	uint8_t x_R_l = 64 - (int16_t)PWC_E_R_l/20;
	uint8_t x_R_r = 64 - (int16_t)PWC_E_R_r/20;
	uint8_t x_pwc_S = 64 - (int16_t)f_track_centre_Position_Way_Count_S/20;

	uint8_t x_A = 60 - (int16_t)unit_annulus_data.track_centre/11;
	uint8_t x_R = 60 - (int16_t)unit_ramp_data.track_centre/11;
	uint8_t x_C = 60 - (int16_t)unit_cross_data.track_centre/11;
	uint8_t x_L = 60 - (int16_t)unit_lose_data.track_centre/11;
	uint8_t x_W = 60 - (int16_t)unit_wire_data.track_centre/11;
	uint8_t x_T = 60 - (int16_t)f_track_centre/11;

	switch (UI_Show_2_catalog){
		case UI_Show_1_catalog_Direction_Controller_Original://方向控制器---电感ADC数据
				Cache_OLED_P6x8Str(0,1,"Inductance_original");
				Cache_OLED_P6x8Str(0,2,"AD0_0:");
				Cache_OLED_P6x8Str(0,3,"AD0_2:");
				Cache_OLED_P6x8Str(0,4,"AD0_3:");
				Cache_OLED_P6x8Str(0,5,"AD0_4:");
				Cache_OLED_P6x8Str(0,6,"AD0_6:");
				Cache_OLED_P6x8Str(64,2,"AD1_0:");
				Cache_OLED_P6x8Str(64,3,"AD1_2:");
				Cache_OLED_P6x8Str(64,4,"AD1_3:");
				Cache_OLED_P6x8Str(64,5,"AD1_4:");
				Cache_OLED_P6x8Str(64,6,"AD1_6:");
				Cache_OLED_P6x8Num(38,2,Car_Data_ADC0[0]);
				Cache_OLED_P6x8Num(38,3,Car_Data_ADC0[2]);
				Cache_OLED_P6x8Num(38,4,Car_Data_ADC0[3]);
				Cache_OLED_P6x8Num(38,5,Car_Data_ADC0[4]);
				Cache_OLED_P6x8Num(38,6,Car_Data_ADC0[6]);
				Cache_OLED_P6x8Num(102,2,Car_Data_ADC1[0]);
				Cache_OLED_P6x8Num(102,3,Car_Data_ADC1[2]);
				Cache_OLED_P6x8Num(102,4,Car_Data_ADC1[3]);
				Cache_OLED_P6x8Num(102,5,Car_Data_ADC1[4]);
				Cache_OLED_P6x8Num(102,6,Car_Data_ADC1[6]);
		break;
		case UI_Show_1_catalog_Direction_Controller_Voltage://方向控制器---电感电压
				//Cache_OLED_P6x8Str(0,1,"Inductance_voltage");

				Cache_OLED_P6x8Num(0,1,(int32_t)elec_data.Voltage_J_L);
				Cache_OLED_P6x8Num(10,2,(int32_t)elec_data.Voltage_H_L);
				Cache_OLED_P6x8Num(30,1,(int32_t)elec_data.Voltage_V_LL);
				Cache_OLED_P6x8Num(40,2,(int32_t)elec_data.Voltage_Z_Z);
				Cache_OLED_P6x8Num(60,1,(int32_t)elec_data.Voltage_Z_H);
				Cache_OLED_P6x8Num(70,2,(int32_t)elec_data.Voltage_V_RR);
				Cache_OLED_P6x8Num(90,1,(int32_t)elec_data.Voltage_H_R);
				Cache_OLED_P6x8Num(100,2,(int32_t)elec_data.Voltage_J_R);

				Cache_OLED_Rectangle(0,  (63-((uint16_t)elec_data.Voltage_J_L)/100),  8,   63);
				Cache_OLED_Rectangle(14, (63-((uint16_t)elec_data.Voltage_H_L)/100),  22,  63);
				Cache_OLED_Rectangle(28, (63-((uint16_t)elec_data.Voltage_V_LL)/100), 36,  63);
				Cache_OLED_Rectangle(42, (63-((uint16_t)elec_data.Voltage_Z_Z)/100),  50,  63);
				Cache_OLED_Rectangle(56, (63-((uint16_t)elec_data.Voltage_Z_H)/100),  64,  63);
				Cache_OLED_Rectangle(70, (63-((uint16_t)elec_data.Voltage_V_RR)/100), 78,  63);
				Cache_OLED_Rectangle(84, (63-((uint16_t)elec_data.Voltage_H_R)/100),  92,  63);
				Cache_OLED_Rectangle(98, (63-((uint16_t)elec_data.Voltage_J_R)/100),  106, 63);
		break; 
		case UI_Show_2_catalog_Direction_Controller_elec_data_1://方向控制器---初算数据
				Cache_OLED_P6x8Str(0,1,"Dire_elec_data");

				Cache_OLED_P6x8Str(0,2,"Square_L");
				Cache_OLED_P6x8Str(0,3,"Square_R");
				Cache_OLED_P6x8Str(0,4,"Sum_V");
				Cache_OLED_P6x8Str(0,5,"Sum_H");
				Cache_OLED_P6x8Str(0,6,"Sum_J");
				Cache_OLED_P6x8Str(0,7,"Sum_Square");
				Cache_OLED_P6x8Num(64,2,elec_data.Square_L);
				Cache_OLED_P6x8Num(64,3,elec_data.Square_R);
				Cache_OLED_P6x8Num(64,4,elec_data.Sum_V);
				Cache_OLED_P6x8Num(64,5,elec_data.Sum_H);
				Cache_OLED_P6x8Num(64,6,elec_data.Sum_J);
				Cache_OLED_P6x8Num(64,7,elec_data.Sum_Square);
				//Cache_OLED_P6x8floatNum(64,7,elec_data.Sum_All);
		break;
		case UI_Show_2_catalog_Direction_Controller_elec_data_2://方向控制器---初算数据
				Cache_OLED_P6x8Str(0,1,"Dire_elec_data");

				Cache_OLED_P6x8Str(0,2,"Sub_H");
				Cache_OLED_P6x8Str(0,3,"Sub_V");
				Cache_OLED_P6x8Str(0,4,"Sub_Square");
				Cache_OLED_P6x8Num(64,2,elec_data.Sub_H);
				Cache_OLED_P6x8Num(64,3,elec_data.Sub_V);
				Cache_OLED_P6x8Num(64,4,elec_data.Sub_Square);
		break;
		case UI_Show_2_catalog_Direction_Controller_PWC_1://方向控制器---计算法过程数据
				Cache_OLED_P6x8Str(0,1,"Position_Way_Count");

				Cache_OLED_P6x8Str(0,2,"PWC_L");
				Cache_OLED_P6x8Str(0,3,"PWC_R");
				Cache_OLED_P6x8Str(0,4,"PWC_E_L_l");
				Cache_OLED_P6x8Str(0,5,"PWC_E_L_r");
				Cache_OLED_P6x8Str(0,6,"PWC_E_R_l");
				Cache_OLED_P6x8Str(0,7,"PWC_E_R_r");
				Cache_OLED_P6x8Num(64,2,PWC_L);
				Cache_OLED_P6x8Num(64,3,PWC_R);
				Cache_OLED_P6x8Num(64,4,PWC_E_L_l);
				Cache_OLED_P6x8Num(64,5,PWC_E_L_r);
				Cache_OLED_P6x8Num(64,6,PWC_E_R_l);
				Cache_OLED_P6x8Num(64,7,PWC_E_R_r);
		break;
		case UI_Show_2_catalog_Direction_Controller_PWC_2://方向控制器---计算法过程数据
				Cache_OLED_P6x8Str(0,1,"Position_Way_Count");

				Cache_OLED_P6x8Num(0,2,(int32_t)PWC_E_L_l);
				Cache_OLED_P6x8Num(32,2,(int32_t)PWC_E_L_r);
				Cache_OLED_P6x8Num(64,3,(int32_t)PWC_E_R_l);
				Cache_OLED_P6x8Num(96,3,(int32_t)PWC_E_R_r);
				Cache_OLED_P6x8Num(64,2,PWC_Transition);
				Cache_OLED_P6x8Num(0,7,DC_temp0);
				Cache_OLED_P6x8Num(40,7,DC_temp1);
				Cache_OLED_P6x8Num(80,7,DC_temp2);
				Cache_OLED_Rectangle((x_L_l),(8*4),(x_L_l+1),(8*4+7));
				Cache_OLED_Rectangle((x_L_r),(8*4),(x_L_r+1),(8*4+7));
				Cache_OLED_Rectangle((x_R_l),(8*5),(x_R_l+1),(8*5+7));
				Cache_OLED_Rectangle((x_R_r),(8*5),(x_R_r+1),(8*5+7));
				Cache_OLED_Rectangle((x_pwc_S),(8*6),(x_pwc_S+1),(8*6+7));
		break;
		case UI_Show_2_catalog_Direction_Controller_Position_Way://方向控制器---赛道定位器输出
				Cache_OLED_P6x8Str(0,1,"Dire_Position_Way");

				Cache_OLED_P6x8Str(0,2,"Count_V");
				Cache_OLED_P6x8Str(0,3,"Count_S");
				Cache_OLED_P6x8Str(0,4,"Count_J");
				Cache_OLED_P6x8Str(0,5,"Co_Ann_S");
				Cache_OLED_P6x8Str(0,6,"Co_Ann_J");
				//Cache_OLED_P6x8Str(0,7,"Count_Sp");
				Cache_OLED_P6x8Num(64,2,f_track_centre_Position_Way_Count_V);
				Cache_OLED_P6x8Num(64,3,f_track_centre_Position_Way_Count_S);
				Cache_OLED_P6x8Num(64,4,f_track_centre_Position_Way_Count_J);
				Cache_OLED_P6x8Num(64,5,f_track_centre_Position_Way_Count_Annulus_S);
				Cache_OLED_P6x8Num(64,6,f_track_centre_Position_Way_Count_Annulus_J);
				//Cache_OLED_P6x8Num(64,7,f_track_centre_Position_Way_Count_Speed);
		break;
		case UI_Show_2_catalog_Direction_Controller_Track_1://方向控制器---赛道判定器输出
				Cache_OLED_P6x8Str(0,1,"Direction_Track");

				Cache_OLED_P6x8Str(0,2,"Annulus");
				Cache_OLED_P6x8Str(0,3,"Ramp");
				Cache_OLED_P6x8Str(0,4,"Cross");
				Cache_OLED_P6x8Str(0,5,"Lose");
				Cache_OLED_P6x8Str(0,6,"Wire");
				Cache_OLED_P6x8Str(0,7,"Track");
				Cache_OLED_P6x8Num(64,2,unit_annulus_data.track_centre);
				Cache_OLED_P6x8Num(64,3,unit_ramp_data.track_centre);
				Cache_OLED_P6x8Num(64,4,unit_cross_data.track_centre);
				Cache_OLED_P6x8Num(64,5,unit_lose_data.track_centre);
				Cache_OLED_P6x8Num(64,6,unit_wire_data.track_centre);
				Cache_OLED_P6x8Num(64,7,f_track_centre);
		break;
		case UI_Show_2_catalog_Direction_Controller_Track_2://方向控制器---赛道判定器输出
				Cache_OLED_P6x8Str(0,1,"Direction_Track");

				Cache_OLED_Rectangle((x_A),(8*2),(x_A+1),(8*2+7));
				Cache_OLED_Rectangle((x_R),(8*3),(x_R+1),(8*3+7));
				Cache_OLED_Rectangle((x_C),(8*4),(x_C+1),(8*4+7));
				Cache_OLED_Rectangle((x_L),(8*5),(x_L+1),(8*5+7));
				Cache_OLED_Rectangle((x_W),(8*6),(x_W+1),(8*6+7));
				Cache_OLED_Rectangle((x_T),(8*7),(x_T+1),(8*7+7));

				if(unit_annulus_data.flag)			//环
					Cache_OLED_P6x8Str(121,2,"A");
				else if(unit_ramp_data.flag)		//坡
					Cache_OLED_P6x8Str(121,3,"R");
				else if(unit_cross_data.flag)		//十字
					Cache_OLED_P6x8Str(121,4,"C");
				else if(unit_lose_data.flag)		//丢
					Cache_OLED_P6x8Str(121,5,"L");
				else//else if(unit_wire_data.flag)	//线
					Cache_OLED_P6x8Str(121,6,"W");
		break;
		case UI_Show_2_catalog_Direction_Controller_Track_3://方向控制器---赛道判定器输出
				Cache_OLED_P6x8Str(0,1,"Direction_Track");
				Cache_OLED_P6x8Str(0,2,"form");
				Cache_OLED_P6x8Str(0,3,"MinErr_50");
				Cache_OLED_P6x8Str(0,4,"MinErr_100");
				Cache_OLED_P6x8Str(0,5,"MinErr_200");
//				Cache_OLED_P6x8Str(0,6,"AnMinErr");
//				Cache_OLED_P6x8Str(0,7,"AnMinErr");
				Cache_OLED_P6x8Num(64,2,form);
				Cache_OLED_P6x8Num(64,3,Annulus_Min_Error_50cm);
				Cache_OLED_P6x8Num(64,4,Annulus_Min_Error_100cm);
				Cache_OLED_P6x8Num(64,5,Annulus_Min_Error_200cm);
//				Cache_OLED_P6x8Num(64,6,Annulus_Min_Error);
//				Cache_OLED_P6x8Num(64,7,Annulus_Min_Error);
		break;
		case UI_Show_2_catalog_Direction_Controller_Out://方向控制器---控制器输出
				Cache_OLED_P6x8Str(0,1,"Direction_Out");

				Cache_OLED_P6x8Str(0,2,"Out_Angle");
				Cache_OLED_P6x8Str(0,3,"Pid.P");
				// Cache_OLED_P6x8Str(0,4,"temp1");
				// Cache_OLED_P6x8Str(0,5,"temp2");
				// Cache_OLED_P6x8Str(0,6,"temp3");
				// Cache_OLED_P6x8Str(0,7,"temp4");
				Cache_OLED_P6x8floatNum(64,2,f_Output_Angle);
				Cache_OLED_P6x8floatNum(64,4,direction_pid.P);
				// Cache_OLED_P6x8floatNum(64,5,DC_temp2);
				// Cache_OLED_P6x8floatNum(64,6,DC_temp3);
				// Cache_OLED_P6x8floatNum(64,7,DC_temp4);
		break;
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/******************************************  Show 3 *************************************************/
/****************************************************************************************************/
enum Show_3{
	UI_Show_3_catalog_Environment,					//速度控制器---环境数据
	UI_Show_3_catalog_Out,							//速度控制器---控制输出
}UI_Show_3_catalog;

/*
 *	数据显示_3
 */
void UI_Show_3(void)
{	
	//UI操作
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;
	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		UI_Show_3_catalog--;
	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		UI_Show_3_catalog++;

	switch (UI_Show_3_catalog){
		case UI_Show_3_catalog_Environment://速度控制器---环境数据
				Cache_OLED_P6x8Str(0,1,"Speed_Environ");

				Cache_OLED_P6x8Str(0,2,"Voltage");
				Cache_OLED_P6x8Str(0,3,"Displace");
				Cache_OLED_P6x8Str(0,4,"Speed_Diff");
				Cache_OLED_P6x8Str(0,5,"Car_Rotate");
				Cache_OLED_P6x8Str(0,6,"Acc");
				//Cache_OLED_P6x8Str(0,7,"Acc");

				Cache_OLED_P6x8floatNum(64,2,f_Car_Voltage);
				Cache_OLED_P6x8floatNum(64,3,f_Car_Displace);
				Cache_OLED_P6x8floatNum(64,4,f_Car_Speed_Diff_LB);
				Cache_OLED_P6x8floatNum(64,5,f_Car_Rotate_Speed);
				Cache_OLED_P6x8floatNum(64,6,f_Car_Acc);
				//Cache_OLED_P6x8floatNum(64,7,0);
		break;
		case UI_Show_3_catalog_Out://速度控制器---控制输出
				Cache_OLED_P6x8Str(0,1,"Speed_Out");

				Cache_OLED_P6x8Str(0,2,"Voltage");
				Cache_OLED_P6x8Str(0,3,"Acc");
				Cache_OLED_P6x8Str(0,4,"electro");

				Cache_OLED_P6x8Str(0,6,"Tf");
				Cache_OLED_P6x8Str(0,7,"Ta");

				Cache_OLED_P6x8floatNum(64,2,f_Output_Voltage);
				Cache_OLED_P6x8floatNum(64,3,f_Output_Acc);
				Cache_OLED_P6x8floatNum(64,4,f_Torque_electromagnetism);

				Cache_OLED_P6x8floatNum(64,6,f_Parameter_Tf);
				Cache_OLED_P6x8floatNum(64,7,f_Parameter_Ta);
		break;
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/******************************************  Show Auulus_50cm **************************************/
/****************************************************************************************************/
int8_t UI_Show_Annulus_50cm_catalog;	//

/*
 *	数据显示_Annulus_50cm
 */
void UI_Show_Annulus_50cm(void)
{	
	//UI操作
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;
	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		UI_Show_Annulus_50cm_catalog--;
	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		UI_Show_Annulus_50cm_catalog++;

	Cache_OLED_P6x8Str(0,1,"Valid_Data_50cm:");
	Cache_OLED_P6x8Num(100,1,Annulus_Valid_Data_50cm);

	if(Annulus_Valid_Data_50cm)
	{//存在有效数据
		if(UI_Show_Annulus_50cm_catalog < 0)
			UI_Show_Annulus_50cm_catalog = Annulus_Valid_Data_50cm - 1;
		else if(UI_Show_Annulus_50cm_catalog >= Annulus_Valid_Data_50cm)
			UI_Show_Annulus_50cm_catalog = 0;

		Cache_OLED_P6x8Num(10 , 5, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][0]);
		Cache_OLED_P6x8Num(55 , 5, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][6]);
		Cache_OLED_P6x8Num(100, 5, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][1]);
		Cache_OLED_P6x8Num(10 , 6, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][2]);
		Cache_OLED_P6x8Num(55 , 6, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][7]);
		Cache_OLED_P6x8Num(100, 6, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][3]);
		Cache_OLED_P6x8Num(10 , 7, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][4]);
		Cache_OLED_P6x8Num(100, 7, Annulus_Standard_Data_50cm[UI_Show_Annulus_50cm_catalog][5]);
	}
	else
	{
		UI_Show_Annulus_50cm_catalog = 0;
		Cache_OLED_P6x8Str(0,1,"Valid_Data_50cm:");
		Cache_OLED_P6x8Str(30,3,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,4,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,5,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,6,"No_Data!!!!!");
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/******************************************  Show Auulus_100cm **************************************/
/****************************************************************************************************/
int8_t UI_Show_Annulus_100cm_catalog;	//

/*
 *	数据显示_Annulus_100cm
 */
void UI_Show_Annulus_100cm(void)
{	
	//UI操作
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;
	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		UI_Show_Annulus_100cm_catalog--;
	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		UI_Show_Annulus_100cm_catalog++;

	Cache_OLED_P6x8Str(0,1,"Valid_Data_100cm:");
	Cache_OLED_P6x8Num(100,1,Annulus_Valid_Data_100cm);

	if(Annulus_Valid_Data_100cm)
	{//存在有效数据
		if(UI_Show_Annulus_100cm_catalog < 0)
			UI_Show_Annulus_100cm_catalog = Annulus_Valid_Data_100cm - 1;
		else if(UI_Show_Annulus_100cm_catalog >= Annulus_Valid_Data_100cm)
			UI_Show_Annulus_100cm_catalog = 0;

		Cache_OLED_P6x8Num(10 , 5, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][0]);
		Cache_OLED_P6x8Num(55 , 5, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][6]);
		Cache_OLED_P6x8Num(100, 5, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][1]);
		Cache_OLED_P6x8Num(10 , 6, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][2]);
		Cache_OLED_P6x8Num(55 , 6, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][7]);
		Cache_OLED_P6x8Num(100, 6, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][3]);
		Cache_OLED_P6x8Num(10 , 7, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][4]);
		Cache_OLED_P6x8Num(100, 7, Annulus_Standard_Data_100cm[UI_Show_Annulus_100cm_catalog][5]);
	}
	else
	{
		UI_Show_Annulus_100cm_catalog = 0;
		Cache_OLED_P6x8Str(0,1,"Valid_Data_100cm:");
		Cache_OLED_P6x8Str(30,3,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,4,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,5,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,6,"No_Data!!!!!");
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/******************************************  Show Auulus_200cm **************************************/
/****************************************************************************************************/
int8_t UI_Show_Annulus_200cm_catalog;	//

/*
 *	数据显示_Annulus_200cm
 */
void UI_Show_Annulus_200cm(void)
{	
	//UI操作
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;
	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		UI_Show_Annulus_200cm_catalog--;
	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		UI_Show_Annulus_200cm_catalog++;

	Cache_OLED_P6x8Str(0,1,"Valid_Data_200cm:");
	Cache_OLED_P6x8Num(100,1,Annulus_Valid_Data_200cm);

	if(Annulus_Valid_Data_200cm)
	{//存在有效数据
		if(UI_Show_Annulus_200cm_catalog < 0)
			UI_Show_Annulus_200cm_catalog = Annulus_Valid_Data_200cm - 1;
		else if(UI_Show_Annulus_200cm_catalog >= Annulus_Valid_Data_200cm)
			UI_Show_Annulus_200cm_catalog = 0;

		Cache_OLED_P6x8Num(10 , 5, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][0]);
		Cache_OLED_P6x8Num(55 , 5, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][6]);
		Cache_OLED_P6x8Num(100, 5, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][1]);
		Cache_OLED_P6x8Num(10 , 6, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][2]);
		Cache_OLED_P6x8Num(55 , 6, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][7]);
		Cache_OLED_P6x8Num(100, 6, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][3]);
		Cache_OLED_P6x8Num(10 , 7, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][4]);
		Cache_OLED_P6x8Num(100, 7, Annulus_Standard_Data_200cm[UI_Show_Annulus_200cm_catalog][5]);
	}
	else
	{
		UI_Show_Annulus_200cm_catalog = 0;
		Cache_OLED_P6x8Str(0,1,"Valid_Data_200cm:");
		Cache_OLED_P6x8Str(30,3,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,4,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,5,"No_Data!!!!!");
		Cache_OLED_P6x8Str(30,6,"No_Data!!!!!");
	}
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/

/****************************************************************************************************/
/***************************************  Parameter_Configuration  **********************************/
/****************************************************************************************************/
/*参数配置页---基类*/
// typedef struct
// {
// 	char page_name[22];		//页名称
// 	void *data_pointer[6];	//数据指针
// 	char data_type[6];		//数据类型
// 	char data_name[6][7];	//数据名称
// }Parameter_Configuration_info;

//数据类型定义
#define data_type_null		0x00
#define data_type_s8		0x01
#define data_type_u8		0x02
#define data_type_s16		0x03
#define data_type_u16		0x04
#define data_type_s32		0x05
#define data_type_u32		0x06
#define data_type_float		0x07
#define data_type_double	0x08

/*	参数配置界面定义
 *	0行显示页名称
 * 	1行显示
 *	2~7行显示数据
 *	2~7行：数据类型2列   数据名称6列   数据显示11列
 */
uint8_t UI_PC_Ycatalog = 0;		//参数配置页大目录
uint8_t UI_PC_catalog = 0;		//参数配置页目录
uint8_t UI_PC_x = 0;			//参数配置光标位置
uint8_t UI_PC_y = 0;			//参数配置光标位置

//参数配置锁定	/* 0：非锁定 */		/* 1：锁定页 */		/* 2：锁定字 */
#define UI_PC_Lock_NO	0x00
#define UI_PC_Lock_Page	0x01
#define UI_PC_Lock_Byte	0x02
uint8_t UI_PC_lock = UI_PC_Lock_NO;

//用于显示调节的储存空间
#define UI_num_max						10			//数据最大容量
#define UI_YPage_num_max				4			//最大类数
#define UI_Page_num_max					10//不能改	//每页最大页数
char UI_PC_Num_array[6][UI_num_max+1];

int8_t UI_PC_Text_s8 = -100;
uint8_t UI_PC_Text_u8 = 100;
int16_t UI_PC_Text_s16 = -1000;
uint16_t UI_PC_Text_u16 = 1000;
int32_t UI_PC_Text_s32 = -100000;
uint32_t UI_PC_Text_u32 = 100000;

//Parameter_Configuration_info Parameter_Configuration[UI_Page_num_max];		//定义参数配置页
void *PC_data_pointer[UI_YPage_num_max][UI_Page_num_max][6];

//页名称
const char PC_page_name[UI_YPage_num_max][UI_Page_num_max][22]=
{
	/////"*********************"21个字符长度,别超过//	
	{
		{"ADC-Demarcate"},			//ADC标定数据
		{"Path-Parameter"},			//路径计算参数()
		{"Direction-PID"},			//方向PID
		{"Steering-Parameter"},		//舵机参数
		{"Lose-State"},				//丢线判定
		{"Cross-State"},			//十字判定
		{"Ramp-State"},				//坡道判定		
		{"Annulus-State"},			//环形判定	
		{"Annulus-State"},			//环形判定
		{"DC_Temp"}					//方向控制器Temp
	},
	{
		{"Speed/Acc-PID"},			//页0：	速度/加速度参数
		{"Moto-Parameter"},			//页1：	电机参数
		{"Moto-Parameter"},			//页2：	电机参数
		{"Max_Speed_Set"},			//最大速度(最大+浮动)
		{"Max_Acc_Set"},			//最大加速度(最大+浮动)
		{"FrictionMicro"},			//最大摩擦控制器
		{"SpecialSpeedLimit"},		//特殊地段限速器
		{""},
		{""},
		{"SC_Temp"}					//速度控制器Temp
	},
	{
		{"Debug"},					//调试
		{"ADC_Demarcate"},			//ADC标定
		{"ADC_Demarcate"},			//ADC标定
		{""},
		{""},
		{""},
		{""},
		{""},
		{""},
		{"Highway"}					//大道
	},
	{
		{"Dual_Set"},				//双车控制数据			
		{"Dual_Time"},
		{"Dual_Annulus"},
		{"Dual_Annulus"},
		{"Dual_Highway"},
		{"Dual_Cross"},
		{"Dual_Cross"},
		{"Dual_Cross"},
		{"Dual_Ramp"},
		{"NULL"}
	}
};
//数据类型
const char PC_data_type[UI_YPage_num_max][UI_Page_num_max][6]=
{
	{
		{data_type_float,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},		//0
		{data_type_s16,		data_type_null,		data_type_null,		data_type_s16,		data_type_s16,		data_type_s16},			//1
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float},		//2
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float},		//3
		{data_type_s16,		data_type_s16,		data_type_s16,		data_type_s16,		data_type_null,		data_type_null},		//2
		{data_type_s16,		data_type_s16,		data_type_null,		data_type_s16,		data_type_null,		data_type_float},
		{data_type_s16,		data_type_s16,		data_type_s16,		data_type_s16,		data_type_float,	data_type_float},
		{data_type_s16,		data_type_s16,		data_type_s16,		data_type_s16,		data_type_s16,		data_type_float},
		{data_type_float,	data_type_s16,		data_type_s16,		data_type_s16,		data_type_null,		data_type_float},
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float}		//7
	},
	{
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float},		//0
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float},		//1
		{data_type_s32,		data_type_float,	data_type_float,	data_type_null,		data_type_null,		data_type_float},		//2
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_null,		data_type_null},		//3
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_null,		data_type_null},		//4
		{data_type_float,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},		//5
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_null,		data_type_null},		//6
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_float}		//7
	},
	{
		{data_type_u8,		data_type_u8,		data_type_u8,		data_type_float,	data_type_null,		data_type_null},		//0
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_null,		data_type_null},		//1
		{data_type_float,	data_type_float,	data_type_float,	data_type_float,	data_type_null,		data_type_null},		//2
		{data_type_u8,		data_type_float,	data_type_null,		data_type_null,		data_type_null,		data_type_null},		//3
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},		//4
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},		//5
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},		//6
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},		//7
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},
		{data_type_s16,		data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},
	},
	{
		{data_type_float,	data_type_null,		data_type_null,		data_type_float,	data_type_float,	data_type_float},		//0
		{data_type_u16,		data_type_u16,		data_type_u16,		data_type_u16,		data_type_null,		data_type_null},		//1
		{data_type_u8,		data_type_u8,		data_type_u16,		data_type_u8,		data_type_u8,		data_type_u16},		//2
		{data_type_u8,		data_type_u8,		data_type_u16,		data_type_u8,		data_type_u8,		data_type_u16},		//3
		{data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8,		data_type_null,		data_type_null},		//4
		{data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8},		//5
		{data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8},		//6
		{data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8,		data_type_null,		data_type_null},		//7
		{data_type_u8,		data_type_u8,		data_type_u8,		data_type_u8,		data_type_null,		data_type_null},
		{data_type_null,	data_type_null,		data_type_null,		data_type_null,		data_type_null,		data_type_null},
	}
};

//数据名称
const char PC_data_name[UI_YPage_num_max][UI_Page_num_max][6][9]=
{
	{
		{{"DeRaAll"},	{""},			{""},			{""},			{""},			{""}},				//0
		{{"TrackMax"},	{""},			{""},			{"SmooTh_M"},	{"Smoot_M"},	{"Smoot_Z"}},		//1
		{{"Pmax"},		{"Pmin"},		{"Dmax"},		{"Dmin"},		{"Spe_max"},	{"Spe_min"}},		//2
		{{"MaxAng"},	{"Mid"},		{"Con_L"},		{"Con_R"},		{""},			{"Force"}},			//舵机变量
		{{"in_S_Sub"},	{"iu_Squa"},	{"qu_Bias"},	{"qu_Squa"},	{""},			{""}},				//丢线阈值
		{{"in_H"},		{"qu_H"},		{""},			{"inBiasJ"},	{""},			{"InSafDis"}},		//6
		{{"V_Sum"},		{"H_Sum"},		{"J_Sum"},		{"iq-Z"},		{"InSafDis"},	{"QuSafDis"}},		//6
		{{"in_deno"},	{"in_mi_e"},	{"in_J"},		{"in_ZZ"},		{"in_Lo_S"},	{"inMaDis"}},		//5
		{{"quMaDis"},	{"qu_Lo_S"},	{"qu_S"},		{"qu_Limit"},	{""},			{"inSafDis"}},				//4
		{{"Temp_0"},	{"Temp_1"},		{"Temp_2"},		{"Temp_3"},		{"Temp_4"},		{"Temp_5"}},		//7
	},
	{
		{{"Sp-P"},		{"Sp-I"},		{"Sp-D"},		{"ACC-P"},		{"ACC-I"},		{"ACC_D"}},			//0
		{{"Para_Tf"},	{"Para_Ta"},	{"Para_N"},		{"Para_T"},		{"ParaSpee"},	{"ParaAcc"}},		//1
		{{"Motor_HZ"},	{"Max_V"},		{"Start_V"},	{""},			{""},			{"Force"}},			//2
		{{"Out_Max"},	{"Set_max"},	{"Set_min"},	{"float"},		{""},			{""}},			//3
		{{"Out_Max"},	{"Set_max"},	{"Set_min"},	{"float"},		{""},			{""}},			//4
		{{"MaxMicro"},	{""},			{""},			{""},			{""},			{""}},			//5
		{{"Annulus"},	{"Ramp"},		{"Overtake"},	{"in_Annul"},	{""},			{""}},			//6
		{{""},			{""},			{""},			{""},			{""},			{""}},			//5
		{{""},			{""},			{""},			{""},			{""},			{""}},			//6
		{{"Temp_0"},	{"Temp_1"},		{"Temp_2"},		{"Temp_3"},		{"Temp_4"},		{"Temp_5"}},			//7
	},
	{
		{{"ANO_md"},	{"BiBi_EN"},	{"Car_AorB"},	{"Power_cs"},	{""},			{""}},			//0
		{{"V_LL"},		{"V_RR"},		{"H_L"},		{"H_R"},		{""},			{""}},			//1
		{{"J_L"},		{"J_R"},		{"Z_Z"},		{"Z_H"},		{""},			{""}},			//2
		{{"O_or_P"},	{"Pa_En_Di"},	{""},			{""},			{""},			{""}},			//3
		{{""},			{""},			{""},			{""},			{""},			{""}},			//4
		{{""},			{""},			{""},			{""},			{""},			{""}},			//5
		{{""},			{""},			{""},			{""},			{""},			{""}},			//6
		{{""},			{""},			{""},			{""},			{""},			{""}},			//5
		{{""},			{""},			{""},			{""},			{""},			{""}},			//6
		{{"in_th_V"},	{""},			{""},			{""},			{""},			{""}},			//7
	},
	{
		{{"Dist_Set"},	{""},			{""},			{"P"},			{"I"},			{"D"}},			//0
		{{"AB_Max"},	{"CD_Max"},		{"EF_Max"},		{"GH_Max"},		{""},			{""}},			//1
		{{"AB_Fla_0"},	{"AB_Dir_0"},	{"AB_Dis_0"},	{"AB_Fla_1"},	{"AB_Dir_1"},	{"AB_Dis_1"}},		//2
		{{"AB_Fla_2"},	{"AB_Dir_2"},	{"AB_Dis_2"},	{"AB_Fla_3"},	{"AB_Dir_3"},	{"AB_Dis_3"}},		//3
		{{"CD_Fla_0"},	{"CD_Dir_0"},	{"CD_Fla_1"},	{"CD_Dir_1"},	{""},			{""}},			//4
		{{"EF_Fla_0"},	{"EF_Dir_0"},	{"EF_Fla_1"},	{"EF_Dir_1"},	{"EF_Fla_2"},	{"EF_Dir_2"}},		//5
		{{"EF_Fla_3"},	{"EF_Dir_3"},	{"EF_Fla_4"},	{"EF_Dir_4"},	{"EF_Fla_5"},	{"EF_Dir_5"}},	//6
		{{"EF_Fla_6"},	{"EF_Dir_6"},	{"EF_Fla_7"},	{"EF_Dir_7"},	{""},		{""}},		//5
		{{"GH_Fla_0"},	{"GH_Dir_0"},	{"GH_Fla_1"},	{"GH_Dir_1"},	{""},		{""}},			//6
		{{""},		{""},		{""},		{""},		{""},		{""}},	//7
	}		
};
/*
*	参数配置页操作初始化
*/
void UI_UI_Parameter_Configuration_Init(void)
{
	uint8_t catalog_temp;
	uint8_t Ycatalog_temp;

	/********************大目录********************/
	Ycatalog_temp = 0;

	//页：ADC标定数据
	catalog_temp = 0;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &elec_data.Demarcate_Rate_All;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：路径计算参数
	catalog_temp = 1;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &f_track_max;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Count_Smoothness_threshold_M;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &Count_Smoothness_M;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &Count_Smoothness_Z;

	//页：方向PID
	catalog_temp = 2;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &direction_pid.Pmax;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &direction_pid.Pmin;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &direction_pid.Dmax;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &direction_pid.Dmin;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &direction_pid.Speed_max;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &direction_pid.Speed_min;

	//页：舵机参数
	catalog_temp = 3;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Steering_Max_Angle;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Steering_Mid;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Steering_Conversion_L;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Steering_Conversion_R;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &Output_Angle_Force;

	//页：
	catalog_temp = 4;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &unit_lose_data.into_S_Sub;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &unit_lose_data.into_threshold_Square;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &unit_lose_data.quit_centre_Bias;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &unit_lose_data.quit_threshold_Square;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 5;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &unit_cross_data.into_threshold_H;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &unit_cross_data.quit_threshold_H;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &unit_cross_data.into_centre_Bias_J;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &unit_cross_data.into_Safety_Distance;

	//页：
	catalog_temp = 6;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &unit_ramp_data.into_quit_V_Sum;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &unit_ramp_data.into_quit_J_Sum;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &unit_ramp_data.into_quit_Z_Z;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &unit_ramp_data.into_Safety_Distance;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &unit_ramp_data.quit_Safety_Distance;

	//页：
	catalog_temp = 7;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &unit_annulus_data.into_denominator_min_error;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &unit_annulus_data.into_min_error;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &unit_annulus_data.into_Sum_J_threshold;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &unit_annulus_data.into_Sum_ZZ_threshold;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &unit_annulus_data.into_lose_threshold_Square;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &unit_annulus_data.into_Maintain_Distance;

	//页：
	catalog_temp = 8;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &unit_annulus_data.quit_Maintain_Distance;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &unit_annulus_data.quit_lose_threshold_Square;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &unit_annulus_data.quit_Square_threshold;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &unit_annulus_data.quit_Way_Limit;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &unit_annulus_data.into_Safety_Distance;

	//页：
	catalog_temp = 9;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &DC_temp0;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &DC_temp1;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &DC_temp2;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &DC_temp3;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &DC_temp4;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &DC_temp5;


	/********************大目录********************/
	Ycatalog_temp = 1;

	//页：速度加速度PID
	catalog_temp = 0;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &speed_pid.P;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &speed_pid.I;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &speed_pid.D;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &acc_pid.P;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &acc_pid.I;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &acc_pid.D;

	//页：
	catalog_temp = 1;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &f_Parameter_Tf;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &f_Parameter_Ta;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &f_Parameter_N;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &f_Parameter_T;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &f_Parameter_V_to_Speed;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &f_Parameter_V_to_Acc;

	//页：
	catalog_temp = 2;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Motor_PWM_HZ;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Motor_Max_Voltage;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &f_Start_Voltage;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &Output_V_Force;

	//页：
	catalog_temp = 3;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &f_Set_Max_Output_Speed;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &f_Set_Max_Speed;		
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &f_Set_Min_Speed;		
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &f_float_Max_Speed;		
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 4;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &f_Set_Max_Output_Acc;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &f_Set_Max_Acc;		
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &f_Set_Min_Acc;		
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &f_float_Max_Acc;	
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 5;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &f_Max_Friction_To_max_micro;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 6;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &f_temp_max_Speed_Annulus;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &f_temp_max_Speed_Ramp;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &f_temp_max_Speed_Overtake;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &f_temp_max_Speed_into_Annulus;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 7;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 8;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 9;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &SC_temp0;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &SC_temp1;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &SC_temp2;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &SC_temp3;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &SC_temp4;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &SC_temp5;


	/********************大目录********************/
	Ycatalog_temp = 2;

	//页：
	catalog_temp = 0;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &ANO_Debug_mode;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Car_BiBi_En;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Car_AorB;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Power_cs;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 1;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &elec_data.Demarcate_Rate_V_LL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &elec_data.Demarcate_Rate_V_RR;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &elec_data.Demarcate_Rate_H_L;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &elec_data.Demarcate_Rate_H_R;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 2;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &elec_data.Demarcate_Rate_J_L;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &elec_data.Demarcate_Rate_J_R;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &elec_data.Demarcate_Rate_Z_Z;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &elec_data.Demarcate_Rate_Z_H;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 3;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Car_Hoare_Outage_or_Park;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Car_Park_Enabled_Distance;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 4;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 5;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 6;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 7;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 8;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 9;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &unit_highway_data.into_threshold_V;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	/********************大目录********************/
	Ycatalog_temp = 3;

	//页：
	catalog_temp = 0;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_Distance_Set;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &distance_pid.P;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &distance_pid.I;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &distance_pid.D;

	//页：
	catalog_temp = 1;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_AB_Time_Max;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_CD_Time_Max;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_EF_Time_Max;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_GH_Time_Max;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 2;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_AB_Flag[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_AB_Direction[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_AB_Distance[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_AB_Flag[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &Dual_AB_Direction[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &Dual_AB_Distance[1];

	//页：
	catalog_temp = 3;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_AB_Flag[2];
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_AB_Direction[2];
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_AB_Distance[2];
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_AB_Flag[3];
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &Dual_AB_Direction[3];
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &Dual_AB_Distance[3];

	//页：
	catalog_temp = 4;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_CD_Flag[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_CD_Direction[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_CD_Flag[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_CD_Direction[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 5;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_EF_Flag[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_EF_Direction[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_EF_Flag[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_EF_Direction[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &Dual_EF_Flag[2];
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &Dual_EF_Direction[2];

	//页：
	catalog_temp = 6;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_EF_Flag[3];
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_EF_Direction[3];
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_EF_Flag[4];
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_EF_Direction[4];
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = &Dual_EF_Flag[5];
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = &Dual_EF_Direction[5];

	//页：
	catalog_temp = 7;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_EF_Flag[6];
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_EF_Direction[6];
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_EF_Flag[7];
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_EF_Direction[7];
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 8;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = &Dual_GH_Flag[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = &Dual_GH_Direction[0];
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = &Dual_GH_Flag[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = &Dual_GH_Direction[1];
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;

	//页：
	catalog_temp = 9;
	PC_data_pointer[Ycatalog_temp][catalog_temp][0] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][1] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][2] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][3] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][4] = NULL;
	PC_data_pointer[Ycatalog_temp][catalog_temp][5] = NULL;
}

/*
 *	显示数据类型
 */
void UI_Parameter_Configuration_affiliate_type(uint8_t x,uint8_t y,uint8_t type)
{
	switch(type){
		case data_type_s8:Cache_OLED_P6x8Str(x,y,"s1");break;
		case data_type_u8:Cache_OLED_P6x8Str(x,y,"u1");break;
		case data_type_s16:Cache_OLED_P6x8Str(x,y,"s2");break;
		case data_type_u16:Cache_OLED_P6x8Str(x,y,"u2");break;
		case data_type_s32:Cache_OLED_P6x8Str(x,y,"s4");break;
		case data_type_u32:Cache_OLED_P6x8Str(x,y,"u4");break;
		case data_type_float:Cache_OLED_P6x8Str(x,y,".f");break;
		case data_type_double:Cache_OLED_P6x8Str(x,y,".d");break;
	}
}

/*
 *	格式化数据到调节
 */
void UI_Parameter_Configuration_affiliate_data(void)
{
	uint8_t temp1;

	//显示数据-类型/名称/数据
	for(temp1=0;temp1<6;temp1++)
	{
		if(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1] != NULL)
		{
			if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_s8)
				snprintf(UI_PC_Num_array[temp1], (UI_num_max+1), "%10d", *(int8_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]));
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_u8)
				snprintf(UI_PC_Num_array[temp1], (UI_num_max+1), "%10d", *(uint8_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]));
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_s16)
				snprintf(UI_PC_Num_array[temp1], (UI_num_max+1), "%10d", *(int16_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]));
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_u16)
				snprintf(UI_PC_Num_array[temp1], (UI_num_max+1), "%10d", *(uint16_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]));
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_s32)
				snprintf(UI_PC_Num_array[temp1], (UI_num_max+1), "%10d", *(int32_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]));
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_u32)
				snprintf(UI_PC_Num_array[temp1], (UI_num_max+1), "%10d", *(uint32_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]));
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_float)
				snprintf(UI_PC_Num_array[temp1], (UI_num_max+1), "%10f", *(float*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]));
			UI_PC_Num_array[temp1][UI_num_max] = NULL;	//防止错误死机
		}
	}
}

/*
 *	调节器输出数据
 */
void UI_Parameter_Configuration_affiliate_stockpile(void)
{
	uint8_t 	temp1;
	int32_t 	temp_int32_t;
	uint32_t	temp_uint32_t;
	float		temp_float;
	
	//显示数据-类型/名称/数据
	for(temp1=0;temp1<6;temp1++)
	{
		if(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1] != NULL)
		{
			//数据
			UI_PC_Num_array[temp1][UI_num_max] = NULL;	//添加空白->防止错误死机
			if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_s8)
			{
				sscanf(UI_PC_Num_array[temp1], "%d", &temp_int32_t);
				*(int8_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]) = temp_int32_t;
			}
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_u8)
			{
				sscanf(UI_PC_Num_array[temp1], "%u", &temp_uint32_t);
				*(uint8_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]) = temp_uint32_t;
			}
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_s16)
			{
				sscanf(UI_PC_Num_array[temp1], "%d", &temp_int32_t);
				*(int16_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]) = temp_int32_t;
			}
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_u16)
			{
				sscanf(UI_PC_Num_array[temp1], "%u", &temp_uint32_t);
				*(uint16_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]) = temp_uint32_t;
			}
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_s32)
			{
				sscanf(UI_PC_Num_array[temp1], "%d", &temp_int32_t);
				*(int32_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]) = temp_int32_t;	
			}
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_u32)
			{
				sscanf(UI_PC_Num_array[temp1], "%u", &temp_uint32_t);
				*(uint32_t*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]) = temp_uint32_t;	
			}
			else if(PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1] == data_type_float)
			{
				sscanf(UI_PC_Num_array[temp1], "%f", &temp_float);
				*(float*)(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1]) = temp_float;
			}
		}
	}
}

/*
 *	参数配置页操作模型
 */
void UI_Parameter_Configuration(void)
{
//	static uint8_t Byte_Lock_flag = 0;
	uint8_t temp1 = 0;

	//显示页名称
	Cache_OLED_P6x8Str(0,1,(char*)&PC_page_name[UI_PC_Ycatalog][UI_PC_catalog]);

	//显示辅助操作
	if(UI_PC_lock == UI_PC_Lock_NO)
		Cache_OLED_P6x8Str(84,0,"Lk-NO  ");
	else if(UI_PC_lock == UI_PC_Lock_Page)
		Cache_OLED_P6x8Str(84,0,"Lk-Page");
	else if(UI_PC_lock == UI_PC_Lock_Byte)
		Cache_OLED_P6x8Str(84,0,"Lk-Byte");

	//未锁定时-获得数据
	if(UI_PC_lock == UI_PC_Lock_NO)
		UI_Parameter_Configuration_affiliate_data();

	//显示已经获得的数据-类型/名称/数据
	for(temp1=0;temp1<6;temp1++)
	{
		if(PC_data_pointer[UI_PC_Ycatalog][UI_PC_catalog][temp1] != NULL)
		{
			//显示数据类型
			UI_Parameter_Configuration_affiliate_type(0,(temp1+2),PC_data_type[UI_PC_Ycatalog][UI_PC_catalog][temp1]);
			//显示数据名称
			Cache_OLED_P6x8Str(13,(temp1+2),(char*)&PC_data_name[UI_PC_Ycatalog][UI_PC_catalog][temp1]);
			//显示数据
			Cache_OLED_P6x8Str(62,(temp1+2),UI_PC_Num_array[temp1]);
		}
	}

	//显示光标
	if(UI_PC_lock == UI_PC_Lock_Page)
	{
		if(UI_time_dida)
		{
			Cache_OLED_P6x8byte_inverse((62+6*UI_PC_x), (UI_PC_y+2), (UI_PC_Num_array[UI_PC_y][UI_PC_x]));
		}
	}
	else if(UI_PC_lock == UI_PC_Lock_Byte)
	{
		Cache_OLED_P6x8byte_inverse((62+6*UI_PC_x), (UI_PC_y+2), (UI_PC_Num_array[UI_PC_y][UI_PC_x]));
	}

	/*** 执行操作 ***/

	//任意状态---开关锁
	if(UI_PC_lock == UI_PC_Lock_NO)
	{
		if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			UI_PC_lock = UI_PC_Lock_Page;
	}
	else if(UI_PC_lock == UI_PC_Lock_Page)
	{
		if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
			UI_PC_lock = UI_PC_Lock_Byte;
		else if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc)
		{
			UI_PC_lock = UI_PC_Lock_NO;
			UI_Parameter_Configuration_affiliate_stockpile();	//输出数据
		}
	}
	else if(UI_PC_lock == UI_PC_Lock_Byte)
	{
		if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Rise)
		{
			UI_PC_lock = UI_PC_Lock_Page;
		}
		else if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc)
		{
			UI_PC_lock = UI_PC_Lock_NO;
			UI_Parameter_Configuration_affiliate_stockpile();	//输出数据
		}
	}

	//未锁定时---翻类/翻大页/翻页
	if(UI_PC_lock == UI_PC_Lock_NO)
	{
		if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
			if(UI_PC_Ycatalog == UI_YPage_num_max-1)//限制最大
				UI_Kind_Catalog++;
			else
				UI_PC_Ycatalog++;
		if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
			if(UI_PC_Ycatalog)
				UI_PC_Ycatalog--;
			else
				UI_Kind_Catalog--;
		if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		{
			if(UI_PC_catalog)
				UI_PC_catalog--;
		}
		if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		{
			if(UI_PC_catalog < (UI_Page_num_max-1))//限制最大
				UI_PC_catalog++;	
		}
	}
	//页面锁定时---移动光标
	else if(UI_PC_lock == UI_PC_Lock_Page)
	{
		if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		{
			if(!UI_PC_y)	UI_PC_y = 5;
			else			UI_PC_y--;
		}
		if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		{
			if(UI_PC_y == 5)	UI_PC_y = 0;
			else				UI_PC_y++;
		}
		if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		{
			if(!UI_PC_x)	UI_PC_x = UI_num_max - 1;
			else			UI_PC_x--;
		}
		if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		{
			UI_PC_x++;
			if(UI_PC_x == UI_num_max)	UI_PC_x = 0;					
		}
	}
	//字节锁定---左右移动光标/上下修改数值
	else if(UI_PC_lock == UI_PC_Lock_Byte)
	{
		if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))	//上--位加
		{
			if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == ' ')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = '-';
			else if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == '-')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = '.';
			else if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == '.')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = '0';
			else if((UI_PC_Num_array[UI_PC_y][UI_PC_x] >= '0') && (UI_PC_Num_array[UI_PC_y][UI_PC_x] < '9'))
				UI_PC_Num_array[UI_PC_y][UI_PC_x]++;
			else if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == '9')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = '9';
			else 
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = ' ';
		}
		if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))	//下---位减
		{
			if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == ' ')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = ' ';
			else if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == '-')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = ' ';
			else if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == '.')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = '-';
			else if(UI_PC_Num_array[UI_PC_y][UI_PC_x] == '0')
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = '.';
			else if((UI_PC_Num_array[UI_PC_y][UI_PC_x] > '0') && (UI_PC_Num_array[UI_PC_y][UI_PC_x] <= '9'))
				UI_PC_Num_array[UI_PC_y][UI_PC_x]--;
			else 
				UI_PC_Num_array[UI_PC_y][UI_PC_x] = ' ';		
		}
		if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
		{
			if(!UI_PC_x)	UI_PC_x = UI_num_max - 1;
			else			UI_PC_x--;
		}
		if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
		{
			UI_PC_x++;
			if(UI_PC_x == UI_num_max)	UI_PC_x = 0;
		}
	}
}

/*
 *	UI数据读取
 */
void UI_Data_Read(void)
{
//	PC_data_pointer[UI_YPage_num_max][UI_Page_num_max][6];
//						3				10				6

	uint32_t read_buffer[32];
	uint32_t page_Jia = 0;

	uint8_t num_yPage, num_Page, num_Hang;

	for(num_yPage = 0; num_yPage < UI_YPage_num_max; num_yPage++)
	{
		//读一波数据
		EEPROM_Read(0,(PAGE_ADDR_UI_Data_Start + page_Jia),(uint32_t*)read_buffer,128);
		page_Jia++;

		//读出
		for(num_Page = 0; num_Page < 5; num_Page++)
		{
			for(num_Hang = 0; num_Hang < 6; num_Hang++)
			{
				if(PC_data_pointer[num_yPage][num_Page][num_Hang] != NULL)
				{
					if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_s8)
						*(int8_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(int8_t*)(&read_buffer[(num_Page*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_u8)
						*(uint8_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(uint8_t*)(&read_buffer[(num_Page*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_s16)
						*(int16_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(int16_t*)(&read_buffer[(num_Page*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_u16)
						*(uint16_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(uint16_t*)(&read_buffer[(num_Page*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_s32)
						*(int32_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(int32_t*)(&read_buffer[(num_Page*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_u32)
						*(uint32_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(uint32_t*)(&read_buffer[(num_Page*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_float)
						*(float*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(float*)(&read_buffer[(num_Page*6+num_Hang)]);
				}
			}
		}

		//读一波数据
		EEPROM_Read(0,(PAGE_ADDR_UI_Data_Start + page_Jia),(uint32_t*)read_buffer,128);
		page_Jia++;

		//读出
		for(num_Page = 5; num_Page < 10; num_Page++)
		{
			for(num_Hang = 0; num_Hang < 6; num_Hang++)
			{
				if(PC_data_pointer[num_yPage][num_Page][num_Hang] != NULL)
				{
					if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_s8)
						*(int8_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(int8_t*)(&read_buffer[((num_Page-5)*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_u8)
						*(uint8_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(uint8_t*)(&read_buffer[((num_Page-5)*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_s16)
						*(int16_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(int16_t*)(&read_buffer[((num_Page-5)*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_u16)
						*(uint16_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(uint16_t*)(&read_buffer[((num_Page-5)*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_s32)
						*(int32_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(int32_t*)(&read_buffer[((num_Page-5)*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_u32)
						*(uint32_t*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(uint32_t*)(&read_buffer[((num_Page-5)*6+num_Hang)]);
					else if(PC_data_type[num_yPage][num_Page][num_Hang] == data_type_float)
						*(float*)PC_data_pointer[num_yPage][num_Page][num_Hang] = *(float*)(&read_buffer[((num_Page-5)*6+num_Hang)]);
				}
			}
		}
	}
}

/*
 *	UI数据写入
 */
void UI_Data_Write(void)
{
	uint32_t write_buffer[32];
	uint32_t page_Jia = 0;

	uint8_t num_yPage, num_Page, num_Hang;

	for(num_yPage = 0; num_yPage < UI_YPage_num_max; num_yPage++)
	{
		for(num_Page = 0; num_Page < 5; num_Page++)
		{
			for(num_Hang = 0; num_Hang < 6; num_Hang++)
			{
				if(PC_data_pointer[num_yPage][num_Page][num_Hang] != NULL)
				{
					write_buffer[(num_Page*6+num_Hang)] = *(uint32_t*)(PC_data_pointer[num_yPage][num_Page][num_Hang]);
				}
			}
		}
		EEPROM_Write(0,(PAGE_ADDR_UI_Data_Start + page_Jia),(uint32_t*)write_buffer,128);
		page_Jia++;

		for(num_Page = 5; num_Page < 10; num_Page++)
		{
			for(num_Hang = 0; num_Hang < 6; num_Hang++)
			{
				if(PC_data_pointer[num_yPage][num_Page][num_Hang] != NULL)
				{
					write_buffer[((num_Page-5)*6+num_Hang)] = *(uint32_t*)(PC_data_pointer[num_yPage][num_Page][num_Hang]);
				}
			}
		}
		EEPROM_Write(0,(PAGE_ADDR_UI_Data_Start + page_Jia),(uint32_t*)write_buffer,128);
		page_Jia++;
	}
}

/*
 *	UI数据复位
 */
void UI_Data_Rest(void)
{
/**********   XXXXXXXXXXXXXXXXX   **********/

	//ADC标定数据
	elec_data.Demarcate_Rate_All = 1.0f;

	//路径计算参数
	f_track_max = 380;

	Count_Smoothness_threshold_M = 		1130;
	Count_Smoothness_M  = 		200;
	Count_Smoothness_Z  = 		200;

	//方向PID
	direction_pid.Pmax =					0.062f;
	direction_pid.Pmin =					0.055f;
	direction_pid.Dmax =					3.75f;
	direction_pid.Dmin =					3.25f;
	direction_pid.Speed_max =				4.0f;
	direction_pid.Speed_min =				0.8f;

	//舵机参数
	Steering_Max_Angle =				25.0f;
	Steering_Mid =						6.0f;
	Steering_Conversion_L = 			1.0f;
	Steering_Conversion_R = 			1.0f;
	Output_Angle_Force = 				0.0f;

	//
	unit_lose_data.into_S_Sub				= 		300;
	unit_lose_data.into_threshold_Square		= 		1400;
	unit_lose_data.quit_centre_Bias				= 		-200;
	unit_lose_data.quit_threshold_Square		= 		1420;

/**********   XXXXXXXXXXXXXXXXX   **********/

	//
	unit_cross_data.into_threshold_H = 1800;
	unit_cross_data.quit_threshold_H = 1780;
	unit_cross_data.into_centre_Bias_J = 700;
	unit_cross_data.into_Safety_Distance = 1.0f;

	//
	unit_ramp_data.into_quit_V_Sum =					4200;
	unit_ramp_data.into_quit_J_Sum =					4500;
	unit_ramp_data.into_quit_Z_Z =						3000;
	unit_ramp_data.into_Safety_Distance =				3.0f;
	unit_ramp_data.quit_Safety_Distance =				3.0f;
	
	//
	unit_annulus_data.into_denominator_min_error =			500;
	unit_annulus_data.into_min_error =						400;
	unit_annulus_data.into_Sum_J_threshold =				6600;
	unit_annulus_data.into_Sum_ZZ_threshold =				1500;
	unit_annulus_data.into_lose_threshold_Square =			600;
	unit_annulus_data.into_Maintain_Distance =				0.3f;						

	//
	unit_annulus_data.quit_Maintain_Distance =					0.6f;
	unit_annulus_data.quit_lose_threshold_Square =			620;
	unit_annulus_data.quit_Square_threshold =			2500;
	unit_annulus_data.quit_Way_Limit =						100;
	unit_annulus_data.into_Safety_Distance =				2.0f;

	//Temp
	DC_temp0 =							0.0f;
	DC_temp1 =							0.0f;
	DC_temp2 =							0.0f;
	DC_temp3 =							0.0f;
	DC_temp4 =							0.0f;
	DC_temp5 =							0.0f;

/**********   XXXXXXXXXXXXXXXXX   **********/

	//速度PID / 加速度PID
	speed_pid.P =						4.0f;
	speed_pid.I =						0.0f;
	speed_pid.D =						0.0f;
	acc_pid.P =							0.0f;
	acc_pid.I =							0.0f;
	acc_pid.D =							0.0f;

	//力矩参数
	f_Parameter_Tf = 					0.0f;
	f_Parameter_Ta = 					0.0f;
	f_Parameter_N =						0.0f;
	f_Parameter_T =						0.0f;
	f_Parameter_V_to_Speed =			0.3707f;
	f_Parameter_V_to_Acc =				0.643776f;
	//
	Motor_PWM_HZ = 						15000;
	Motor_Max_Voltage =					4.0f;
	f_Start_Voltage =					0.3041f;

	Output_V_Force = 					0.0f;

	//
	f_Set_Max_Output_Speed =			4.0f;
	f_Set_Max_Speed =					1.0f;
	f_Set_Min_Speed =					1.0f;
	f_float_Max_Speed =					1.0f;
			
	//
	f_Set_Max_Output_Acc =				3.0f;
	f_Set_Max_Acc =						1.0f;
	f_Set_Min_Acc =						1.0f;
	f_float_Max_Acc =					1.0f;
	
/**********   XXXXXXXXXXXXXXXXX   **********/

	//
	f_Max_Friction_To_max_micro =		1.70f;

	//
	f_temp_max_Speed_Annulus =			2.0f;
	f_temp_max_Speed_Ramp =				3.0f;

	//

	//

	//

	//Temp
	SC_temp0 =							0.0f;
	SC_temp1 =							0.0f;
	SC_temp2 =							0.0f;
	SC_temp3 =							0.0f;
	SC_temp4 =							0.0f;
	SC_temp5 =							0.0f;

/**********   XXXXXXXXXXXXXXXXX   **********/
	//
	ANO_Debug_mode =					1;
	Car_BiBi_En =						1;
	Car_AorB =							1;
	Power_cs =							0.0106f;

	//
	elec_data.Demarcate_Rate_V_LL =		3.222f;
	elec_data.Demarcate_Rate_V_RR =		3.222f;
	elec_data.Demarcate_Rate_H_L =		3.222f;
	elec_data.Demarcate_Rate_H_R =		3.222f;

	//
	elec_data.Demarcate_Rate_J_L =		3.222f;
	elec_data.Demarcate_Rate_J_R =		3.222f;
	elec_data.Demarcate_Rate_Z_Z =		3.222f;
	elec_data.Demarcate_Rate_Z_H =		3.222f;

	//

	//

	//

/**********   XXXXXXXXXXXXXXXXX   **********/
	//

	//

	//

	//

	//
	
}

/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/*****************************************  NULL_OLED  **********************************************/
/****************************************************************************************************/
/*
 *	NULL_OLED
 */
void UI_NULL_OLED_TX(void)
{
	if((Key_Inquire_data(Key_Up_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Up_Read) == Key_bit_Acc))
		UI_Kind_Catalog++;
	if((Key_Inquire_data(Key_Down_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Down_Read) == Key_bit_Acc))
		UI_Kind_Catalog--;

	if(UI_time_dida)
	{
		Cache_OLED_P6x8Num(0,0,UI_Kind_Catalog);
		Cache_OLED_P6x8Num(64,0,UI_PC_catalog);
		Cache_OLED_P6x8Str(0,3,"NO UI_Kind_Catalog!!!");
		Cache_Update_OLED();
	}
	else
		Cache_Update_OLED();	
}
/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/


/****************************************************************************************************/
/****************************************************************************************************/
/********************************************  综合  ************************************************/
/****************************************************************************************************/
/*
 *	UI初始化
 */
#define Version_Number		0x12345678	//版本号
void UI_OLED_Init(void)
{
	uint32_t check_buffer[32];

	//清屏
	OLED_CLS();

	//参数配置页操作初始化
	UI_UI_Parameter_Configuration_Init();
	
	//读取版本号
	EEPROM_Read(0,Version_PAGE_ADDR,(uint32_t*)check_buffer,128);
	if(check_buffer[0] == Version_Number)
	{//版本号符合---直接读EEPROM数据到内存
		//从EEPROM读取数据
		UI_Data_Read();
		printf("Car_Reserve_Read    OK!\r\n");
		printf("Version_Number  :  0x%x\r\n",check_buffer[0]);
	}
	else
	{//版本号不符合---往EEPROM写默认数据
		check_buffer[0] = Version_Number;
		//更新版本号
		EEPROM_Write(0,Version_PAGE_ADDR,(uint32_t*)check_buffer,128);
		//复位数据
		UI_Data_Rest();
		//写入EEPROM
		UI_Data_Write();
		printf("Car_Reserve_Rest    OK!\r\n");
		printf("Version_Number  :  0x%x\r\n",check_buffer[0]);
	}
}

/*
 *	UI扫描
 *	参数：nop
 */
void UI_OLED_play(uint16_t time)
{
	if(Car_State != Car_State_Outage)
	{//不在断电态--->强制设为Go界面
		OLED_CLS();		//熄屏
	 	UI_Kind_Catalog = UI_Kind_catalog_Go;
	}

	switch(UI_Kind_Catalog)
	{
		case UI_Kind_catalog_Go:
		{
			UI_Car_Go(time);
		}
		break;
		case UI_Kind_catalog_Order:
		{
			Cache_OLED_P6x8Str(0,0,"Order:");
			Cache_OLED_P6x8Num(36,0,UI_Kind_Catalog);
			Cache_OLED_P6x8Str(64,0,"Page:");
			Cache_OLED_P6x8Num(96,0,UI_Order_catalog);
			UI_Order();
			Cache_Update_OLED();
			}
		break;
		case UI_Kind_catalog_Show_1:
		{
			Cache_OLED_P6x8Str(0,0,"Show:");
			Cache_OLED_P6x8Num(32,0,UI_Kind_Catalog);
			Cache_OLED_P6x8Str(64,0,"Page:");
			Cache_OLED_P6x8Num(96,0,UI_Show_1_catalog);
			UI_Show_1();
			Cache_Update_OLED();
		}
		break;
		case UI_Kind_catalog_Show_2:
		{
			Cache_OLED_P6x8Str(0,0,"Show:");
			Cache_OLED_P6x8Num(32,0,UI_Kind_Catalog);
			Cache_OLED_P6x8Str(64,0,"Page:");
			Cache_OLED_P6x8Num(96,0,UI_Show_2_catalog);
			UI_Show_2();
			Cache_Update_OLED();
		}
		break;
		case UI_Kind_catalog_Show_3:
		{
			Cache_OLED_P6x8Str(0,0,"Show:");
			Cache_OLED_P6x8Num(32,0,UI_Kind_Catalog);
			Cache_OLED_P6x8Str(64,0,"Page:");
			Cache_OLED_P6x8Num(96,0,UI_Show_3_catalog);
			UI_Show_3();
			Cache_Update_OLED();
		}
		break;
		case UI_Kind_catalog_Show_Annulus_50cm:
		{
			Cache_OLED_P6x8Str(0,0,"Show:");
			Cache_OLED_P6x8Num(32,0,UI_Kind_Catalog);
			Cache_OLED_P6x8Str(64,0,"Page:");
			Cache_OLED_P6x8Num(96,0,UI_Show_Annulus_50cm_catalog);
			UI_Show_Annulus_50cm();
			Cache_Update_OLED();
		}
		break;
		case UI_Kind_catalog_Show_Annulus_100cm:
		{
			Cache_OLED_P6x8Str(0,0,"Show:");
			Cache_OLED_P6x8Num(32,0,UI_Kind_Catalog);
			Cache_OLED_P6x8Str(64,0,"Page:");
			Cache_OLED_P6x8Num(96,0,UI_Show_Annulus_100cm_catalog);
			UI_Show_Annulus_100cm();
			Cache_Update_OLED();
		}
		break;
		case UI_Kind_catalog_Show_Annulus_200cm:
		{
			Cache_OLED_P6x8Str(0,0,"Show:");
			Cache_OLED_P6x8Num(32,0,UI_Kind_Catalog);
			Cache_OLED_P6x8Str(64,0,"Page:");
			Cache_OLED_P6x8Num(96,0,UI_Show_Annulus_200cm_catalog);
			UI_Show_Annulus_200cm();
			Cache_Update_OLED();
		}
		break;
		case UI_Kind_catalog_PC:
		{
			//因为要显示锁定状态---所以压缩空间
			Cache_OLED_P6x8Str(0,0,"PC:");
			Cache_OLED_P6x8Num(18,0,UI_PC_Ycatalog);
			Cache_OLED_P6x8Str(36,0,"Page:");
			Cache_OLED_P6x8Num(66,0,UI_PC_catalog);
			UI_Parameter_Configuration();
			Cache_Update_OLED();
		}
		break;
		default:
		{//非法页面
			UI_NULL_OLED_TX();
		}
		break;
	}

	//命令界面禁止赛道决策
	if(UI_Kind_Catalog == UI_Kind_catalog_Order)
		Track_Arbiter_Allow = 0;
	else
		Track_Arbiter_Allow = 1;

	UI_time += time;
	if(UI_time >= 500)
	{
		UI_time = 0;
		UI_time_dida = !UI_time_dida;
	}
}

#endif
