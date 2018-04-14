#if 0
#include <stdio.h>
#include "include.h"
#include "debug.h"

uint8_t Site_comfirm = 0;	//UI光标锁定
int8_t Site_X = 0;			//UI光标位置(1~7)
int8_t Site_Y = 6;			//UI光标位置(1~-)

uint8_t Park_or_Lock = 2;	//NO_0		Lock_1		Park_2
uint8_t Write_Buffer_mode = 0;	//Stop_0	ANO_1	CCD_2
uint8_t Lock_remove_signal = 0;		//延时发车
uint8_t S_S = 0;	//呼吸

//					0		1			2		3		4		5		6		7		8		9
float Step[10] = { 0.000001,	0.000001, 0.00001,	0.0001,	0.001,	0.01,	0.1,	1.0,	10.0,	100.0};   //默认调节步长为0.01
UI_Units_TypeDef UI_Units_Structure[6][8];	//变量链接表

void UI_Init(void)
{
	//页0	车参数	(char) *****************************************************************
	UI_Units_Structure[0][0].Add.add_u8 = (uint8_t*)&g_ucCarMode;	//行车模式
	UI_Units_Structure[0][0].Step = 7;
	strcpy(UI_Units_Structure[0][0].Name, "g_ucCarMode:");

	UI_Units_Structure[0][1].Add.add_u8 = (uint8_t*)&Park_or_Lock;	//停车方式
	UI_Units_Structure[0][1].Step = 7;
	strcpy(UI_Units_Structure[0][1].Name, "Park_or_Lock:");

	UI_Units_Structure[0][2].Add.add_u8 = (uint8_t*)&Write_Buffer_mode;	//串口模式
	UI_Units_Structure[0][2].Step = 7;
	strcpy(UI_Units_Structure[0][2].Name, "Buff_Mode:");
	
	//页1	车参数	(char) *****************************************************************
	UI_Units_Structure[1][0].Add.add_u8 = (uint8_t*)&Midline_slope_Calculate_Length;	//斜率计算数据量
	UI_Units_Structure[1][0].Step = 7;
	strcpy(UI_Units_Structure[1][0].Name, "MidSlopeCalc:");

	//页2	车参数	(long) *****************************************************************
	UI_Units_Structure[2][0].Add.add_u16 = (uint16_t*)&PWM_Mid;	//舵机中值
	UI_Units_Structure[2][0].Step = 8;
	strcpy(UI_Units_Structure[2][0].Name, "PWM_Mid:");

	//页3	车参数	(float) *****************************************************************
	UI_Units_Structure[3][1].Add.add_float = (float*)&g_fCarSpeedSetMax;	//直道高速
	UI_Units_Structure[3][1].Step = 8;
	strcpy(UI_Units_Structure[3][1].Name, "SpeedSetMax:");
	
	UI_Units_Structure[3][2].Add.add_float = (float*)&g_fCarSpeedSetCurveMax;	//弯道高速
	UI_Units_Structure[3][2].Step = 8;
	strcpy(UI_Units_Structure[3][2].Name, "SpSetCurMax:");

	//页4	速度方向PID	(float) *****************************************************************
	UI_Units_Structure[4][0].Add.add_float = (float*)&SPEED_CONTROL_P;	//直道速度P
	UI_Units_Structure[4][0].Step = 2;
	strcpy(UI_Units_Structure[4][0].Name,"Speed-P:");

	UI_Units_Structure[4][1].Add.add_float = (float*)&SPEED_CONTROL_I;	//直道速度I
	UI_Units_Structure[4][1].Step = 2;
	strcpy(UI_Units_Structure[4][1].Name, "Speed-I:");

	UI_Units_Structure[4][2].Add.add_float = (float*)&SPEED_CONTROL_Curve_P;	//弯道速度P
	UI_Units_Structure[4][2].Step = 2;
	strcpy(UI_Units_Structure[4][2].Name, "SpeedCurve-P:");

	UI_Units_Structure[4][3].Add.add_float = (float*)&SPEED_CONTROL_Curve_I;	//弯道速度I
	UI_Units_Structure[4][3].Step = 2;
	strcpy(UI_Units_Structure[4][3].Name, "SpeedCurve-I:");

	UI_Units_Structure[4][4].Add.add_float = (float*)&DIR_CONTROL_P;	//方向P
	UI_Units_Structure[4][4].Step = 2;
	strcpy(UI_Units_Structure[4][4].Name, "DirControl-P:");

	UI_Units_Structure[4][5].Add.add_float = (float*)&DIR_CONTROL_D;	//方向D
	UI_Units_Structure[4][5].Step = 2;
	strcpy(UI_Units_Structure[4][5].Name, "DirControl-D:");

	UI_Units_Structure[4][6].Add.add_float = (float*)&DIR_CONTROL_DIR_Gain_P;	//方向P
	UI_Units_Structure[4][6].Step = 2;
	strcpy(UI_Units_Structure[4][6].Name, "DirConGain-P:");

	UI_Units_Structure[4][7].Add.add_float = (float*)&DIR_CONTROL_DIR_Gain_D;	//方向D
	UI_Units_Structure[4][7].Step = 2;
	strcpy(UI_Units_Structure[4][7].Name, "DirConGain-D:");

	//页5	速度方向PID	(float) *****************************************************************
	UI_Units_Structure[5][0].Add.add_float = (float*)&DIR_Fuzzy_P;				//模糊P
	UI_Units_Structure[5][0].Step = 0;
	strcpy(UI_Units_Structure[5][0].Name, "DIR_Fuzzy_P:");

	UI_Units_Structure[5][1].Add.add_float = (float*)&DIR_Fuzzy_D;				//模糊D
	UI_Units_Structure[5][1].Step = 0;
	strcpy(UI_Units_Structure[5][1].Name, "DIR_Fuzzy_D:");

	UI_Units_Structure[5][2].Add.add_float = (float*)&Speed_Different_Ratio_Constant;	//差速常量
	UI_Units_Structure[5][2].Step = 5;
	strcpy(UI_Units_Structure[5][2].Name, "SpeDiffRatio:");

	UI_Units_Structure[5][3].Add.add_float = (float*)&DIR_Fuzzy_P;				//模糊P
	UI_Units_Structure[5][3].Step = 0;
	strcpy(UI_Units_Structure[5][3].Name, "DIR_Fuzzy_P:");

	UI_Units_Structure[5][4].Add.add_float = (float*)&DIR_Fuzzy_P;				//模糊P
	UI_Units_Structure[5][4].Step = 0;
	strcpy(UI_Units_Structure[5][4].Name, "DIR_Fuzzy_P:");

	UI_Units_Structure[5][5].Add.add_float = (float*)&DIR_Fuzzy_P;				//模糊P
	UI_Units_Structure[5][5].Step = 0;
	strcpy(UI_Units_Structure[5][5].Name, "DIR_Fuzzy_P:");
}

