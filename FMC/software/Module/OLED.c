#include "OLED.h"
#include "KEA128_gpio.h"
#include "KEA128_port.h"
#include "stdarg.h"
//#include "stdio.h"


#define XLevelL		0x00		//水平最低行
#define XLevelH		0x10		//水平最高行
#define XLevel		((XLevelH&0x0F)*16+XLevelL)		//水平行定义
#define Max_Column	128			 //最大水平像素点
#define Max_Row		64				//最大竖直像素点
#define	Brightness	0xCF		//屏幕亮度


#define X_WIDTH 128				//屏幕宽度
#define Y_WIDTH 64				//屏幕高度
	 

 

#define OLED_SCL(x)  gpio_set (OLED_SCL_PIN	, x)
#define OLED_SDA(x)  gpio_set (OLED_SDA_PIN	, x)
#define OLED_RST(x)  gpio_set (OLED_RST_PIN	, x)
#define OLED_DC(x)   gpio_set (OLED_DC_PIN	, x)
#define OLED_CS(x)   gpio_set (OLED_CS_PIN  , x)
                                       
//======================================
/*              字库
//      F6*8            6*8字体库
//      F14*16_Idx      汉字索引文件（Index file）
//      F14*16          汉字库
//      F8*16           8*16字体库
//      longqiu96*64    龙邱的大LOGO_有空改掉
//      LIBLOGO60*58    龙邱的LOGO)_有空改掉
//      LOGO[1024]      协会LOGO
*/

const uint8 F6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

//      F14*16_Idx      汉字索引文件（Index file）
const uint8 F14x16_Idx[] = 
{
	"安徽工程大学"
};

//      F14*16          汉字库
const uint8 F14x16[] = {  
	0x00,0x00,0xB0,0x90,0x90,0xD0,0x92,0x8C,0x90,0x10,0x90,0x90,0x80,0x00,0x00,0x80,
	0x80,0x40,0x46,0x49,0x28,0x10,0x1C,0x13,0x20,0x60,0x00,0x00,/*"安",0*/

	0x00,0x08,0xE4,0x12,0x9C,0x50,0x5E,0xB0,0x9C,0xFC,0x22,0xE0,0x10,0x00,0x00,0x01,
	0x7F,0x20,0x3D,0x43,0xBD,0x72,0x40,0x27,0x1C,0x23,0x40,0x00,/*"徽",1*/

	0x00,0x00,0x08,0x08,0x08,0x08,0xF8,0x08,0x08,0x08,0x08,0x0C,0x08,0x00,0x00,0x00,
	0x20,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x20,0x20,0x20,0x00,/*"工",2*/

	0x00,0x20,0x24,0xFC,0xA4,0x20,0x00,0x3C,0x24,0x24,0x24,0x7C,0x84,0x00,0x00,0x08,
	0x07,0x7F,0x00,0x43,0x41,0x49,0x49,0x7F,0x49,0x49,0x40,0x00,/*"程",3*/

	0x00,0x20,0x20,0x20,0x20,0x20,0xFE,0xA2,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x80,
	0x40,0x20,0x10,0x0C,0x03,0x01,0x06,0x08,0x30,0x60,0x40,0x00,/*"大",4*/

	0x00,0x80,0x60,0xA4,0xB8,0xA0,0xA4,0xA8,0xA0,0xB8,0x26,0x60,0x20,0x00,0x00,0x00,
	0x04,0x04,0x04,0x44,0x44,0x3E,0x05,0x04,0x04,0x04,0x04,0x00,/*"学",5*/
};

