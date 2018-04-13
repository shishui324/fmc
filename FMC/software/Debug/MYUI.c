#include "headfile.h"
#include "MYUI.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//配置系统在线调参界面核心函数
uint8_t cursor_x = 0,cursor_y = 0; //光标的X,Y坐标
int8_t state = 0; //调参状态 0 1 2 对应处于主菜单 一级子菜单和二级子菜单三个状态
int8_t ParameterNo = 1; //主菜单参数号
int8_t Parameter2No = 1; //一级子菜单的参数号

uint8 Save_Flag=0;		//保存参数标志
uint8 Read_Flag=0;		//读取参数标志


//通过修改这个使能菜单数组，可以设置某个参数号下是否能有二级菜单。1表示有二级菜单，0表示没有
extern float sum_16_34;

extern uint16_t encode_time;
extern uint16_t sensor_time;
extern uint16_t control_time;
extern uint16_t UI_time;

extern Motor_control_info Motor_control;
extern ADC_info Adc;
extern Sensor_info Sensor;

//调参界面中：
//上下可以切换选中的参数
//按下右键与中键确定选择，左键返回
//在一级菜单时长按中键初始化液晶屏
//这个时候可以通过上、下键修改对应的参数，再按一次中心键 光标重新变回 '>' 参数设置完成，光标又可以通过上、下键选择参数
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigParameterCore() {
    uint8 Para_max=7;
    if(Key_Flag4)
        Para_max=14;
    else Para_max=7;

//////////////////////////////// 以下是调参界面的操作逻辑设置   //////////////////////////////////////////////////////////////////////////////////////////
    //移动光标
    if(state == 0) { //在主菜单中（state = 0）

        if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
        {
            Cache_OLED_P6x8Str(cursor_x,cursor_y," ");
            ParameterNo--;
        }
        else if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
        {
            Cache_OLED_P6x8Str(cursor_x,cursor_y," ");
            ParameterNo++;
        }
        if(ParameterNo > 7) ParameterNo = 1;
        else if(ParameterNo < 1) ParameterNo = 7;
    }
    else if(state == 1) { //在二级菜单中

        if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
        {
            Cache_OLED_P6x8Str(cursor_x,cursor_y," ");
            Parameter2No--;
        }
        else if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
        {
            Cache_OLED_P6x8Str(cursor_x,cursor_y," ");
            Parameter2No++;


        }

        //参数号限位
        if(Parameter2No > Para_max) Parameter2No = 1;
        else if(Parameter2No < 1) Parameter2No = Para_max;
    }






    //在这修改  ParameterNo 的范围可以配置哪些一级参数号拥有二级菜单 如果改为 ParameterNo < 15 则所有的一级菜单都拥有二级界面
    //有一些标志位需要经常使用，但是每次进入二级菜单又太麻烦的时候，可以留几个一级界面的位置，直接在一级界面修改标志位，而不进入二级菜单。

    //更改状态进入二级菜单
    if((Key_Inquire_data(Key_Middle_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc)||
            (Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc)
      )
    {
        state ++;

        if((state==2)&&(Parameter2No==2)&&(Key_Flag4))
        {
            Save_Flag=1;
        }
        if((state==2)&&(Parameter2No==1)&&(Key_Flag4))
        {
            Read_Flag=1;
        }

    }

    if(Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) 
    {
        state--;


    }
		if(Key_Inquire_data(Key_Left_Read) == Key_bit_ACCRise)
		{
		OLED_Init();
		
		}
    ///////菜单限制
//		if((en_sec_menu[ParameterNo-1]==0)&&(state>1))
//		state=1;
    if(state>1)
    {
        if(Key_Flag1)
        {
            state=1;
        }
        else if(Key_Flag4)
        {
            state=2;
        }
    }
    if(state<0)state=0;

//////////////////////////////// 调参界面的操作逻辑设置结束   //////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////// 以下是通过参数号确定光标   ///////////////////////////////////////////////////////////////////////////////////////////////////
    if(state == 0) { //一级菜单的参数

        //参数号限位

        //通过参数号确定光标坐标
        if(ParameterNo == 1) {
            cursor_x = 0;
            cursor_y = 1;
        }
        else if(ParameterNo == 2) {
            cursor_x = 0;
            cursor_y = 2;
        }
        else if(ParameterNo == 3) {
            cursor_x = 0;
            cursor_y = 3;
        }
        else if(ParameterNo == 4) {
            cursor_x = 0;
            cursor_y = 4;
        }
        else if(ParameterNo == 5) {
            cursor_x = 0;
            cursor_y = 5;
        }
        else if(ParameterNo == 6) {
            cursor_x = 0;
            cursor_y = 6;
        }
        else if(ParameterNo == 7) {
            cursor_x = 0;
            cursor_y = 7;
        }

    }
    else if(state == 1) { //二级菜单的参数


        //通过参数号确定光标坐标
        if(Parameter2No == 1) {
            cursor_x = 0;
            cursor_y = 1;
        }
        else if(Parameter2No == 2) {
            cursor_x = 0;
            cursor_y = 2;
        }
        else if(Parameter2No == 3) {
            cursor_x = 0;
            cursor_y = 3;
        }
        else if(Parameter2No == 4) {
            cursor_x = 0;
            cursor_y = 4;
        }
        else if(Parameter2No == 5) {
            cursor_x = 0;
            cursor_y = 5;
        }
        else if(Parameter2No == 6) {
            cursor_x = 0;
            cursor_y = 6;
        }
        else if(Parameter2No == 7) {
            cursor_x = 0;
            cursor_y = 7;
        }
        else if(Parameter2No == 8) {
            cursor_x = 0;
            cursor_y = 1;
        }
        else if(Parameter2No == 9) {
            cursor_x = 0;
            cursor_y = 2;
        }
        else if(Parameter2No == 10) {
            cursor_x = 0;
            cursor_y = 3;
        }
        else if(Parameter2No == 11) {
            cursor_x = 0;
            cursor_y = 4;
        }
        else if(Parameter2No == 12) {
            cursor_x = 0;
            cursor_y = 5;
        }
        else if(Parameter2No == 13) {
            cursor_x = 0;
            cursor_y = 6;
        }
        else if(Parameter2No == 14) {
            cursor_x = 0;
            cursor_y = 7;
        }

    }





///////////////////////////////////// 光标位置确定结束 ///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////// 显示光标  ///////////////////////////////////////////////////////////////////////////////////////////////////
    //显示光标
    if(state == 0)
        Cache_OLED_P6x8Str(cursor_x,cursor_y,">");
    else if((state == 1)&&(!(Key_Flag1)))
        Cache_OLED_P6x8Str(cursor_x,cursor_y,">");
    else if(state == 2)
        Cache_OLED_P6x8Str(cursor_x,cursor_y,"*");
//		 else if(state == 3)
//        Cache_OLED_P6x8Str(cursor_x,cursor_y,"#");
///////////////// 显示光标结束 ///////////////////////////////////////////////////////////////////////////////////////////////////
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//通过按键修改某个参数的值 上下键
//传入参数：parameter_type ptype ：需要修改的这个参数的类型，枚举定义在头文件中
//                                int8_t，    该变量为char型
//                                uint8_t，   该变量为unsigned char型
//                                int16_t，   该变量为short型
//                                uint16_t，  该变量为unsigned short型
//                                int32_t，   该变量为int型
//                                uint32_t，  该变量为unsigned int型
//                                pfloat，    该变量为float型
//                                pdouble，   该变量为double型
//
//         void* ppara：要修改的变量
//
//         double precision ：每次加/减的精度
//
//
//
//传出参数：无
//用法实例：ChangeParameter(pdouble,&p,0.05); //修改double类型变量p的值，每次修改0.05
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ChangeParameterVal(parameter_type ptype,void* ppara,double precision) {

    if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
    {

        switch(ptype)
        {
        case pint8_t:
            *(( int8_t *  )ppara)= *(( int8_t *  )ppara) + precision;
            break;
        case puint8_t:
            *(( uint8_t * )ppara)= *(( uint8_t * )ppara) + precision;
            break;
        case pint16_t:
            *(( int16_t * )ppara) = *(( int16_t * )ppara)+ precision;
            break;
        case puint16_t:
            *(( uint16_t *)ppara) = *(( uint16_t *)ppara)+precision;
            break;
        case pint32_t:
            *(( int32_t * )ppara) =*(( int32_t * )ppara)+ precision;
            break;
        case puint32_t:
            *(( uint32_t *)ppara) =*(( uint32_t *)ppara)+ precision;
            break;
        case pfloat:
            *(( float *   )ppara) =*(( float *   )ppara)+ precision;
            break;
        case pdouble:
            *(( double *  )ppara) =*(( double *  )ppara)+ precision;
            break;
        }
    }
    else if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc)) {

        switch(ptype)
        {
        case pint8_t:
            *(( int8_t *  )ppara) =*(( int8_t *  )ppara) - precision;
            break;
        case puint8_t:
            *(( uint8_t * )ppara) =*(( uint8_t * )ppara) - precision;
            break;
        case pint16_t:
            *(( int16_t * )ppara) =*(( int16_t * )ppara) - precision;
            break;
        case puint16_t:
            *(( uint16_t *)ppara) =*(( uint16_t *)ppara) - precision;
            break;
        case pint32_t:
            *(( int32_t * )ppara) =*(( int32_t * )ppara) - precision;
            break;
        case puint32_t:
            *(( uint32_t *)ppara) =*(( uint32_t *)ppara) - precision;
            break;
        case pfloat:
            *(( float *   )ppara) =*(( float *   )ppara) - precision;
            break;
        case pdouble:
            *(( double *  )ppara) =*(( double *  )ppara) - precision;
            break;
        }
    }
}

