#include "headfile.h"
#include "MYUI.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//配置系统在线调参界面核心函数
uint8_t cursor_x = 0,cursor_y = 0; //光标的X,Y坐标
int8_t state = 0; //调参状态 0 1 2 对应处于主菜单 一级子菜单和二级子菜单三个状态
int8_t ParameterNo = 1; //主菜单参数号
int8_t Parameter2No = 1; //一级子菜单的参数号

//通过修改这个使能菜单数组，可以设置某个参数号下是否能有二级菜单。1表示有二级菜单，0表示没有
uint8_t en_sec_menu[7]= {0,1,1,1,1,1,0}; //使能二级子菜单，调参


uint32_t kkk=0;

uint8_t precision_init_flag=0;
double precision;

extern float sum_16_34;
extern float sub_25;
extern uint16 ad_avr_val[10];

extern	uint16_t	Sensor_Time;
extern	uint16_t	Direction_Time;
extern	uint16_t	Speed_Time;
extern	uint16_t	Gyro_Time;
extern	uint16_t	Car_speed;
extern	uint16_t 	Ramp_speed;
extern	int8_t 		TURN_Para;
extern	uint16_t 	TURN_LOSE_VAL;
extern  uint16      MOTOR_DUTY_LIMIT;

extern uint8 circle_turn_val[4];
extern char circle_turn_direction[4];

//uint8 cirle_NO1_R=0;

//调参界面中：
//上下可以切换选中的参数
//按下右键与中键确定选择，左键返回
//在一级菜单时长按中键初始化液晶屏
//这个时候可以通过上、下键修改对应的参数，再按一次中心键 光标重新变回 '>' 参数设置完成，光标又可以通过上、下键选择参数
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigParameterCore() {

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
		else if(ParameterNo < 0) ParameterNo = 7;
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
        if(Parameter2No > 7) Parameter2No = 1;
        else if(Parameter2No < 1) Parameter2No = 7;
    }

    //在这修改  ParameterNo 的范围可以配置哪些一级参数号拥有二级菜单 如果改为 ParameterNo < 15 则所有的一级菜单都拥有二级界面
    //有一些标志位需要经常使用，但是每次进入二级菜单又太麻烦的时候，可以留几个一级界面的位置，直接在一级界面修改标志位，而不进入二级菜单。
   
        //更改状态进入二级菜单
        if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc)||
		   (Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
        {
            state ++;
        }
    
    if((Key_Inquire_data(Key_Middle_Read) == Key_bit_Drop))// || (Key_Inquire_data(Key_Middle_Read) == Key_bit_Acc)
            state--;
    ///////菜单限制
		if((en_sec_menu[ParameterNo-1]==0)&&(state>1))
		state=1;
		if(state>2)state=2;
		
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
    }
///////////////////////////////////// 光标位置确定结束 ///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////// 显示光标  ///////////////////////////////////////////////////////////////////////////////////////////////////
    //显示光标
    if(state == 0)
        Cache_OLED_P6x8Str(cursor_x,cursor_y,">");
    else if((state == 1)&&((en_sec_menu[ParameterNo-1])))
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
//修改：通过按键来修改其精度
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ChangeParameterVal(parameter_type ptype,void* ppara,double precision) {

    if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
    {

        switch(ptype)
        {
        case pint8_t:
            *(( int8_t *  )ppara) += precision;
            break;
        case puint8_t:
            *(( uint8_t * )ppara) += precision;
            break;
        case pint16_t:
            *(( int16_t * )ppara) += precision;
            break;
        case puint16_t:
            *(( uint16_t *)ppara) += precision;
            break;
        case pint32_t:
            *(( int32_t * )ppara) += precision;
            break;
        case puint32_t:
            *(( uint32_t *)ppara) += precision;
            break;
        case pfloat:
            *(( float *   )ppara) += precision;
            break;
        case pdouble:
            *(( double *  )ppara) += precision;
            break;
        }
    }
    else if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc)) {

        switch(ptype)
        {
        case pint8_t:
            *(( int8_t *  )ppara) -= precision;
            break;
        case puint8_t:
            *(( uint8_t * )ppara) -= precision;
            break;
        case pint16_t:
            *(( int16_t * )ppara) -= precision;
            break;
        case puint16_t:
            *(( uint16_t *)ppara) -= precision;
            break;
        case pint32_t:
            *(( int32_t * )ppara) -= precision;
            break;
        case puint32_t:
            *(( uint32_t *)ppara) -= precision;
            break;
        case pfloat:
            *(( float *   )ppara) -= precision;
            break;
        case pdouble:
            *(( double *  )ppara) -= precision;
            break;
        }
    }
}