//      F8*16           8*16字体库
// 描  述: [8X16]西文字符的字模数据 (纵向取模,字节倒序)
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
const uint8 F8X16[]=
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
  0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//!1
  0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//"2
  0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//#3
  0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$4
  0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//%5
  0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//&6
  0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//'7
  0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//(8
  0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//)9
  0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//*10
  0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+11
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//,12
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//-13
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//.14
  0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,///15
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//016
  0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//117
  0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//218
  0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//319
  0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//420
  0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//521
  0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//622
  0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//723
  0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//824
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//925
  0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//:26
  0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//;27
  0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//<28
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//=29
  0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//>30
  0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//?31
  0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@32
  0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A33
  0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B34
  0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C35
  0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D36
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E37
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F38
  0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G39
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H40
  0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I41
  0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J42
  0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K43
  0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L44
  0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M45
  0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N46
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O47
  0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P48
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q49
  0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R50
  0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S51
  0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T52
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U53
  0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V54
  0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W55
  0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X56
  0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y57
  0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z58
  0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[59
  0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\60
  0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//]61
  0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^62
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_63
  0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//`64
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a65
  0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b66
  0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c67
  0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d68
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e69
  0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f70
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g71
  0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h72
  0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i73
  0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j74
  0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k75
  0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l76
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m77
  0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n78
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o79
  0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p80
  0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q81
  0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r82
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s83
  0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t84
  0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u85
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v86
  0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w87
  0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x88
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y89
  0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z90
  0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{91
  0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//|92
  0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//}93
  0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~94

};


const char LOGO[1024] =
{  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0xFE,0x7F,0x1F,0xFF,0xFC,0xFC,0x0C,0xFC,
0xFC,0xFC,0x1C,0x00,0x06,0x86,0xC6,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x10,
0x10,0x30,0x30,0x70,0x40,0xFE,0xFE,0xFE,0x10,0x10,0x10,0xF0,0xFF,0xFF,0x1F,0x18,
0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x30,0x30,0x30,0x30,0xB0,0xF8,
0xFF,0x7F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xF8,0xF8,0xF8,0x18,0x18,0xF8,0xF8,0xF8,0x00,0x0C,0xCC,0xFC,0xFE,0x3E,
0x06,0xE6,0xFE,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0xE0,0xF8,0x7E,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0x63,0x61,
0x61,0xF0,0xB8,0x99,0x19,0x99,0xF9,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x04,0x84,0x86,0x82,0x83,0x9F,0x9F,0x9F,0x84,0xF4,0xF6,0xE7,0xC7,0xC7,0xC2,0xC2,
0x42,0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0x3E,0xFF,0xFF,
0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0xFF,0x0C,0x0C,0xFF,0xFF,0xFF,0xFF,0x00,0xFB,0xF9,0xF8,0x18,
0x19,0x1B,0xFB,0xFB,0xFB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xF8,0xFF,0x8F,0x87,0x83,0x83,0x83,0xC3,0xC3,0xC3,0xC3,0x01,0x01,0x00,0x00,
0x00,0x00,0x01,0x01,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0x0C,0x0C,
0x0C,0x0D,0x83,0xE7,0xFF,0x7F,0xFF,0xF3,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x02,
0x83,0xC1,0xE1,0x61,0x31,0xF9,0xFD,0xFF,0x0F,0x9F,0x3F,0x7C,0xF8,0xFC,0xDE,0x8E,
0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x06,0x03,0x01,0x00,0x00,0xFF,0xFF,
0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x87,0xE7,0x23,0x03,0x03,0xE3,0xE3,0x01,0x00,0xE3,0xE3,0xE3,0xC3,
0x13,0x33,0xF3,0xF3,0xE3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,
0xFE,0x1F,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x00,0x00,0x0F,0x0F,0x03,0x00,0x00,
0x02,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x06,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x0C,0x0C,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x03,0x03,0x07,
0x07,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,
0x00,0x00,0x00,0x00,0x08,0x18,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x02,0x07,0x07,0x00,0x00,0x00,0x03,0x03,0x03,0x00,0x00,0x03,0x03,0x03,
0x00,0x00,0x01,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x06,0x03,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
//======================================//


//==============================================================
//函数名：      void OLED_PutPixel(uint8 x,uint8 y)
//功能描述：    OLED_写 1Bit 数据
//参数：        写入的数据
//返回：        无
//调用：        内部调用
//==============================================================
void OLED_WriteData(O_byte data)
{
	O_byte i=8;
	
	OLED_DC(1);
	OLED_SCL(0);
	while(i--)
	{
	  	if(data&0x80){OLED_SDA(1);}
	  	else{OLED_SDA(0);}
	  	OLED_SCL(1);
		
	  	OLED_SCL(0);
	  	data<<=1;
	}
}

//内部使用用户无需调用
void OLED_WriteCmd(O_byte cmd)
{
	O_byte i=8;
	
	OLED_DC(0);
	OLED_SCL(0);
	
	while(i--)
	{
		if(cmd&0x80){OLED_SDA(1);}
		else{OLED_SDA(0);}
		OLED_SCL(1);
		
		OLED_SCL(0);
		cmd<<=1;
	}
  
}

//==============================================================
//函数名：      void OLED_Set_Pos(O_byte x, O_byte y)
//功能描述：    设置起始点
//参数：        坐标
//返回：        无
//调用：        内部调用
//==============================================================
void OLED_Set_Pos(O_byte x, O_byte y)
{ 
  OLED_WriteCmd(0xb0+y);
  OLED_WriteCmd(((x&0xf0)>>4)|0x10);
  OLED_WriteCmd((x&0x0f)|0x01); 
} 


//==============================================================
//函数名：      void OLED_Fill(O_byte bmp_data)
//功能描述：    填满OLED   
//参数：        0xff 黑屏    0x00 亮屏
//返回：        无
//调用：        内部调用/外部调用
//==============================================================
void OLED_Fill(O_byte bmp_data)
{
	O_byte y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_WriteCmd(0xb0+y);
		OLED_WriteCmd(0x01);
		OLED_WriteCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WriteData(bmp_data);
	}
}


//==============================================================
//函数名：      void OLED_CLS_Y(void)
//功能描述：    清屏  
//参数：        无
//返回：        无
//调用：        内部调用/外部调用
//==============================================================
void OLED_CLS_Y(O_byte y)
{
    O_byte x;
    OLED_WriteCmd(0xb0+y);
    OLED_WriteCmd(0x01);
    OLED_WriteCmd(0x10); 
    for(x=0;x<X_WIDTH;x++)
        OLED_WriteData(0);
}

//==============================================================
//函数名：      void OLED_CLS(void)
//功能描述：    清屏  
//参数：        无
//返回：        无
//调用：        内部调用/外部调用
//==============================================================
void OLED_CLS(void)
{
	O_byte y,x;	
	for(y=0;y<8;y++)
	{
		OLED_WriteCmd(0xb0+y);
		OLED_WriteCmd(0x01);
		OLED_WriteCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			OLED_WriteData(0);
	}
}


//==============================================================
//函数名：      void OLED_DLY_ms(O_word ms)
//功能描述：    OLED_延时(大致)
//参数：        ms
//返回：        无
//调用：        内部调用
//==============================================================
void OLED_DLY_ms(O_word ms)
{                         
  O_word a;
  while(ms)
  {
    a=13350;
    while(a--);
    ms--;
  }
  return;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED初始化函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Init(void)
{
	gpio_init (OLED_SCL_PIN, GPO,1);
  gpio_init (OLED_SDA_PIN, GPO,1);
  gpio_init (OLED_RST_PIN, GPO,1);
  gpio_init (OLED_DC_PIN , GPO,1);
	gpio_init (OLED_CS_PIN , GPO,0);

	port_pull (OLED_SCL_PIN);
	port_pull (OLED_SDA_PIN);
	port_pull (OLED_RST_PIN);
	port_pull (OLED_DC_PIN );


	OLED_SCL(1);
	//OLED_CS(0); //使能信号端，拉低时正常使用
	OLED_RST(0);
	OLED_DLY_ms(50);
	OLED_RST(1);
	
	OLED_WriteCmd(0xae);//--turn off oled panel
	OLED_WriteCmd(0x00);//---set low column address
	OLED_WriteCmd(0x10);//---set high column address
	OLED_WriteCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteCmd(0x81);//--set contrast control register
	OLED_WriteCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WriteCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WriteCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WriteCmd(0xa6);//--set normal display
	OLED_WriteCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WriteCmd(0x3f);//--1/64 duty
	OLED_WriteCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteCmd(0x00);//-not offset
	OLED_WriteCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteCmd(0xd9);//--set pre-charge period
	OLED_WriteCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteCmd(0xda);//--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb);//--set vcomh
	OLED_WriteCmd(0x40);//Set VCOM Deselect Level
	OLED_WriteCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteCmd(0x02);//
	OLED_WriteCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WriteCmd(0x14);//--set(0x10) disable
	OLED_WriteCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_WriteCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);  //初始清屏
	OLED_Set_Pos(0,0);
}




//==============================================================
//函数名： void OLED_PutPixel(O_byte x,O_byte y)
//功能描述：绘制一个点（x,y）
//参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
//返回：无
//==============================================================
void OLED_PutPixel(O_byte x,O_byte y)
{
  O_byte data1;  //data1当前点的数据 
	 
  OLED_Set_Pos(x,y); 
  data1 = 0x01<<(y%8); 	
  OLED_WriteCmd(0xb0+(y>>3));
  OLED_WriteCmd(((x&0xf0)>>4)|0x10);
  OLED_WriteCmd((x&0x0f)|0x00);
  OLED_WriteData(data1); 	 	
}



//==============================================================
//函数名： void OLED_Rectangle(O_byte x1,O_byte y1,
//                   O_byte x2,O_byte y2,O_byte color,O_byte gif)
//功能描述：绘制一个实心矩形
//参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）
//      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
//返回：无
//==============================================================
void OLED_Rectangle(O_byte x1,O_byte y1,O_byte x2,O_byte y2,O_byte gif)
{
	O_byte n; 
		
	OLED_Set_Pos(x1,y1>>3);
	for(n=x1;n<=x2;n++)
	{
		OLED_WriteData(0x01<<(y1%8)); 			
		if(gif == 1) 	OLED_DLY_ms(50);
	}  
	OLED_Set_Pos(x1,y2>>3);
  for(n=x1;n<=x2;n++)
	{
		OLED_WriteData(0x01<<(y2%8)); 			
		if(gif == 1) 	OLED_DLY_ms(5);
	}
}

//==============================================================
//函数名：OLED_P6x8Num(O_byte x,O_byte y,O_byte *p)
//功能描述：写入一个数字
//参数：显示的位置（x,y），y为页范围0～7，要显示的数字，要显示的位数，显示效果
//返回：无
//==============================================================  
void OLED_P6x8Num(uint8_t x,uint8_t y,int32_t Num)
{
  char NumBuff[20];
  uint8_t c=0,i=0,j=0;

  sprintf(NumBuff, "%d", (int32_t)Num);

  while (NumBuff[j] != NULL)
  {    
    c =NumBuff[j]-32;
    if(x>126){x=0;y++;}
    OLED_Set_Pos(x,y);    
    for (i = 0;i < 6;i++)
    {
      OLED_WriteData(F6x8[c][i]);
    }
    x+=6;
    j++;
  }
}

//==============================================================
//函数名：OLED_P6x8floatNum(O_byte x,O_byte y,O_byte *p)
//功能描述：写入一个数字
//参数：显示的位置（x,y），y为页范围0～7，要显示的数字
//返回：无
//==============================================================  
void OLED_P6x8floatNum(O_byte x,O_byte y,float Num)
{
  char NumBuff[10];
  O_byte c=0,i=0,j=0; 
  sprintf(NumBuff,"%f",Num);
	NumBuff[9] = NULL;
  
  while(NumBuff[j])
  {    
    c =NumBuff[j]-32;
    if(x>126){x=0;y++;}
    OLED_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  	  OLED_WriteData(F6x8[c][i]);  
  	x+=6;
  	j++;
  }
}

//==============================================================
//函数名：OLED_P8x16floatNum(O_byte x,O_byte y,int32_t Num,uint8_t num)
//功能描述：写入一个数字
//参数：显示的位置（x,y），y为页范围0～7，要显示的数字
//返回：无
//==============================================================  
void OLED_P8x16floatNum(O_byte x,O_byte y,float Num,uint8_t num)
{
	char NumBuff[8];
	O_byte c=0,i=0,j=0; 

	sprintf(NumBuff,"%f",Num);
       
  while (num--)
  {    
    c =NumBuff[j]-32;
    if(x>120){x=0;y++;}
    OLED_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  OLED_WriteData(F8X16[c*16+i]);
  	OLED_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  OLED_WriteData(F8X16[c*16+i+8]);  
  	x+=8;
  	j++;
  }
}


//==============================================================
//函数名：OLED_P6x8Str(O_byte x,O_byte y,O_byte *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void OLED_P6x8Str(O_byte x,O_byte y,char ch[])//,uint8_t neg)
{
  O_byte c=0,i=0,j=0;      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    OLED_Set_Pos(x,y);    
  	for(i=0;i<6;i++)
		{
//			if(neg)
//					OLED_WriteData(~F6x8[c][i]); 
//			else
				OLED_WriteData(F6x8[c][i]);
  	}
		x+=6;
  	j++;
  }
}

