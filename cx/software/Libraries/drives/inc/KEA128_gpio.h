/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		KEA128_gpio
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#ifndef _KEA128_gpio_h
#define _KEA128_gpio_h



#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"

//??PTA???? 
//#define PTA0_OUT     GPIOA->PDORs.PDOR0	
//#define PTA1_OUT     GPIOA->PDORs.PDOR1
//#define PTA2_OUT     GPIOA->PDORs.PDOR2
//#define PTA3_OUT     GPIOA->PDORs.PDOR3
//#define PTA4_OUT     GPIOA->PDORs.PDOR4
//#define PTA5_OUT     GPIOA->PDORs.PDOR5
//#define PTA6_OUT     GPIOA->PDORs.PDOR6
//#define PTA7_OUT     GPIOA->PDORs.PDOR7
//#define PTB0_OUT     GPIOA->PDORs.PDOR8
//#define PTB1_OUT     GPIOA->PDORs.PDOR9
//#define PTB2_OUT     GPIOA->PDORs.PDOR10
//#define PTB3_OUT     GPIOA->PDORs.PDOR11
//#define PTB4_OUT     GPIOA->PDORs.PDOR12
//#define PTB5_OUT     GPIOA->PDORs.PDOR13
//#define PTB6_OUT     GPIOA->PDORs.PDOR14
//#define PTB7_OUT     GPIOA->PDORs.PDOR15
//#define PTC0_OUT     GPIOA->PDORs.PDOR16
//#define PTC1_OUT     GPIOA->PDORs.PDOR17
//#define PTC2_OUT     GPIOA->PDORs.PDOR18
//#define PTC3_OUT     GPIOA->PDORs.PDOR19
//#define PTC4_OUT     GPIOA->PDORs.PDOR20
//#define PTC5_OUT     GPIOA->PDORs.PDOR21
//#define PTC6_OUT     GPIOA->PDORs.PDOR22
//#define PTC7_OUT     GPIOA->PDORs.PDOR23
//#define PTD0_OUT     GPIOA->PDORs.PDOR24
//#define PTD1_OUT     GPIOA->PDORs.PDOR25
//#define PTD2_OUT     GPIOA->PDORs.PDOR26
//#define PTD3_OUT     GPIOA->PDORs.PDOR27
//#define PTD4_OUT     GPIOA->PDORs.PDOR28
//#define PTD5_OUT     GPIOA->PDORs.PDOR29
//#define PTD6_OUT     GPIOA->PDORs.PDOR30
//#define PTD7_OUT     GPIOA->PDORs.PDOR31

////??PTB???? 
//#define PTE0_OUT     GPIOB->PDORs.PDOR0
//#define PTE1_OUT     GPIOB->PDORs.PDOR1
//#define PTE2_OUT     GPIOB->PDORs.PDOR2
//#define PTE3_OUT     GPIOB->PDORs.PDOR3
//#define PTE4_OUT     GPIOB->PDORs.PDOR4
//#define PTE5_OUT     GPIOB->PDORs.PDOR5
//#define PTE6_OUT     GPIOB->PDORs.PDOR6
//#define PTE7_OUT     GPIOB->PDORs.PDOR7
//#define PTF0_OUT     GPIOB->PDORs.PDOR8
//#define PTF1_OUT     GPIOB->PDORs.PDOR9
//#define PTF2_OUT    GPIOB->PDORs.PDOR10
//#define PTF3_OUT    GPIOB->PDORs.PDOR11
//#define PTF4_OUT    GPIOB->PDORs.PDOR12
//#define PTF5_OUT    GPIOB->PDORs.PDOR13
//#define PTF6_OUT    GPIOB->PDORs.PDOR14
//#define PTF7_OUT    GPIOB->PDORs.PDOR15
//#define PTG0_OUT    GPIOB->PDORs.PDOR16
//#define PTG1_OUT    GPIOB->PDORs.PDOR17
//#define PTG2_OUT    GPIOB->PDORs.PDOR18
//#define PTG3_OUT    GPIOB->PDORs.PDOR19
//#define PTG4_OUT    GPIOB->PDORs.PDOR20
//#define PTG5_OUT    GPIOB->PDORs.PDOR21
//#define PTG6_OUT    GPIOB->PDORs.PDOR22
//#define PTG7_OUT    GPIOB->PDORs.PDOR23
//#define PTH0_OUT    GPIOB->PDORs.PDOR24
//#define PTH1_OUT    GPIOB->PDORs.PDOR25
//#define PTH2_OUT    GPIOB->PDORs.PDOR26
//#define PTH3_OUT    GPIOB->PDORs.PDOR27
//#define PTH4_OUT    GPIOB_PDOR&(1<<28)
//#define PTH5_OUT    GPIOB->PDORs.PDOR29
//#define PTH6_OUT    GPIOB->PDORs.PDOR30
//#define PTH7_OUT    GPIOB->PDORs.PDOR31



////??PTC???? 
//#define PTI0_OUT     GPIOC->PDORs.PDOR0
//#define PTI1_OUT     GPIOC->PDORs.PDOR1
//#define PTI2_OUT     GPIOC->PDORs.PDOR2
//#define PTI3_OUT     GPIOC->PDORs.PDOR3
//#define PTI4_OUT     GPIOC->PDORs.PDOR4
//#define PTI5_OUT     GPIOC->PDORs.PDOR5
//#define PTI6_OUT     GPIOC->PDORs.PDOR6
//#define PTI7_OUT     GPIOC->PDORs.PDOR7



