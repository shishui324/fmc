/***********************************************************

编码器相关函数










*************************************************************************/
#include "encode.h"
#include "headfile.h"
#include "struct.h"


int8_t getCountNum_L;  //获取编码器计数值
int8_t getCountNum_R;  //获取编码器计数值

/*******************************************
 *  * 函数名称 ：encode_init
 *  * 函数参数 : 无
 *  * 返回值   : 无
 *  * 函数功能 ：编码器初始化
 *  * 日    期 : 2018/03/14

****************************/


void encode_init(void)
{
    ftm_count_init(ftm0);   //左编码器
    gpio_init(E5,GPI,0);    //DIR_L始化
    port_pull(E5);

    ftm_count_init(ftm1);   //右编码器
    gpio_init(D7,GPI,0);    //DIR_R初始化
    port_pull(D7);
}



void get_num(void)
{  
	if(gpio_get(D7))
        getCountNum_R=-ftm_count_get(ftm0);        //获取右编码器值
    else
       getCountNum_R=ftm_count_get(ftm0);        //获取编码器值
    ftm_count_clean(ftm0);

		
    if(gpio_get(E5))
        getCountNum_L=ftm_count_get(ftm1);        //获取左编码器值
    else
    {
        getCountNum_L=-ftm_count_get(ftm1);        //获取编码器值
    }
    ftm_count_clean(ftm1);                  //清编码器值

  

}