void OLED_P6x8byte_inverse(O_byte x,O_byte y,char ch)
{
  O_byte c=0,i=0,j=0;      
    
	c =ch-32;
	if(x>126){x=0;y++;}
	OLED_Set_Pos(x,y);    
	for(i=0;i<6;i++)
	{
		OLED_WriteData(~F6x8[c][i]); 
	}
	x+=6;
	j++;
}

//==============================================================
//函数名：OLED_P8x16Str(O_byte x,O_byte y,O_byte *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void OLED_P8x16Str(O_byte x,O_byte y,char ch[])
{
  O_byte c=0,i=0,j=0;
        
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    OLED_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  OLED_WriteData(F8X16[c*16+i]);
  	OLED_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  OLED_WriteData(F8X16[c*16+i+8]);  
  	x+=8;
  	j++;
  }
}




//==============================================================
//函数名：      void OLED_P14x16Str(O_byte x,O_byte y,O_byte ch[])
//功能描述：    输出汉字字符串
//参数：        坐标，数组
//返回：        无
//调用：        内部调用/外部调用
//==============================================================
void OLED_P14x16Str(O_byte x,O_byte y,O_byte ch[])
{
	O_byte wm=0,ii = 0;
	O_word adder=1; 
	
	while(ch[ii] != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(F14x16_Idx[wm] > 127)
  	{
  		if(F14x16_Idx[wm] == ch[ii])
  		{
  			if(F14x16_Idx[wm + 1] == ch[ii + 1])
  			{
  				adder = wm * 14;
  				break;
  			}
  		}
  		wm += 2;			
  	}
  	if(x>118){x=0;y++;}
  	OLED_Set_Pos(x , y); 
  	if(adder != 1)// 显示汉字					
  	{
  		OLED_Set_Pos(x , y);
  		for(wm = 0;wm < 14;wm++)               
  		{
  			OLED_WriteData(F14x16[adder]);	
  			adder += 1;
  		}      
  		OLED_Set_Pos(x,y + 1); 
  		for(wm = 0;wm < 14;wm++)          
  		{
  			OLED_WriteData(F14x16[adder]);
  			adder += 1;
  		}   		
  	}
  	else			  //显示空白字符			
  	{
  		ii += 1;
      OLED_Set_Pos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  				OLED_WriteData(0);
  		}
  		OLED_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{   		
  				OLED_WriteData(0);	
  		}
  	}
  	x += 14;
  	ii += 2;
	}
}