//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8λ�˿ڵ�ַ����
//  @param      x               xȡֵ��Χ 0����Ӧϵͳ���ϵ�A0-A7�˸����� 1����Ӧϵͳ���ϵ�B0-B7�˸����� 
//                                        2����Ӧϵͳ���ϵ�C0-C7�˸����� 3����Ӧϵͳ���ϵ�D0-D7�˸�����
//  @since      v2.0
//  Sample usage:               A_PDOR8(2) = 0XFF;   //C0-C7�˸���������ߵ�ƽ
//-------------------------------------------------------------------------------------------------------------------
#define A_PDOR8(x)    *(uint8 *)(FGPIOA_BASE+0x00+x) //����A�˿ڵ�8λ�����ַ
#define A_PSOR8(x)    *(uint8 *)(FGPIOA_BASE+0x04+x) //����A�˿ڵ�8λ��λ��ַ
#define A_PCOR8(x)    *(uint8 *)(FGPIOA_BASE+0x08+x) //����A�˿ڵ�8λ�����ַ
#define A_PTOR8(x)    *(uint8 *)(FGPIOA_BASE+0x0C+x) //����A�˿ڵ�8λȡ����ַ
#define A_PDIR8(x)    *(uint8 *)(FGPIOA_BASE+0x10+x) //����A�˿ڵ�8λ�����ַ
#define A_PDDR8(x)    *(uint8 *)(FGPIOA_BASE+0x14+x) //����A�˿ڵ�8λ�����ַ
#define A_PIDR8(x)    *(uint8 *)(FGPIOA_BASE+0x18+x) //����A�˿ڵ�8λ���õ�ַ                                                     
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8λ�˿ڵ�ַ����
//  @param      x               xȡֵ��Χ 0����Ӧϵͳ���ϵ�E0-E7�˸����� 1����Ӧϵͳ���ϵ�F0-F7�˸����� 
//                                        2����Ӧϵͳ���ϵ�G0-G7�˸����� 3����Ӧϵͳ���ϵ�H0-H7�˸�����
//  @since      v2.0
//  Sample usage:               B_PDOR8(3) = 0XFF;   //H0-H7�˸���������ߵ�ƽ
//-------------------------------------------------------------------------------------------------------------------                                          
#define B_PDOR8(x)    *(uint8 *)(FGPIOB_BASE+0x00+x) //����B�˿ڵ�8λ�����ַ
#define B_PSOR8(x)    *(uint8 *)(FGPIOB_BASE+0x04+x) //����B�˿ڵ�8λ��λ��ַ
#define B_PCOR8(x)    *(uint8 *)(FGPIOB_BASE+0x08+x) //����B�˿ڵ�8λ�����ַ
#define B_PTOR8(x)    *(uint8 *)(FGPIOB_BASE+0x0C+x) //����B�˿ڵ�8λȡ����ַ
#define B_PDIR8(x)    *(uint8 *)(FGPIOB_BASE+0x10+x) //����B�˿ڵ�8λ�����ַ
#define B_PDDR8(x)    *(uint8 *)(FGPIOB_BASE+0x14+x) //����B�˿ڵ�8λ�����ַ
#define B_PIDR8(x)    *(uint8 *)(FGPIOB_BASE+0x18+x) //����B�˿ڵ�8λ���õ�ַ                              
//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO            8λ�˿ڵ�ַ����
//  @param      x               xȡֵ��Χ 0����Ӧϵͳ���ϵ�I0-I7�˸�����
//  @since      v2.0
//  Sample usage:               B_PDOR8(0) = 0XFF;   //I0-I7�˸���������ߵ�ƽ
//-------------------------------------------------------------------------------------------------------------------                              
#define C_PDOR8(x)    *(uint8 *)(FGPIOC_BASE+0x00+x) //����C�˿ڵ�8λ�����ַ
#define C_PSOR8(x)    *(uint8 *)(FGPIOC_BASE+0x04+x) //����C�˿ڵ�8λ��λ��ַ
#define C_PCOR8(x)    *(uint8 *)(FGPIOC_BASE+0x08+x) //����C�˿ڵ�8λ�����ַ
#define C_PTOR8(x)    *(uint8 *)(FGPIOC_BASE+0x0C+x) //����C�˿ڵ�8λȡ����ַ
#define C_PDIR8(x)    *(uint8 *)(FGPIOC_BASE+0x10+x) //����C�˿ڵ�8λ�����ַ
#define C_PDDR8(x)    *(uint8 *)(FGPIOC_BASE+0x14+x) //����C�˿ڵ�8λ�����ַ
#define C_PIDR8(x)    *(uint8 *)(FGPIOC_BASE+0x18+x) //����C�˿ڵ�8λ���õ�ַ



void    gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat);//��ʼ��gpio
void    gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr);            //�������ŷ���
uint8   gpio_get(PTX_n ptx_n);                          //��ȡ����״̬
void    gpio_set(PTX_n ptx_n, uint8 dat);               //��������״̬
void    gpio_turn(PTX_n ptx_n);                         //��ת����״̬


#endif