//修改某标志位的值（一般用于没有二级菜单的位置）
void ChangeFlagVal(uint8 *flag,uint8 min,uint8 max) {

    if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Drop)
        (*flag)++;

    if(*flag > max) *flag = min;
    else if(*flag < min) *flag = max;
}

////////////////////////////段落一:显示类UI/////////////////////////////////////////

/***********************************************************************
**  函数名称:show_sensor
**  函数功能:显示车的运行参数
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void show_sensor(void)
{

    Cache_OLED_Rectangle(0,  (63-((uint16_t)(Sensor.once_uni_ad[1])*63/100.0f)),  8,   63);
    Cache_OLED_Rectangle(14, (63-((uint16_t)(L_out_value*63/1000))),  22,  63);
    Cache_OLED_Rectangle(84, (63-((uint16_t)(R_out_value*63/1000))),  92,  63);
    Cache_OLED_Rectangle(98, (63-((uint16_t)(Sensor.once_uni_ad[6])*63/100.0f)),  106, 63);
    Cache_OLED_P6x8Num(0,0,getCountNum_L);
		Cache_OLED_printf(0,1,"%4f",Sensor.twice_uni_ad[1]);
//    Cache_OLED_P6x8Num(0,1,Sensor.once_uni_ad[2]);
    Cache_OLED_P6x8Num(90,0,getCountNum_R);
		Cache_OLED_printf(90,1,"%4f",Sensor.twice_uni_ad[6]);
 //   Cache_OLED_P6x8Num(90,1,Sensor.once_uni_ad[6]);
//        Cache_OLED_P6x8Num(90,0,ad_avr_val[2]);

    Cache_OLED_P6x8floatNum(40,0,Servo.error[0]);
    Cache_OLED_P6x8floatNum(40,1,Servo.output);
    Cache_OLED_P6x8floatNum(40,2,Sensor.sum_16_34);
		Cache_OLED_P6x8floatNum(40,3,Sensor.sub_25[0]);
		//Cache_OLED_P6x8floatNum(40,4,circle_distence);
    Cache_OLED_P6x8floatNum(40,4,motor_protect_time);
		Cache_OLED_P6x8floatNum(40,5,100*(Sensor.twice_uni_ad[1]-Sensor.twice_uni_ad[6]));

}


/***********************************************************************
**  函数名称:show_8_L
**  函数功能:显示8个电感的值-----滤波后，未进行归一化处理
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void show_8_L(void)
{
	for(uint8_t i=1;i<=SENSOR_NUM;i++)
	{
		Cache_OLED_Rectangle(0,8*(i-1),(Adc.ad_avr_val[i]*80/3000),8*i);
		Cache_OLED_printf(85,i-1,"%1d:%4d",i,Adc.ad_avr_val[i]);
	}
		  

}


/***********************************************************************
**  函数名称:show_one_sensor
**  函数功能:显示电感一次归一化之后的值
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
***************************************************************************/