//==============================================================
//函数名：      void OLED_Print(O_byte x, O_byte y, O_byte ch[])
//功能描述：    输出汉字和字符混合字符串  
//参数：        坐标，数组
//返回：        无
//调用：        内部调用/外部调用
//==============================================================
void OLED_Print(O_byte x, O_byte y, O_byte ch[])
{
	O_byte ch2[3];
	O_byte ii=0;        
	while(ch[ii] != '\0')
	{
		if(ch[ii] > 127)
		{
			ch2[0] = ch[ii];
	 		ch2[1] = ch[ii + 1];
			ch2[2] = '\0';			//汉字为两个字节
			OLED_P14x16Str(x , y, ch2);	//显示汉字
			x += 14;
			ii += 2;
		}
		else
		{
			ch2[0] = ch[ii];	
			ch2[1] = '\0';			//字母占一个字节
			OLED_P8x16Str(x , y , (char *)ch2);	//显示字母
			x += 8;
			ii+= 1;
		}
	}
} 


//==============================================================
//函数名： void Draw_BMP(O_byte x,O_byte y)
//功能描述：显示BMP图片128×64
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void Draw_BMP(O_byte x0,O_byte y0,O_byte x1,O_byte y1,char bmp[])
{ 	
  O_word ii=0;
  O_byte x,y;
  
  // if(y1%8==0) y=y1/8;      
  // else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
    OLED_Set_Pos(x0,y);				
    for(x=x0;x<x1;x++)
	  {      
	   	OLED_WriteData(bmp[ii++]);	    	
    }
	}
}