void UI_Display()
{
	uint8_t x;

	switch (Site_Y)
	{
	case 0:	
		for (x = 0;x < 8;x++)
		{
			if (x == Site_X)//扫描到光标处
			{
				if (Site_comfirm)//光标锁定
				{
					OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
					OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8,1);
				}
				else//光标未锁定
				{
					if (S_S) {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8,1);
					}
					else {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8,0);
					}
				}
			}
			else//
			{
				OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
				OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8,0);
			}
		}
		break;
	case 1:
		for (x = 0;x < 8;x++)
		{
			if (x == Site_X)//扫描到光标处
			{
				if (Site_comfirm)//光标锁定
				{
					OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
					OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8, 1);
				}
				else//光标未锁定
				{
					if (S_S) {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8, 1);
					}
					else {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8, 0);
					}
				}
			}
			else//
			{
				OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
				OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u8, 8, 0);
			}
		}
		break;
	case 2:
		for (x = 0;x < 8;x++)
		{
			if (x == Site_X)//扫描到光标处
			{
				if (Site_comfirm)//光标锁定
				{
					OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
					OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u16, 8, 1);
				}
				else//光标未锁定
				{
					if (S_S) {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u16, 8, 1);
					}
					else {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u16, 8, 0);
					}
				}
			}
			else//
			{
				OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
				OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_u16, 8, 0);
			}
		}
		break;
	case 3:
		for (x = 0;x < 8;x++)
		{
			if (x == Site_X)//扫描到光标处
			{
				if (Site_comfirm)//光标锁定
				{
					OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
					OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_float, 8,1);
				}
				else//光标未锁定
				{
					if (S_S) {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_float, 8,1);
					}
					else {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
						OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_float, 8,0);
					}
				}
			}
			else//
			{
				OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
				OLED_P6x8Num(78, x, (int32_t)*UI_Units_Structure[Site_Y][x].Add.add_float, 8,0);
			}
		}
		break;
	case 4:
		for (x = 0;x < 8;x++)
		{
			if (x == Site_X)//扫描到光标处
			{
				if (Site_comfirm)//光标锁定
				{
					OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
					OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 1000000), 8, 1);
				}
				else//光标未锁定
				{
					if (S_S) {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
						OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 1000000), 8, 1);
					}
					else {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
						OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 1000000), 8, 0);
					}
				}
			}
			else//
			{
				OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
				OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 1000000), 8, 0);
			}
		}
		break;
	case 5:
		for (x = 0;x < 8;x++)
		{
			if (x == Site_X)//扫描到光标处
			{
				if (Site_comfirm)//光标锁定
				{
					OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
					OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 10000000), 8, 1);
				}
				else//光标未锁定
				{
					if (S_S) {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 1);
						OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 10000000), 8, 1);
					}
					else {
						OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
						OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 10000000), 8, 0);
					}
				}
			}
			else//
			{
				OLED_P6x8Str(0, x, (uint8_t*)UI_Units_Structure[Site_Y][x].Name, 0);
				OLED_P6x8Num(78, x, (int32_t)((*UI_Units_Structure[Site_Y][x].Add.add_float) * 10000000), 8, 0);
			}
		}
		break;
	case 6:
		OLED_P6x8Str(0, 0, "CarLock:", 0);				//车锁——4位
		OLED_P6x8Num(60, 0, (int32_t)g_ucCarLock, 4,0);
		OLED_P6x8Str(0, 1,  "Park_Flag:", 0);			//停车标志
		OLED_P6x8Num(60, 1, (int32_t)guc_Park_Flag, 4,0);
		OLED_P6x8Str(0, 2,	"Power:", 0);				//电压——4位
		OLED_P6x8Num(60, 2, (int32_t)(Power_ADC * 100), 4,0);
		OLED_P6x8Str(0, 3, "Speed_Set:", 0);				//电压——4位
		OLED_P6x8Num(60, 3, (int32_t)g_fCarSpeedSet, 4,0);
		OLED_P6x8Str(0, 5,	"Journey:", 0);				//路程——4位
		OLED_P6x8Num(60, 5, (int32_t)g_ilJourney, 6,0);
		break;
	case 7:
		OLED_P6x8Str(0, 0, "L_Mod:", 0);			//巡线模式
		OLED_P6x8Str(0, 1, "W_Mod:", 0);			//赛道模式
		OLED_P6x8Str(0, 2, "Ha_Wa:", 0);			//赛道宽
		OLED_P6x8Str(64, 0, "Mid_L:", 0);			//中线位置(矫正)
		OLED_P6x8Str(64, 1, "Midli:", 0);			//中线位置
		OLED_P6x8Str(64, 2, "M_Set:", 0);			//设定中线位置
		OLED_P6x8Str(0, 3, "In_In:", 0);			//平均灰度值
		OLED_P6x8Str(64, 3, "St_Ou:", 0);			//平均灰度值
		OLED_P6x8Num(36, 0, (int32_t)g_ucSeekLineMode1, 4,0);
		OLED_P6x8Num(36, 1, (int32_t)g_ucWayMode1, 4,0);
		OLED_P6x8Num(36, 2, (int32_t)g_uiHalfWay1, 4,0);
		OLED_P6x8Num(100, 0, (int32_t)gi_Midline_Correct1, 4,0);
		OLED_P6x8Num(100, 1, (int32_t)gi_Midline1, 4,0);
		OLED_P6x8Num(100, 2, (int32_t)gi_Midline_Set, 4,0);
		OLED_P6x8Num(36, 3, (int32_t)l_uiIntensity1, 4, 0);
		OLED_P6x8Num(100, 3, (int32_t)(gf_Steering_Out * 1000.0f), 4, 0);
		OLED_Draw_Position(4);

		extern uint8_t CCDImage1[128];
		extern uint8_t CCDImage_Normalization[128];
		
		OLED_Draw_BMP(CCDImage1, 5);
		OLED_Draw_BMP(CCDImage_Normalization, 6);
		break;
	case 8:
		break;
        default:
			break;
	}
}