//double ChangePrecisionVal(double precision,double min_pre,double max_pre)
//{
//	if(min_pre>max_pre)
//		return precision;
//	if((Key_Inquire_data(Key_Left_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Left_Read) == Key_bit_Acc))
//	{
//		precision*=10;
//		precision>max_pre? precision = max_pre:0;
//	}
//	if((Key_Inquire_data(Key_Right_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Right_Read) == Key_bit_Acc))
//	{
//		precision/=10;
//		precision<min_pre? precision = max_pre:0;
//	}
//	return precision;
//}

//修改某标志位的值（一般用于没有二级菜单的位置）
void ChangeFlagVal(uint8 *flag,uint8 min,uint8 max) {

    if(Key_Inquire_data(Key_Middle_Read) == Key_bit_Drop)
        (*flag)++;

    if(*flag > max) *flag = min;
    else if(*flag < min) *flag = max;
}

/***********************************
显示参数
环数值
坡数值
电感数值
电感数值图形化显示
电感数值最大值显示
电感最大值
陀螺仪数值
************************************/

void Show_Main_menu(void)
{
	Cache_OLED_P6x8Str(20,0,"show_param ");
	Cache_OLED_P6x8floatNum(50,0,Power_V);
	Cache_OLED_P6x8Str(7,1,"1.Circle");
	Cache_OLED_P6x8Str(7,2,"2.Motor_PID");
	Cache_OLED_P6x8Str(7,3,"3.SPEED");
	Cache_OLED_P6x8Str(7,4,"4.Servo_PID");
	Cache_OLED_P6x8Str(7,5,"5.set");
	Cache_OLED_P6x8Str(7,6,"6.sen");
	Cache_OLED_P6x8Str(7,7,"7.show_Histogram");

}
void show_sensor(void)
{
//	Cache_OLED_P6x8Str(6,0,"circle1_R: ");
//	if(circle_turn_val[0] == 160)		//160 70cm&& 180 50cm
//		OLED_printf(6,0,"circle1_R:%s","70cm");
//	else
//		OLED_printf(6,0,"circle1_R:%s","50cm");
	OLED_printf(6,0,"circle1_R: %d",circle_turn_val[0]);
	if(circle_turn_direction[0] == 1)		//160 70cm&& 180 50cm
		OLED_printf(6,1,"circle1_D:%s","right");
	else
		OLED_printf(6,1,"circle1_D:%s","left");
		
//	if(circle_turn_val[1] == 160)		//160 70cm&& 180 50cm
//		OLED_printf(6,2,"circle2_R:%s","70cm");
//	else
//		OLED_printf(6,2,"circle2_R:%s","50cm");
	OLED_printf(6,2,"circle2_R: %d",circle_turn_val[1]);
	if(circle_turn_direction[1] == 1)		//160 70cm&& 180 50cm
		OLED_printf(6,3,"circle2_D:%s","right");
	else
		OLED_printf(6,3,"circle2_D:%s","left");
		
//	if(circle_turn_val[2] == 160)		//160 70cm&& 180 50cm
//		OLED_printf(6,4,"circle3_R:%s","70cm");
//	else
//		OLED_printf(6,4,"circle3_R:%s","50cm");
	OLED_printf(6,4,"circle3_R: %d",circle_turn_val[2]);
	if(circle_turn_direction[2] == 1)		//160 70cm&& 180 50cm
		OLED_printf(6,5,"circle3_D:%s","right");
	else
		OLED_printf(6,5,"circle3_D:%s","left");
		
//	if(circle_turn_val[3] == 160)		//160 70cm&& 180 50cm
//		OLED_printf(6,6,"circle4_R:%s","70cm");
//	else
//		OLED_printf(6,6,"circle4_R:%s","50cm");
	OLED_printf(6,6,"circle4_R: %d",circle_turn_val[3]);
	if(circle_turn_direction[3] == 1)		//160 70cm&& 180 50cm
		OLED_printf(6,7,"circle4_D:%s","right");
	else
		OLED_printf(6,7,"circle4_D:%s","left");
	
	
//	Cache_OLED_Rectangle(0,  (63-((uint16_t)(ad_avr_val[1])*63/4095)),  8,   63);
//	Cache_OLED_Rectangle(14, (63-((uint16_t)(Motor_control.Motor_Left_pid.output*63/1000))),  22,  63);
//	Cache_OLED_Rectangle(84, (63-((uint16_t)(Motor_control.Motor_Right_pid.output*63/1000))),  92,  63);
//	Cache_OLED_Rectangle(98, (63-((uint16_t)(ad_avr_val[6])*63/4095)),  106, 63);
//	Cache_OLED_P6x8Num(0,0,Motor_control.Motor_Left_pid.present_value[0]);
//	Cache_OLED_P6x8Num(0,1,Sensor.once_uni_ad[2]);
//	Cache_OLED_P6x8Num(90,0,Motor_control.Motor_Right_pid.present_value[0]);
//	Cache_OLED_P6x8Num(90,1,Sensor.once_uni_ad[6]);
////        Cache_OLED_P6x8Num(90,0,ad_avr_val[2]);
//        
//	Cache_OLED_P6x8floatNum(40,0,Servo.error[0]);
//	Cache_OLED_P6x8floatNum(40,1,Sensor.once_uni_ad[4]);
//	Cache_OLED_P6x8floatNum(40,2,sum_16_34);

}