void Draw_LOGO(void)
{
  Draw_BMP(0,0,127,7,(char*)LOGO);
}



/************************************************************************************************************/
/**********************************************缓冲区更新********************************************/
/************************************************************************************************************/
uint8_t location_x = 0;
uint8_t location_y = 0;
char All_Data[8][128];

//定位
void Cache_OLED_Set_Pos(uint8_t x,uint8_t y)
{
  location_x = x;
  location_y = y;
}

//往缓冲区写数据
void Cache_OLED_WriteData(uint8_t data)
{
  All_Data[location_y][location_x] = data;
  location_x++;
  if(location_x >= 128)
  {
    location_x = 0;
    location_y++;
    if(location_y >= 8)
      location_y = 0;
  }
}

//写字符串
void Cache_OLED_P6x8Str(uint8_t x,uint8_t y,char ch[])//,uint8_t neg)
{
  uint8_t c=0,i=0,j=0;      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    Cache_OLED_Set_Pos(x,y);    
  	for(i=0;i<6;i++)
				Cache_OLED_WriteData(F6x8[c][i]);
		x+=6;
  	j++;
  }
}

//写取反字
void Cache_OLED_P6x8byte_inverse(uint8_t x,uint8_t y,char ch)
{  
  uint8_t c=0,i=0,j=0;      
    
	c =ch-32;
	if(x>126){x=0;y++;}
	Cache_OLED_Set_Pos(x,y);    
	for(i=0;i<6;i++)
		Cache_OLED_WriteData(~F6x8[c][i]); 
	x+=6;
	j++;
}

