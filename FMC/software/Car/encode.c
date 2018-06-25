 /***********************************************************

��������غ���










*************************************************************************/
#include "KEA128_gpio.h"
#include "KEA128_ftm.h"
#include "KEA128_port.h"
#include "struct.h"
#include "encode.h"




int16_t getCountNum_L;  //��ȡ����������ֵ
int16_t getCountNum_R;  //��ȡ����������ֵ

/*******************************************
 *  * �������� ��encode_init
 *  * �������� : ��
 *  * ����ֵ   : ��
 *  * �������� ����������ʼ��
 *  * ��    �� : 2018/03/14

****************************/


void encode_init(void)
{
    ftm_count_init(ftm0);   //�������
    gpio_init(C5,GPI,0);    //DIR_Lʼ��
    port_pull(C5);

    ftm_count_init(ftm1);   //�ұ�����
    gpio_init(H5,GPI,0);    //DIR_R��ʼ��
    port_pull(H5);
}



void get_num(void)
{  
	if(gpio_get(C5))
        getCountNum_R=-ftm_count_get(ftm0);        //��ȡ�ұ�����ֵ
    else
       getCountNum_R=ftm_count_get(ftm0);        //��ȡ������ֵ
    ftm_count_clean(ftm0);

		
    if(gpio_get(H5))
        getCountNum_L=ftm_count_get(ftm1);        //��ȡ�������ֵ
    else
    {
        getCountNum_L=-ftm_count_get(ftm1);        //��ȡ������ֵ
    }
    ftm_count_clean(ftm1);                  //�������ֵ
}






