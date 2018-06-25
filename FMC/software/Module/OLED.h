/*********************************************************************************************************************
					------------------------------------ 
					    OLED液晶    单片机                        
    					D0          I0
    					D1          I1            
    					RES         H0    
    					DC          H1
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _OELD_H
#define _OELD_H

#include "common.h"


//----宏定义OLED引脚----	 
#define  OLED_SCL_PIN	H3
#define  OLED_SDA_PIN	H4
#define  OLED_RST_PIN	C0
#define  OLED_DC_PIN	C1
#define  OLED_CS_PIN	I5

//定义OLED使用的标准
typedef unsigned          char O_byte;
typedef unsigned short     int O_word;

static void OLED_WriteData(O_byte data);        //功能描述：    OLED_写 1Bit 数据
static void OLED_WriteCmd(O_byte cmd);          //功能描述：    OLED_写 1Bit 命令
static void OLED_Set_Pos(O_byte x, O_byte y);           //功能描述：    设置起始点
static void OLED_DLY_ms(O_word ms);            //功能描述：    OLED_延时(大致)

void OLED_Init(void);//功能描述：    OLED_初始化
void OLED_CLS_Y(O_byte y);
void OLED_CLS(void);            //功能描述：    清屏  
void OLED_Fill(O_byte bmp_data);                //功能描述：    填满OLED   参数：   0xff 黑屏    0x00 亮屏


void OLED_PutPixel(O_byte x,O_byte y);

void OLED_Rectangle(O_byte x1,O_byte y1,O_byte x2,O_byte y2,O_byte gif);

void OLED_P6x8Num(O_byte x, O_byte y, int32_t Num);
void OLED_P6x8floatNum(O_byte x,O_byte y,float Num);
void OLED_P8x16Num(O_byte x,O_byte y,int32_t Num,uint8_t num);
void OLED_P8x16floatNum(O_byte x,O_byte y,float Num,uint8_t num);

void OLED_P6x8Str(O_byte x,O_byte y,char ch[]);//,uint8_t neg);
void OLED_P6x8byte_inverse(O_byte x,O_byte y,char ch);

void OLED_P8x16Str(O_byte x,O_byte y,char ch[]);

void OLED_P14x16Str(O_byte x,O_byte y,O_byte ch[]);

void OLED_Print(O_byte x, O_byte y, O_byte ch[]);

void Draw_BMP(O_byte x0,O_byte y0,O_byte x1,O_byte y1,char bmp[]);

void Draw_LOGO(void);

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

//定位
void Cache_OLED_Set_Pos(uint8_t x,uint8_t y);

//往缓冲区写数据
void Cache_OLED_WriteData(uint8_t data);

//
void Cache_OLED_P6x8Str(uint8_t x,uint8_t y,char ch[]);

//
void Cache_OLED_P6x8byte_inverse(uint8_t x,uint8_t y,char ch);

//
void Cache_OLED_P6x8Num(uint8_t x,uint8_t y,int32_t Num);

//写数0x型
void Cache_OLED_P6x8Num_0X(uint8_t x,uint8_t y,int32_t Num_0X);

//写浮点数
void Cache_OLED_P6x8floatNum(O_byte x,O_byte y,float Num);

//
void Cache_OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
signed int Cache_OLED_printf(uint8_t x,uint8_t y,const char *pFormat, ...);

//缓冲区数据推往OLED
void Cache_Update_OLED(void);
/*******************************************************************/

uint8_t gety(void );
uint8_t getx(void );





/********************************************************************/

#endif