void UI_Adjust(uint8_t Record)
{
#define UI_Key_OK (Record & (0x01 << 0))
#define UI_Key_Up (Record & (0x01 << 1))
#define UI_Key_Do (Record & (0x01 << 2))
#define UI_Key_Le (Record & (0x01 << 3))
#define UI_Key_Ri (Record & (0x01 << 4))

extern uint8_t Lock_remove_signal;

	if (UI_Key_OK)
	{
		if(Site_Y < 6)
			Site_comfirm = ~Site_comfirm;
		else
		{
			if (g_ucCarLock)
			{
				Lock_remove_signal = 1;
			}
			else
			{
				g_ucCarLock = 1;
				Lock_remove_signal = 0;
			}
		}
	}
		

	if (Site_comfirm)//锁定位调参
	{
		switch (Site_Y)
		{
		case 0:
			if (UI_Key_Up)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) += Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Do)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) -= Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Le)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) += Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			if (UI_Key_Ri)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) -= Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			break;
		case 1:
			if (UI_Key_Up)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) += Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Do)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) -= Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Le)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) += Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			if (UI_Key_Ri)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u8) -= Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			break;
		case 2:
			if (UI_Key_Up)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u16) += Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Do)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u16) -= Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Le)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u16) += Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			if (UI_Key_Ri)(*UI_Units_Structure[Site_Y][Site_X].Add.add_u16) -= Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			break;
		case 3:
			if (UI_Key_Up)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) += Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Do)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) -= Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Le)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) += Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			if (UI_Key_Ri)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) -= Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			break;
		case 4:
			if (UI_Key_Up)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) += Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Do)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) -= Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Le)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) += Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			if (UI_Key_Ri)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) -= Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			break;
		case 5:
			if (UI_Key_Up)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) += Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Do)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) -= Step[UI_Units_Structure[Site_Y][Site_X].Step];
			if (UI_Key_Le)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) += Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			if (UI_Key_Ri)(*UI_Units_Structure[Site_Y][Site_X].Add.add_float) -= Step[UI_Units_Structure[Site_Y][Site_X].Step] * 10.0f;
			break;
		default:
			break;
		}
	}
	else//非锁定为移动光标
	{
		if (UI_Key_Up)if (Site_X > 0)Site_X--;
		if (UI_Key_Do)if (Site_X < 5) Site_X++;
		if (UI_Key_Le)if (Site_Y > 0){ Site_Y--;OLED_CLS();}
		if (UI_Key_Ri)if (Site_Y < 7){ Site_Y++;OLED_CLS();}
	}
}


#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //尽量用最后面的扇区，确保安全

uint8 Store_Data(void)
{
	return(flash_write_buf(SECTOR_NUM, 0, sizeof(UI_Units_Structure), (uint8*)UI_Units_Structure));
}

#endif