void show_Motor_PID(void)
{

	Cache_OLED_P6x8Str(25,0,"Motor_PID ");

	OLED_printf(6,1,"kp:%.3f",Motor_control.Motor_Left_pid.kp);
	OLED_printf(6,2,"ki:%.1f",Motor_control.Motor_Left_pid.ki);
	OLED_printf(6,3,"kd:%.1f",Motor_control.Motor_Left_pid.kd);
	OLED_printf(6,4,"kvff:%.1f",Motor_control.Motor_Left_pid.kvff);
	OLED_printf(6,5,"kaff:%.1f",Motor_control.Motor_Left_pid.kaff);

}
void show_SPEED(void)
{
	Cache_OLED_P6x8Str(25,0,"SPEED_SET ");
	OLED_printf(6,1,"SPEED: %d",Car_speed);
	OLED_printf(6,2,"TURN_P:%d",TURN_Para);
	OLED_printf(6,3,"TURN_L:%d",TURN_LOSE_VAL);
	OLED_printf(6,4,"M_D_L: %d",MOTOR_DUTY_LIMIT);
	OLED_printf(6,5,"Ramp_s:%d",Ramp_speed);
//	OLED_printf(6,2,"ki:%.1f",Motor_control.Motor_Right_pid.ki);
//	OLED_printf(6,3,"kd:%.1f",Motor_control.Motor_Right_pid.kd);
//	OLED_printf(6,4,"kvff:%.1f",Motor_control.Motor_Right_pid.kvff);
//	OLED_printf(6,5,"kaff:%.1f",Motor_control.Motor_Right_pid.kaff);
}
void show_Servo_PID(void)
{
	Cache_OLED_P6x8Str(25,0,"Program Time ");
	OLED_printf(6,1,"Sensor:     %d us",Sensor_Time);
	OLED_printf(6,2,"Direction:  %d us",Direction_Time);
	OLED_printf(6,3,"Speed:      %d us",Speed_Time);
	OLED_printf(6,4,"Gyro:       %d us",Gyro_Time);
	OLED_printf(6,5,"Remaining:	 %d us",(1000-Sensor_Time-Direction_Time-Speed_Time-Gyro_Time));
}
void show_8L(void)
{
		Cache_OLED_P6x8Str(40,0,"8L_show");
	
		Cache_OLED_P6x8Str(0,1,"1.");
		Cache_OLED_P6x8Str(64,1,"2.");
		Cache_OLED_P6x8Str(0,2,"3.");
		Cache_OLED_P6x8Str(64,2,"4.");
		Cache_OLED_P6x8Str(0,3,"5.");
//		Cache_OLED_P6x8Str(64,3,"6.");
//		Cache_OLED_P6x8Str(0,4,"7.");
//		Cache_OLED_P6x8Str(64,4,"8.");
		
		Cache_OLED_P6x8Num(12,1,ad_avr_val[1]);
		Cache_OLED_P6x8Num(76,1,ad_avr_val[2]);
		Cache_OLED_P6x8Num(12,2,ad_avr_val[3]);
		Cache_OLED_P6x8Num(76,2,ad_avr_val[4]);
		Cache_OLED_P6x8Num(12,3,ad_avr_val[5]);
//		Cache_OLED_P6x8Num(76,3,ad_avr_val[6]);
//		Cache_OLED_P6x8Num(12,4,Adc.ad_max_val[1]);
//		Cache_OLED_P6x8Num(76,4,Adc.ad_max_val[2]);
//		Cache_OLED_P6x8Num(12,5,Adc.ad_max_val[3]);
//		Cache_OLED_P6x8Num(76,5,Adc.ad_max_val[6]);
//		Cache_OLED_P6x8Num(12,6,ad_avr_val[7]);
}