void show_one_sensor(void)
{
		for(uint8_t i=1;i<=SENSOR_NUM;i++)
	{
		Cache_OLED_Rectangle(0,8*(i-1),(Sensor.once_uni_ad[i]*80/100),8*i);
		Cache_OLED_printf(85,i-1,"%1d:%d",i,(uint16_t)Sensor.once_uni_ad[i]);
	} 

}
/***********************************************************************
**  函数名称:
**  函数功能:
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
***************************************************************************/

void show_circle(void)
{

//    Cache_OLED_Rectangle( 0,  (63-((uint16_t)(ad_avr_val[1])*63/3000)),  8,   63);
//    Cache_OLED_Rectangle(16,  (63-((uint16_t)(ad_avr_val[2])*63/3000)),  24,   63);
//    Cache_OLED_Rectangle(32,  (63-((uint16_t)(ad_avr_val[3])*63/3000)),  40,   63);
////	Cache_OLED_Rectangle(48,  (63-((uint16_t)(ad_avr_val[4])*63/3000)),  56,   63);
////	Cache_OLED_Rectangle(64,  (63-((uint16_t)(ad_avr_val[5])*63/3000)),  72,   63);
//    Cache_OLED_Rectangle(80,  (63-((uint16_t)(ad_avr_val[4])*63/3000)),  88,   63);
//    Cache_OLED_Rectangle(96,  (63-((uint16_t)(ad_avr_val[5])*63/3000)),  104,  63);
//    Cache_OLED_Rectangle(112, (63-((uint16_t)(ad_avr_val[6])*63/3000)),  120,63);

//    Cache_OLED_P6x8Num(0,7,1);
//    Cache_OLED_P6x8Num(16,7,2);
//    Cache_OLED_P6x8Num(32,7,3);
////
////	Cache_OLED_P6x8Num(48,7,4);
////	Cache_OLED_P6x8Num(64,7,5);
//    Cache_OLED_P6x8Num(80,7,4);
//    Cache_OLED_P6x8Num(96,7,5);
//    Cache_OLED_P6x8Num(112,7,6);
	Cache_OLED_printf(0,0,"sum:%.1f",Sensor.sum_16_34);
	Cache_OLED_printf(0,1,"sub:%.1f",Sensor.sub_25[0]);
	Cache_OLED_printf(0,2,"IN: %4d",circle_in);
	Cache_OLED_printf(0,3,"L : %4d",circle_left_flag);
	Cache_OLED_printf(0,4,"R : %4d",circle_right_flag);
	Cache_OLED_printf(0,5,"turn%4d",circle_turn);
	Cache_OLED_printf(0,6,"out:%4d",Servo.output);
	Cache_OLED_printf(0,7,"dis:%4d",circle_distence);
	Cache_OLED_printf(64,7,"er:%4d",Servo.error[0]);
	Cache_OLED_printf(64,6,"%d",motor_protect_time);
	Cache_OLED_printf(64,3,"1-6%f",Sensor.sub_16);

//	  Cache_OLED_P6x8floatNum(40,2,sum_16_34);
//	
//		Cache_OLED_P6x8floatNum(40,3,sub_25);
//		Cache_OLED_P6x8floatNum(40,4,circle_distence);
	




}

