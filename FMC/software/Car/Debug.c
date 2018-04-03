/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2015,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_computer.c
 * @brief      ɽ��๦�ܵ���������λ����������
 * @author     ɽ��Ƽ�
 * @version    v5.2.2
 * @date       2015-03-24
 */


/*
 * ����ͷ�ļ�
 */
#include "KEA128_uart.h"
#include "Debug.h"
#include "headfile.h"

/*!
 *  @brief      ɽ��๦�ܵ���������λ��������ʾ������ʾ����
 *  @param      wareaddr    ����������ʼ��ַ
 *  @param      waresize    ��������ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ�����ϡ������Ǻͼ��ٶ� ��λ����ʾ���� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6253&ctid=27
 */

//KEA128 ��ʱ3.6ms


void vcan_sendware(void *wareaddr, uint32_t waresize)
{
		#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_putbuff(DEBUG_PORT, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(DEBUG_PORT, (uint8_t *)wareaddr, waresize);    //��������
    uart_putbuff(DEBUG_PORT, cmdr, sizeof(cmdr));    //���ͺ�����

}