//调参界面

void show_Histogram(void)	
{
	Cache_OLED_Rectangle( 0,  (63-((uint16_t)(ad_avr_val[1])*63/4095)),  8,    63);
	Cache_OLED_Rectangle(16,  (63-((uint16_t)(ad_avr_val[2])*63/4095)),  24,   63);
//	Cache_OLED_Rectangle(32,  (63-((uint16_t)(ad_avr_val[3])*63/4095)),  40,   63);
	Cache_OLED_Rectangle(48,  (63-((uint16_t)(ad_avr_val[3])*63/4095)),  56,   63);
//	Cache_OLED_Rectangle(64,  (63-((uint16_t)(ad_avr_val[5])*63/4095)),  72,   63);
//	Cache_OLED_Rectangle(80,  (63-((uint16_t)(ad_avr_val[4])*63/4095)),  88,   63);
	Cache_OLED_Rectangle(96,  (63-((uint16_t)(ad_avr_val[4])*63/4095)),  104,  63);
	Cache_OLED_Rectangle(112, (63-((uint16_t)(ad_avr_val[5])*63/4095)),  120,  63);
	
//	Cache_OLED_P6x8Num(0,7,1);
//	Cache_OLED_P6x8Num(16,7,2);
//	Cache_OLED_P6x8Num(32,7,3);
//	Cache_OLED_P6x8Num(48,7,4);
//	Cache_OLED_P6x8Num(64,7,5);
//	Cache_OLED_P6x8Num(80,7,6);
//	Cache_OLED_P6x8Num(96,7,7);
//	Cache_OLED_P6x8Num(112,7,8);

}
void OLED_ConfigParameter(void) {

    ConfigParameterCore(); //调参界面的核心，负责控制光标的位置，各级菜单之间切换

/////////   显示一级菜单内容 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(state == 0) {
		Show_Main_menu();
    }