/***********************************************************************
**  函数名称:
**  函数功能:
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void show_run_time(void)
{
  Cache_OLED_printf(20,1,"encode %d",encode_time);
	Cache_OLED_printf(20,2,"sensor %d",sensor_time);
	Cache_OLED_printf(20,3,"control %d",control_time);
	Cache_OLED_printf(20,4,"UI %d",UI_time);

}
/***********************************************************************
**  函数名称:Show_Main_menu
**  函数功能:显示类UI的主菜单界面
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
***************************************************************************/

void Show_Main_menu(void)
{

    Cache_OLED_printf(20,0,"Show_menu  %.3fV",Power_V);
    Cache_OLED_P6x8Str(7,1,"1.sensor_show");
    Cache_OLED_P6x8Str(7,2,"2.once_uni_ad");
    Cache_OLED_P6x8Str(7,3,"3.sen");
    Cache_OLED_P6x8Str(7,4,"4.show_circle");
    Cache_OLED_P6x8Str(7,5,"5.");
    Cache_OLED_P6x8Str(7,6,"6.run time");
    Cache_OLED_P6x8Str(7,7,"7.");

}
/***********************************************************************
**  函数名称:Show_UI
**  函数功能:显示类UI
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
***************************************************************************/

void Show_UI(void)
{
/////////   显示一级菜单内容 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(state == 0) {

        Show_Main_menu();
    }
