/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		ͷ�ļ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/
#ifndef _headfile_h
#define _headfile_h


#include "common.h"
#include "KEA128_port_cfg.h"
#include <stdio.h>
#include "stdbool.h"

//--------������--------
#include "KEA128_ftm.h"
#include "KEA128_pit.h"
#include "KEA128_gpio.h"
#include "KEA128_port.h"
#include "KEA128_adc.h"
#include "KEA128_irq.h"
#include "KEA128_uart.h"
#include "KEA128_kbi.h"
#include "KEA128_flash.h"
#include "KEA128_systick.h"
#include "KEA128_i2c.h"
#include "KEA128_spi.h"




//--------FMC��--------
#include "OLED.h"
#include "struct.h"
#include "sensor.h"
#include "ANO_DT.h"
#include "control.h"
#include "motor.h"
#include "encode.h"
#include "Car_Key.h"


//-------------------------------------
#include "Debug.h"
#include  "MYUI.h"




#endif