/////////   显示二级菜单内容 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if(state == 1) {
		precision_init_flag = 1;
        switch(ParameterNo)
        {	           
				case 1: { 				//左电机PID的参数
							show_sensor();
						}	break;
				case 2 :{		
							show_Motor_PID();
						}	break;
				case 3 :{				//图形显示
							show_SPEED();
						}	break;
				case 4 :{				
							show_Servo_PID();					
						}	break;
				case 5 :{				//手动输出电机
//							Cache_OLED_P6x8Str(8,1,"R");
//							Cache_OLED_P6x8floatNum(40,1,Motor_control.Motor_Right_pid.output);
//							Cache_OLED_P6x8Str(8,2,"L ");
//							Cache_OLED_P6x8floatNum(40,2,Motor_control.Motor_Left_pid.output);
//							Cache_OLED_P6x8Str(8,3,"Flag ");	
				//			Cache_OLED_P6x8Num(40,3,flag);								
							show_sensor();											}break;
					
				case 6 :{
							show_8L();				
						}break;
				
				case 7 :{
							show_Histogram();
						}break;
			
				default :break ;
					
				}

    }
		
		else if(state == 2){
			
		if(ParameterNo==1)
		
		{
//				switch(Parameter2No)
//				{
//					case 0:{
//						ChangeParameterVal(puint8_t,&circle_turn_val[0],20);
//						if(circle_turn_val[0]>180)
//							circle_turn_val[0] = 160;
//						if(circle_turn_val[0]<160)
//							circle_turn_val[0]=180;
//							show_sensor();
//						
//					}	break;
//					case 1:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[0],1);
//						if(circle_turn_direction[0]>1)
//							circle_turn_direction[0] = 0;
//						if(circle_turn_direction[0]<0)
//							circle_turn_direction[0]=1;
//							show_sensor();
//					}	break;
//					case 2:{
//						ChangeParameterVal(puint8_t,&circle_turn_val[1],20);
//						if(circle_turn_val[1]>180)
//							circle_turn_val[1] = 160;
//						if(circle_turn_val[1]<160)
//							circle_turn_val[1]=180;
//						show_sensor();
//					}	break;
//					case 3:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[1],1);
//						if(circle_turn_direction[1]>1)
//							circle_turn_direction[1] = 0;
//						if(circle_turn_direction[1]<0)
//							circle_turn_direction[1]=1;
//							show_sensor();
//					}	break;
//					case 4:{
//						ChangeParameterVal(puint8_t,&circle_turn_val[2],20);
//						if(circle_turn_val[2]>180)
//							circle_turn_val[2] = 160;
//						if(circle_turn_val[2]<160)
//							circle_turn_val[2]=180;
//							show_sensor();
//						
//					}	break;
//					case 5:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[0],1);
//						if(circle_turn_direction[2]>1)
//							circle_turn_direction[2] = 0;
//						if(circle_turn_direction[2]<0)
//							circle_turn_direction[2]=1;
//							show_sensor();
//					}	break;
//					case 6:{
//						ChangeParameterVal(puint8_t,&circle_turn_val[3],20);
//						if(circle_turn_val[3]>180)
//							circle_turn_val[3] = 160;
//						if(circle_turn_val[3]<160)
//							circle_turn_val[3]=180;
//						show_sensor();
//					}	break;
//					case 7:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[3],1);
//						if(circle_turn_direction[3]>1)
//							circle_turn_direction[3] = 0;
//						if(circle_turn_direction[3]<0)
//							circle_turn_direction[3]=1;
//							show_sensor();
//					}	break;
//					default:break ;					
//				}
			}
		
			else if(ParameterNo==2)
			{
					switch(Parameter2No)
				{
					
						
					case 1:{
//						if(precision_init_flag)
//						{
//							precision_init_flag=0;
//							precision = 0.1;
//						}
//						precision = ChangePrecisionVal(precision,0.1,10);
						ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kp,0.1);
						Motor_control.Motor_Right_pid.kp = Motor_control.Motor_Left_pid.kp;
						show_Motor_PID();									
							
					}	break ;
					case 2:{
						
				ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.ki,0.1);
				Motor_control.Motor_Right_pid.ki = Motor_control.Motor_Left_pid.ki;
				show_Motor_PID();
						
							
					}	break ;
					case 3:{
				ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kd,0.1);
				Motor_control.Motor_Right_pid.kd = Motor_control.Motor_Left_pid.kd;
				show_Motor_PID();			
							
					}	break ;
										case 4:{
						
				ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kvff,0.1);
				Motor_control.Motor_Right_pid.kvff = Motor_control.Motor_Left_pid.kvff;
				show_Motor_PID();
						
							
					}	break ;
					case 5:{
				ChangeParameterVal(pfloat,&Motor_control.Motor_Left_pid.kaff,0.1);
				Motor_control.Motor_Right_pid.kaff = Motor_control.Motor_Left_pid.kaff;
				show_Motor_PID();			
							
					}	break ;
					
					
					default :break ;
			
			}
		}
			else if(ParameterNo==3)
			{
					switch(Parameter2No)
				{
					case 1:{
						ChangeParameterVal(puint16_t,&Car_speed,1);
						show_SPEED();									
							
					}	break ;
					case 2:{
						
				ChangeParameterVal(pint8_t,&TURN_Para,1);
				show_SPEED();
						
							
					}	break ;
					case 3:{
				ChangeParameterVal(puint16_t,&TURN_LOSE_VAL,1);
				show_SPEED();			
							
					}	break ;
				case 4:{
						
				ChangeParameterVal(puint16_t,&MOTOR_DUTY_LIMIT,25);
				show_SPEED();
						
							
					}	break ;
					case 5:{
						
					ChangeParameterVal(puint16_t,&Ramp_speed,1);
					show_SPEED();
						
							
					}	break ;
//					case 5:{
////				ChangeParameterVal(pfloat,&Motor_control.Motor_Right_pid.kaff,0.1);
////				show_Right_Motor_PID();			
//					}	break ;
//					
					default :break ;
//			
				}}
				else if(ParameterNo==5)
				{
				switch(Parameter2No)
				{
					case 0:{
						ChangeParameterVal(puint8_t,&circle_turn_val[0],1);
						if(circle_turn_val[0]>200)
							circle_turn_val[0] = 160;
						if(circle_turn_val[0]<150)
							circle_turn_val[0]=180;
							show_sensor();
					}	break;
					case 1:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[0],1);
//						if(circle_turn_direction[0]>1)
//							circle_turn_direction[0] = 0;
//						if(circle_turn_direction[0]<0)
//							circle_turn_direction[0]=1;
							if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
							{
								circle_turn_direction[0] = 1;
							}
							if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
							{
								circle_turn_direction[0] = 0;
							}
							show_sensor();
					}	break;
					case 2:{
						ChangeParameterVal(puint8_t,&circle_turn_val[1],1);
						if(circle_turn_val[1]>200)
							circle_turn_val[1] = 160;
						if(circle_turn_val[1]<150)
							circle_turn_val[1]=180;
						show_sensor();
					}	break;
					case 3:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[1],1);