/////////   显示二级菜单内容 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if(state == 1) {

        switch(ParameterNo)
        {

        case 1: { 				//左电机PID的参数
            show_sensor();
        }
        break;
        case 2 : {
            show_one_sensor();
        }
        break;
        case 3 : {				//图形显示
            show_8_L();

        }
        break;

        case 4 : {
            show_circle();
        }
        break;
        case 5 : {
            Cache_OLED_P6x8floatNum(40,1,Adc.ad_max_temp[7]);
            Cache_OLED_P6x8floatNum(40,2,Adc.ad_max_val[2]);
            Cache_OLED_P6x8floatNum(40,3,Adc.ad_max_val[3]);
            Cache_OLED_P6x8floatNum(40,4,Adc.ad_max_val[4]);
            Cache_OLED_P6x8floatNum(40,5,Adc.ad_max_val[5]);
            Cache_OLED_P6x8floatNum(40,6,Adc.ad_max_val[6]);
						Cache_OLED_P6x8floatNum(40,7,Adc.ad_max_val[7]);
        }
        break;

        case 6 : {
					show_run_time();
        } break;

        case 7 : {
        } break;

        default :
            break ;

        }

    }
}

//////////////////////////////////段落二：调参UI/////////////////////////////////////////////
/***********************************************************************
**  函数名称:
**  函数功能:
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
***************************************************************************/

void Show_Left_Motor_PID(void)
{

    Cache_OLED_P6x8Str(25,0,"Left_Motor_PID ");
    if(Parameter2No<=7)
    {
        Cache_OLED_printf(6,1,"Read parameter");
        Cache_OLED_printf(6,2,"save parameter");

        Cache_OLED_printf(6,3,"kp:%.1f",Motor_control.Motor_Left_pid.kp);
        Cache_OLED_printf(6,4,"ki:%.1f",Motor_control.Motor_Left_pid.ki);
        Cache_OLED_printf(6,5,"kd:%.1f",Motor_control.Motor_Left_pid.kd);
        Cache_OLED_printf(6,6,"kvff:%.1f",Motor_control.Motor_Left_pid.kvff);
        Cache_OLED_printf(6,7,"kaff:%.1f",Motor_control.Motor_Left_pid.kaff);


    }


}
/***********************************************************************
**  函数名称:
**  函数功能:
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
***************************************************************************/

void Show_Right_Motor_PID(void)
{

    Cache_OLED_P6x8Str(25,0,"Right_Motor_PID ");
    if(Parameter2No<=7)
    {
        Cache_OLED_printf(6,1,"Read parameter");
        Cache_OLED_printf(6,2,"save parameter");
        Cache_OLED_printf(6,3,"kp:%.1f",Motor_control.Motor_Right_pid.kp);
        Cache_OLED_printf(6,4,"ki:%.1f",Motor_control.Motor_Right_pid.ki);
        Cache_OLED_printf(6,5,"kd:%.1f",Motor_control.Motor_Right_pid.kd);
        Cache_OLED_printf(6,6,"kvff:%.1f",Motor_control.Motor_Right_pid.kvff);
        Cache_OLED_printf(6,7,"kaff:%.1f",Motor_control.Motor_Right_pid.kaff);


    }
}
/***********************************************************************
**  函数名称:Change_Servo_PID
**  函数功能:显示方向PID参数
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
***************************************************************************/