//写数
void Cache_OLED_P6x8Num(uint8_t x,uint8_t y,int32_t Num)
{
  char NumBuff[10];
  uint8_t c=0,i=0,j=0;
  sprintf(NumBuff, "%d", (int32_t)Num);

  while (NumBuff[j] != NULL)
  {    
    c =NumBuff[j]-32;
    if(x>126){x=0;y++;}
    Cache_OLED_Set_Pos(x,y);    
    for (i = 0;i < 6;i++)
    {
      Cache_OLED_WriteData(F6x8[c][i]);
    }
    x+=6;
    j++;
  }
}

//写数0x型
void Cache_OLED_P6x8Num_0X(uint8_t x,uint8_t y,int32_t Num_0X)
{
  char NumBuff[10];
  uint8_t c=0,i=0,j=0;
  sprintf(NumBuff, "%0x", (int32_t)Num_0X);

  while (NumBuff[j] != NULL)
  {    
    c =NumBuff[j]-32;
    if(x>126){x=0;y++;}
    Cache_OLED_Set_Pos(x,y);
    for (i = 0;i < 6;i++)
    {
      Cache_OLED_WriteData(F6x8[c][i]);
    }
    x+=6;
    j++;
  }
}

//写浮点数
void Cache_OLED_P6x8floatNum(uint8_t x,uint8_t y,float Num)
{
  char NumBuff[10];
  uint8_t c=0,i=0,j=0;
  snprintf(NumBuff,10,"%.3f",(float)Num);
	NumBuff[9] = NULL;
  
  while(NumBuff[j])
  {    
    c =NumBuff[j]-32;
    if(x>126){x=0;y++;}
    Cache_OLED_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  	  Cache_OLED_WriteData(F6x8[c][i]);  
  	x+=6;
  	j++;
  }
}