//						if(circle_turn_direction[1]>1)
//							circle_turn_direction[1] = 0;
//						if(circle_turn_direction[1]<0)
//							circle_turn_direction[1]=1;
							if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
							{
								circle_turn_direction[1] = 1;
							}
							if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
							{
								circle_turn_direction[1] = 0;
							}
							show_sensor();
					}	break;
					case 4:{
						ChangeParameterVal(puint8_t,&circle_turn_val[2],1);
						if(circle_turn_val[2]>200)
							circle_turn_val[2] = 160;
						if(circle_turn_val[2]<160)
							circle_turn_val[2]=150;
							show_sensor();
						
					}	break;
					case 5:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[0],1);
//						if(circle_turn_direction[2]>1)
//							circle_turn_direction[2] = 0;
//						if(circle_turn_direction[2]<0)
//							circle_turn_direction[2]=1;
							if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
							{
								circle_turn_direction[2] = 1;
							}
							if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
							{
								circle_turn_direction[2] = 0;
							}
							show_sensor();
					}	break;
					case 6:{
						ChangeParameterVal(puint8_t,&circle_turn_val[3],1);
						if(circle_turn_val[3]>200)
							circle_turn_val[3] = 160;
						if(circle_turn_val[3]<150)
							circle_turn_val[3]=180;
						show_sensor();
					}	break;
					case 7:{
//						ChangeParameterVal(pint8_t,&circle_turn_direction[3],1);
//						if(circle_turn_direction[3]>1)
//							circle_turn_direction[3] = 0;
//						if(circle_turn_direction[3]<0)
//							circle_turn_direction[3]=1;
						if((Key_Inquire_data(Key_Front_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Front_Read) == Key_bit_Acc))
						{
							circle_turn_direction[3] = 1;
						}
						if((Key_Inquire_data(Key_Back_Read) == Key_bit_Drop) || (Key_Inquire_data(Key_Back_Read) == Key_bit_Acc))
						{
							circle_turn_direction[3] = 0;
						}
						show_sensor();
					}	break;
					default:break ;					
				}
//			
			}
			
			}				
			
			
			
			
	
}