void Show_Servo_PID(void)
{
    Cache_OLED_P6x8Str(40,0,"Servo_PID ");

    if(Parameter2No<=7)
    {

        Cache_OLED_printf(6,1,"Read parameter");
        Cache_OLED_printf(6,2,"save parameter");
        Cache_OLED_printf(6,3,"kp:%.1f",Servo.kp );
        Cache_OLED_printf(6,4,"ki:%.1f",Servo.ki);
        Cache_OLED_printf(6,5,"kd:%.1f",Servo.kd);
        Cache_OLED_printf(6,6,"err_max:%.1f",Servo.distance_err_max_val);
        Cache_OLED_printf(6,7,"err_d_max:%.1f",Servo.distance_err_d_max_val);

    }
    else
    {
        Cache_OLED_printf(6,1,"dis_err_max:%.1f",Servo.max_dis_err);
        Cache_OLED_printf(6,2,"dis_err_d_max:%.1f",Servo.max_dis_err_d);

    }

}

/***********************************************************************
**  函数名称:Change_speed
**  函数功能:显示速度参数值
**  输  入	值:
**  返  回  值:
**  其他说明:
**  日       期:
**************************************************************************/

void Show_speed(void)
{
    Cache_OLED_P6x8Str(40,0,"Speed ");

    if(Parameter2No<=7)
    {

        Cache_OLED_printf(6,1,"Read parameter");
        Cache_OLED_printf(6,2,"save parameter");
        Cache_OLED_printf(6,3,"zhidao:%d",Speed.zhidao_speed_val);
        Cache_OLED_printf(6,4,"wandao:%d",Speed.wandao_speed_val);
        Cache_OLED_printf(6,5,"cross:%d",Speed.cross_speed_val);
        Cache_OLED_printf(6,6,"shizi:%d",Speed.shizi_speed_val);
        Cache_OLED_printf(6,7,"ramp1:%d",Speed.ramp1_speed_val);

    }
    else
    {
        Cache_OLED_printf(6,1,"ramp2:%d",Speed.ramp2_speed_val);
        Cache_OLED_printf(6,2,"high mode:%d",Speed.speed_high_mode);
        Cache_OLED_printf(6,3,"high dis:%d",Speed.high_speed_dis);
        Cache_OLED_printf(6,4,"stop:%d",Speed.stop_car_enable);
        Cache_OLED_printf(6,5,"test:%d",Speed.test_time);
				Cache_OLED_printf(6,6,"VATE:%d",Speed.set_speed_val);

    }



}

/***********************************************************************
**  函数名称:Change_Main_menu
**  函数功能:调参主菜单显示界面
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void Change_Main_menu(void)
{
    Cache_OLED_printf(10,0,"Change_menu%.3fV",Power_V);
    Cache_OLED_P6x8Str(7,1,"1.left motor");
    Cache_OLED_P6x8Str(7,2,"2.right motor");
    Cache_OLED_P6x8Str(7,3,"3.servo PID");
    Cache_OLED_P6x8Str(7,4,"4.speed");
    Cache_OLED_P6x8Str(7,5,"5.");
    Cache_OLED_P6x8Str(7,6,"6.");
    Cache_OLED_P6x8Str(7,7,"7.");

}
/***********************************************************************
**  函数名称:Left_Motor_menu
**  函数功能:左电机控制菜单，用来改变左电机的参数
**  输 入 值:
**  返 回 值:
**  其他说明: 
**  日    期:
***************************************************************************/