//画一个实心矩形
void Cache_OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
	uint8_t n,m; 
	uint8_t sd,xd,zd,y1_,y2_;
	
	sd = 0xff<<(y1%8);
	xd = 0xff>>(7-y2%8);
	y1_ = y1>>3;
	y2_ = y2>>3;
	
	if(y1_ == y2_)
	{
		zd = sd & xd;
		Cache_OLED_Set_Pos(x1,y1_);
		for(n=x1;n<x2;n++)
			Cache_OLED_WriteData(zd);
	}
	else if((y1_+1) == y2_)
	{
		Cache_OLED_Set_Pos(x1,y1_);
		for(n=x1;n<x2;n++)
			Cache_OLED_WriteData(sd);
		Cache_OLED_Set_Pos(x1,y2_);
		for(n=x1;n<x2;n++)
			Cache_OLED_WriteData(xd);
	}
	else
	{
		Cache_OLED_Set_Pos(x1,y1_);
		for(n=x1;n<x2;n++)
			Cache_OLED_WriteData(sd);
		Cache_OLED_Set_Pos(x1,y2_);
		for(n=x1;n<x2;n++)
			Cache_OLED_WriteData(xd);
		for(m=y1_+1;m<y2_;m++)
		{	
			Cache_OLED_Set_Pos(x1,m);
			for(n=x1;n<x2;n++)
				Cache_OLED_WriteData(0xff);
		}
	}
}







////////////////////////////////////////////////////////////////////////////////////////////////
//得到当前X坐标


////////////////////////////////////////////////////////////////////////////////////////////////
//OLED上的printf，除了要输入坐标其他地方和printf用法一样
//由于IAR平台的问题，不修改stdio底层，不能使用 %f 输出浮点数
//
//传入参数：   x:要显示位置的x坐标，x<=127
//             y:要显示位置的y坐标, y<=7
//             const char *pFormat, ... ：用法和printf一样
//传出参数：   result打印的结果
//
//用法实例：OLED_printf(0,0,"hello world"); //在液晶（0,0）位置打印"hello world"
//          OLED_printf(0,1,"a = %d",a);    //在液晶（0,1）位置打印变量a
//          反正除了要输入坐标其他用法和printf一模一样
//          OLED_printf(0,2,"%.2f",3.14); //修改底层之后也能输出浮点数
//
//没有修改stdio底层的情况下：
//要输出浮点数的解决方法：调用float2num函数把浮点数转换为字符串用 %s 输出
//浮点数输出实例： OLED_printf(0,2,"%s",float2str(3.14,2)); //在（0,2）位置输出3.14
//
//有时候需要接着之前的输出打印数据，重新计算坐标会很麻烦，可以使用get_x()和get_y()
//获取当前坐标位置实现接着之前的输入打印数据
//用法实例：OLED_printf(get_x(),get_y(),"prit");//接着当前坐标打印了 "prit"
//
//注意：OLED_printf会和printf冲突，只能同时使用一个
//      使用float2str()函数进行浮点转字符串的时候，一个OLED_printf();只能
//      输出一个浮点数，如果想OLED_printf(0,2,"%s %s",float2str(3.14,2) , float2str(1.2,1));
//      的话，会出错，结果显示的是打印出来两个 3.14 而不是 3.14 1.2
////////////////////////////////////////////////////////////////////////////////////////////////
signed int Cache_OLED_printf(uint8_t x,uint8_t y,const char *pFormat, ...){
    
    char pStr[25] = {'\0'}; 
    va_list ap;
    signed int result;
     
     //Forward call to vprintf
    va_start(ap, pFormat);
    result = vsprintf((char *)pStr, pFormat, ap);
    va_end(ap);

    Cache_OLED_P6x8Str(x,y,(char *)pStr); 

    return result;
}

//缓冲区数据推往OLED
void Cache_Update_OLED(void)
{
  uint8_t x,y;
	// Draw_BMP(0,0,127,7,(char*)All_Data);
  for(y = 0;y < 8;y++)
  {
    OLED_Set_Pos(0,y);
    for(x = 0;x < 128;x++)
    {
      OLED_WriteData(All_Data[y][x]);
      All_Data[y][x] = 0x00;
    }
  }
}

uint8_t getx(void){

    return location_x;
	
}

//得到当前Y坐标
uint8_t gety(void ){

    return location_y;
}

