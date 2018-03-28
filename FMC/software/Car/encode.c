/***********************************************************

��������غ���










*************************************************************************/
#include "encode.h"
#include "headfile.h"
#include "struct.h"


int8_t getCountNum_L;  //��ȡ����������ֵ
int8_t getCountNum_R;  //��ȡ����������ֵ

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
    gpio_init(E5,GPI,0);    //DIR_Lʼ��
    port_pull(E5);

    ftm_count_init(ftm1);   //�ұ�����
    gpio_init(D7,GPI,0);    //DIR_R��ʼ��
    port_pull(D7);
}



void get_num(void)
{  
	if(gpio_get(D7))
        getCountNum_R=-ftm_count_get(ftm0);        //��ȡ�ұ�����ֵ
    else
       getCountNum_R=ftm_count_get(ftm0);        //��ȡ������ֵ
    ftm_count_clean(ftm0);

		
    if(gpio_get(E5))
        getCountNum_L=ftm_count_get(ftm1);        //��ȡ�������ֵ
    else
    {
        getCountNum_L=-ftm_count_get(ftm1);        //��ȡ������ֵ
    }
    ftm_count_clean(ftm1);                  //�������ֵ

  

}




