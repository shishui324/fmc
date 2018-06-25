/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2015,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_computer.c
 * @brief      山外多功能调试助手上位机驱动代码
 * @author     山外科技
 * @version    v5.2.2
 * @date       2015-03-24
 */


/*
 * 包含头文件
 */
#include "KEA128_uart.h"
#include "Debug.h"
#include "headfile.h"

/*!
 *  @brief      山外多功能调试助手上位机，虚拟示波器显示函数
 *  @param      wareaddr    波形数组起始地址
 *  @param      waresize    波形数组占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外资料】陀螺仪和加速度 上位机显示例程 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6253&ctid=27
 */

//KEA128 用时3.6ms

uint16 BB_EN = 0,BB_TIME = 0,QQ_TIME = 0,BB_TIME_COUNT = 0;
uint16 LED0_EN = 0,LED0_BB_TIME = 0,LED0_QQ_TIME = 0,LED0_BB_TIME_COUNT = 0;
uint16 LED1_EN = 0,LED1_BB_TIME = 0,LED1_QQ_TIME = 0,LED1_BB_TIME_COUNT = 0;
uint16 LED2_EN = 0,LED2_BB_TIME = 0,LED2_QQ_TIME = 0,LED2_BB_TIME_COUNT = 0;
uint16 LED3_EN = 0,LED3_BB_TIME = 0,LED3_QQ_TIME = 0,LED3_BB_TIME_COUNT = 0;

void vcan_sendware(void *wareaddr, uint32_t waresize)
{
	#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(DEBUG_PORT, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(DEBUG_PORT, (uint8_t *)wareaddr, waresize);    //发送数据
    uart_putbuff(DEBUG_PORT, cmdr, sizeof(cmdr));    //发送后命令

}
void BB_Cry(uint16 bb_time,uint16 qq_time)
{
	BB_EN = 1;
	BB_TIME = bb_time;
	QQ_TIME = qq_time;
}
void BB_Play(void)
{
	if(BB_EN)
	{
		BB_TIME_COUNT++;
		if(BB_TIME_COUNT<BB_TIME)
			gpio_set(H6,0);
		else if(BB_TIME_COUNT<(BB_TIME+QQ_TIME))
			gpio_set(H6,1);
		else
			BB_TIME_COUNT = 0;
	}
	else
	{
		gpio_set(H6,1);
		BB_TIME = 0;
		QQ_TIME = 0;
	}
	BB_EN = 0;
}

void bling_bling_led0(uint16 bb_time,uint16 qq_time)
{
	LED0_EN = 1;
	LED0_BB_TIME = bb_time;
	LED0_QQ_TIME = qq_time;
}
void bling_bling_led0_play(void)
{
	if(LED0_EN)
	{
		LED0_BB_TIME_COUNT++;
		if(LED0_BB_TIME_COUNT<LED0_BB_TIME)
			gpio_set(C7,1);
		else if(LED0_BB_TIME_COUNT<(LED0_BB_TIME+LED0_QQ_TIME))
			gpio_set(C7,0);
		else
			LED0_BB_TIME_COUNT = 0;
	}
	else
	{
		gpio_set(C7,0);
		LED0_BB_TIME_COUNT = 0;
		LED0_QQ_TIME = 0;
	}
	LED0_EN = 0;
}

void bling_bling_led1(uint16 bb_time,uint16 qq_time)
{
	LED1_EN = 1;
	LED1_BB_TIME = bb_time;
	LED1_QQ_TIME = qq_time;
}
void bling_bling_led1_play(void)
{
	if(LED1_EN)
	{
		LED1_BB_TIME_COUNT++;
		if(LED1_BB_TIME_COUNT<LED1_BB_TIME)
			gpio_set(C6,1);
		else if(LED1_BB_TIME_COUNT<(LED1_BB_TIME+LED1_QQ_TIME))
			gpio_set(C6,0);
		else
			LED1_BB_TIME_COUNT = 0;
	}
	else
	{
		gpio_set(C6,0);
		LED1_BB_TIME_COUNT = 0;
		LED1_QQ_TIME = 0;
	}
	LED1_EN = 0;
}

void bling_bling_led2(uint16 bb_time,uint16 qq_time)
{
	LED2_EN = 1;
	LED2_BB_TIME = bb_time;
	LED2_QQ_TIME = qq_time;
}
void bling_bling_led2_play(void)
{
	if(LED2_EN)
	{
		LED2_BB_TIME_COUNT++;
		if(LED2_BB_TIME_COUNT<LED2_BB_TIME)
			gpio_set(I3,1);
		else if(LED2_BB_TIME_COUNT<(LED2_BB_TIME+LED2_QQ_TIME))
			gpio_set(I3,0);
		else
			LED2_BB_TIME_COUNT = 0;
	}
	else
	{
		gpio_set(I3,0);
		LED2_BB_TIME_COUNT = 0;
		LED2_QQ_TIME = 0;
	}
	LED2_EN = 0;
}

void bling_bling_led3(uint16 bb_time,uint16 qq_time)
{
	LED3_EN = 1;
	LED3_BB_TIME = bb_time;
	LED3_QQ_TIME = qq_time;
}
void bling_bling_led3_play(void)
{
	if(LED3_EN)
	{
		LED3_BB_TIME_COUNT++;
		if(LED3_BB_TIME_COUNT<LED3_BB_TIME)
			gpio_set(I2,1);
		else if(LED3_BB_TIME_COUNT<(LED3_BB_TIME+LED3_QQ_TIME))
			gpio_set(I2,0);
		else
			LED3_BB_TIME_COUNT = 0;
	}
	else
	{
		gpio_set(I2,0);
		LED3_BB_TIME_COUNT = 0;
		LED3_QQ_TIME = 0;
	}
	LED3_EN = 0;
}

void BB_LED_PLAY(void)
{
	BB_Play();
	bling_bling_led0_play();
	bling_bling_led1_play();
	bling_bling_led2_play();
	bling_bling_led3_play();
}