void Left_Motor_menu(void)
{
    switch(Parameter2No)
    {
    case 1: {

        if(Read_Flag)
        {
            Read_Flag=0;
            Read_Left_motor();
            OLED_CLS();
            OLED_P6x8Str(40,0,"Read ok");
            systick_delay_ms(1000);
        }
        state=1;
//				EnableInterrupts;//开总中断

    }
    break;
    case 2: {

        if(Save_Flag)
        {

            Save_Flag=Save_Left_motor();
            OLED_CLS();
            if(Save_Flag)
            {
                OLED_P6x8Str(40,0,"save error");
                Save_Flag=0;
            }
            else
            {
                OLED_P6x8Str(40,0,"save ok");
            }


            systick_delay_ms(1000);

            EnableInterrupts;	//开总中断
        }


        state=1;

    }
    break ;


    case 3: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kp,0.1);
    }
    break ;
    case 4: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.ki,0.1);

    }
    break ;
    case 5: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kd,0.1);
    }
    break ;
    case 6: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kvff,0.1);
    }
    break ;
    case 7: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kaff,0.1);
    }
    break ;


    default :
        break ;


    }
    Show_Left_Motor_PID();

}
/***********************************************************************
**  函数名称:Right_Motor_menu
**  函数功能:右电机控制菜单，用来改变右电机的参数
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void Right_Motor_menu(void)
{
    switch(Parameter2No)
    {
    case 1: {

        if(Read_Flag)
        {
            Read_Flag=0;
            Read_Right_motor();
            OLED_CLS();
            OLED_P6x8Str(40,0,"Read ok");
            systick_delay_ms(1000);
        }
        state=1;
    }
    break;
    case 2: {

        if(Save_Flag)
        {

            Save_Flag=Save_Right_motor();
            OLED_CLS();
            if(Save_Flag)
            {
                OLED_P6x8Str(40,0,"save error");
                Save_Flag=0;
            }
            else	{
                OLED_P6x8Str(40,0,"save ok");
            }
            systick_delay_ms(1000);

            EnableInterrupts;	//开总中断
        }

        state=1;

    }
    break ;

    case 3: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Right_pid.kp,0.1);
    }
    break ;
    case 4: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Right_pid.ki,0.1);
    }
    break ;
    case 5: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Right_pid.kd,0.1);
    }
    break ;
    case 6: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Right_pid.kvff,0.1);
    }
    break ;
    case 7: {
        ChangeParameterVal(pfloat,&Motor_control.Motor_Right_pid.kaff,0.1);
    }
    break ;


    default :
        break ;
    }
    Show_Right_Motor_PID();

}
/***********************************************************************
**  函数名称:Servo_menu
**  函数功能:方向控制菜单，改变方向参数
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void Servo_menu(void)
{
 switch(Parameter2No)
    {
    case 1: {

        if(Read_Flag)
        {
            Read_Flag=0;
            Read_Servo();
            OLED_CLS();
            OLED_P6x8Str(40,0,"Read ok");
            systick_delay_ms(1000);
        }
        state=1;
    }
    break;
    case 2: {

        if(Save_Flag)
        {

            Save_Flag=Save_Servo();
            OLED_CLS();
            if(Save_Flag)
            {
                OLED_P6x8Str(40,0,"save error");
                Save_Flag=0;
            }
            else	{
                OLED_P6x8Str(40,0,"save ok");
            }
            systick_delay_ms(1000);

            EnableInterrupts;	//开总中断
        }

        state=1;

    }
    break ;

    case 3: {
        ChangeParameterVal(pfloat,&Servo.kp ,0.1);
    }
    break ;
    case 4: {
        ChangeParameterVal(pfloat,&Servo.ki,0.1);
    }
    break ;
    case 5: {
        ChangeParameterVal(pfloat,&Servo.kd,0.1);
    }
    break ;
    case 6: {
        ChangeParameterVal(pfloat,&Servo.distance_err_max_val,0.1);
    }
    break ;
    case 7: {
        ChangeParameterVal(pfloat,&Servo.distance_err_d_max_val,0.1);
    }
    break ;
		case 8: {
        ChangeParameterVal(pfloat,&Servo.max_dis_err,0.1);
    }
    break ;
		case 9: {
        ChangeParameterVal(pfloat,&Servo.max_dis_err_d,0.1);
    }
    break ;
		


    default :
        break ;
    }
    Show_Servo_PID();

}
/***********************************************************************
**  函数名称:Speed_menu
**  函数功能:速度参数控制
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/

void Speed_menu(void)
{
 switch(Parameter2No)
    {
    case 1: {

        if(Read_Flag)
        {
            Read_Flag=0;
            Read_Speed();
            OLED_CLS();
            OLED_P6x8Str(40,0,"Read ok");
            systick_delay_ms(1000);
        }
				
        state=1;
    }
    break;
    case 2: {

        if(Save_Flag)
        {

            Save_Flag=Save_Speed();
            OLED_CLS();
            if(Save_Flag)
            {
                OLED_P6x8Str(40,0,"save error");
                Save_Flag=0;
            }
            else	{
                OLED_P6x8Str(40,0,"save ok");
            }
            systick_delay_ms(1000);

            EnableInterrupts;	//开总中断
        }

        state=1;

    }
    break ;

    case 3: {
        ChangeParameterVal(puint16_t,&Speed.zhidao_speed_val ,1);
    }                      
    break ;                
    case 4: {              
        ChangeParameterVal(puint16_t,&Speed.wandao_speed_val,1);
    }                      
    break ;                
    case 5: {              
        ChangeParameterVal(puint16_t,&Speed.cross_speed_val,1);
    }                      
    break ;                
    case 6: {              
        ChangeParameterVal(puint16_t,&Speed.shizi_speed_val,1);
    }                     
    break ;               
    case 7: {             
        ChangeParameterVal(puint16_t,&Speed.ramp1_speed_val,1);
    }                      
    break ;                
		case 8: {              
        ChangeParameterVal(puint16_t,&Speed.ramp2_speed_val,1);
    }                      
    break ;                
		case 9: {              
        ChangeParameterVal(puint16_t,&Speed.speed_high_mode,1);
    }  
		break ;                    
		 case 10: {            
        ChangeParameterVal(puint16_t,&Speed.high_speed_dis,1);
    }                      
    break ;                
		case 11: {             
        ChangeParameterVal(puint16_t,&Speed.stop_car_enable,1);
    }                      
    break ;                
		case 12: {             
        ChangeParameterVal(puint16_t,&Speed.test_time,1);
    }
    break ;
		case 13: {             
        ChangeParameterVal(puint16_t,&Speed.set_speed_val,1);
    }
    break ;
		


    default :break ;
        
    }
    Show_speed();


}
/***********************************************************************
**  函数名称:Change_UI
**  函数功能:调参UI	
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/
void Change_UI(void)
{
/////////   显示一级菜单内容 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(state == 0) {

        Change_Main_menu();
    }
/////////   显示二级菜单内容 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if(state == 1) {

        switch(ParameterNo)
        {

        case 1: { 				//左电机PID的参数
            Show_Left_Motor_PID();
        }
        break;
        case 2 : {
            Show_Right_Motor_PID();
        }
        break;
        case 3 : {
            Show_Servo_PID();

        }
        break;

        case 4 : {
            Show_speed();

        }
        break;
        case 5 : {				//手动输出电机

        } break;

        case 6 : {
        } break;

        case 7 : {
        } break;

        default :
            break ;

        }

    }

    else if(state == 2) {

        if(ParameterNo==1)

        {
				Left_Motor_menu();

        }

        else if(ParameterNo==2)
        {
					Right_Motor_menu();
        }
        else if(ParameterNo==3)
        {
		Servo_menu();

        }
				else if(ParameterNo==4)
				{
				Speed_menu();
				}

    }


}


/***********************************************************************
**  函数名称:OLED_ConfigParameter
**  函数功能:总UI
**  输 入 值:
**  返 回 值:
**  其他说明:
**  日    期:
***************************************************************************/
void OLED_ConfigParameter(void) {

    ConfigParameterCore(); //调参界面的核心，负责控制光标的位置，各级菜单之间切换
    if(Key_Flag1)
    {
        Show_UI();
    }
    else if (Key_Flag2)
    {
        
    }
    else if(Key_Flag3)
    {

    }
    else if(Key_Flag4)
    {
			Change_UI();
    }
    else {

    }
}











