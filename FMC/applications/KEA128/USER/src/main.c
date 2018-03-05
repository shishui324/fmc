#include "headfile.h"
#include "stdbool.h"
#include "math.h"

#include "Car_Key.h"

//n/6.16


uint32_t time_1ms = 0;
uint32_t time_10ms = 0;
uint32_t time_20ms = 0;
uint32_t time_50ms = 0;
uint32_t time_100ms = 0;
uint32_t time_200ms = 0;
uint32_t time_500ms = 0;
uint32_t time_1s = 0;
uint32_t time_2s = 0;
uint32_t time_5s = 0;
uint32_t time_10s = 0;





void time_10ms_serve(void);
void time_20ms_serve(void);
void time_50ms_serve(void);
void time_100ms_serve(void);
void time_200ms_serve(void);
void time_500ms_serve(void);
void time_1s_serve(void);
void time_2s_serve(void);
void time_5s_serve(void);
void time_10s_serve(void);



void get_adc_int_value(void);
void deal_sensor(void);
void servo_pid_caculate(void);
void control(void);
void motor_set(void);
void speed_control(void);

#define DIR_CONTROL 1


//#define KEY_1_PIN C2
//#define KEY_2_PIN B5
#define KEY1 gpio_get(C2)
#define KEY2 gpio_get(B5)

#define speed_duty  150

#define SENSOR_NUM  2   //定义电感数量
#define AD_MAX_VAL 4095.0 //AD的最大值

#define MOTOR_DUTY_MAX       300      //ÏÞÖÆÕ¼¿Õ±È30%£¬ÒÔ·ÀµçÁ÷¹ý´óÉÕÇý¶¯
#define MOTOR_DUTY_MIN       -300

float Speed_Ratio = 6.16;
int8_t KEY_FLAG = 0;
bool STOP_CAR_FLAG = true;//停车信号


uint32 update_dis1cm_encoder;

/*************speed_control***************/
uint8_t g_fLeftMotorSpeed = 0;	//左电机速度(测量量)		cm/s
uint8_t g_fRightMotorSpeed = 0;	//右电机速度(测量量)		cm/s
//姿态数据
uint8_t g_fCarSpeed = 0;				//速度(测量)
uint8_t g_fLeft_Tyre_Speed = 0;
uint8_t g_fRight_Tyre_Speed = 0;
uint8_t ControlSpeed = 0;				//迟滞速度
uint8_t LeftControlSpeed = 0;				//迟滞速度
uint8_t RightControlSpeed = 0;				//迟滞速度

//速度控制项输出
uint8_t g_fSpeedControlOut = 0;		//速度控制输出
uint8_t g_fLeftMotorOut = 0;			//左电机输出
uint8_t g_fRightMotorOut = 0;			//右电机输出
/*****************************************/

uint16 ad_max_temp[10];
uint16 ad_int_val[10][3];
uint16 ad_avr_temp[5][10];
uint16 ad_mid_val[10]; //AD采样中值
uint32 ad_add_val[10]; //AD采样中值和
uint16 ad_avr_val[10]; //AD采样平均值
float once_uni_ad[SENSOR_NUM+1];  //一次归一化
float twice_uni_ad[SENSOR_NUM+1]; //二次归一化

float sum_12;   //电感12的和

int8_t getCountNum_L;  //获取编码器计数值
int8_t getCountNum_R;  //获取编码器计数值

#define SERVO_LIMIT_VAL 18
//#define SERVO_LIMIT_VAL	400`  //差速限幅  400
int16 R_out_value; //右边pwm输出的值
int16 L_out_value; //左边pwm输出的值

//µç»ú£¬ËÙ¶È
typedef struct
{
    uint8 INH;                    //Èí¼þÊ¹ÄÜ
    int16 set_value_temp;         //Ò£¿ØÉè¶¨ËÙ¶ÈÁÙÊ±±äÁ¿
    int16 output;
    int16 speed_racc;             //µç»ú¼ÓËÙ¶È
    float last_uk;               //ÉÏÒ»´ÎµÄ¿ØÖÆÁ¿Êä³ö¾ø¶ÔÖµ£¬¼´µç»úÕ¼¿Õ±È/10
    float kp;
    float ki;
    float kd;
    float kvff;      //ËÙ¶ÈÇ°À¡ÏµÊý
    float kaff;      //¼ÓËÙ¶ÈÏµÊý
    float delta_uk;
    float out_duty;

    uint16 kvff_param;
    int16 present_value[10];  //±àÂëÆ÷Êµ¼Ê²âµÃÖµ
    int16 error[10];          //Îó²î¶ÓÁÐ
    int16 set_value[10];      //ËÙ¶ÈÉè¶¨¶ÓÁÐ
} Motor_pid_info;            //µç»úPIDËùÐèµÄÐÅÏ¢ÀàÐÍ

typedef struct
{
    Motor_pid_info	Motor_Left_pid;
    Motor_pid_info	Motor_Right_pid;
} Motor_control_info;

void motor_pid_caculate(Motor_pid_info *motor_info);

typedef struct
{
    uint16 zhidao_speed_val; //直道速度
    uint16 wandao_speed_val; //弯道速度
    uint16 cross_speed_val;  //
    uint16 shizi_speed_val;  //Ê®×ÖËÙ¶È
    uint16 zhijiao_speed_val;//Ö±½ÇËÙ¶È
    uint16 speed_high_mode; //¸ßËÙÄ£Ê½Ê¹ÄÜ
    uint16 high_speed_dis;  //¸ßËÙÄ£Ê½¶¨¾à
    uint16 stop_car_enable; //Í£³µ
    uint16 test_time;       //²âÊÔÊ±¼ä
} Speed_info;


typedef struct
{
    float kp;
    float ki;
    float kd;
    int16 output;
    int error[10];

    float  dis1cm_err_store[25];           //1cm道路偏差
    float  distance_err_max_val;           //最大单位偏差 ,遥控可修改
    float  distance_err_d_max_val;         //最大偏差率(偏差微分),可修改
    float  max_dis_err;                    // 最大偏差变化
    float  max_dis_err_d;                  // 最大偏差变化率

} Servo_info;   //差速pid

Servo_info Servo;
Motor_pid_info Motor;
Motor_control_info Motor_control;
Speed_info Speed;

int main(void)
{
    get_clk();              //获取时钟频率 必须执行

//		DisableInterrupts ;                  //禁止中断
    OLED_Init();

//////////////ADC初始化///////////////////////////////////////////////////////////////
    adc_init(ADC0_SE2); //PA6
    adc_init(ADC0_SE3); //PA7
    /////////////////电机初始化/////////////////////////

    ftm_pwm_init(ftm2,ftm_ch0,15*1000,0);         //电机初始化 通道0 15Khz
    ftm_pwm_init(ftm2,ftm_ch1,15*1000,0);         //电机初始化 通道1 15Khz

    ftm_pwm_init(ftm2,ftm_ch2,15*1000,0);         //电机初始化 通道2 15Khz
    ftm_pwm_init(ftm2,ftm_ch3,15*1000,0);         //电机初始化 通道3 15Khz

///////////////编码器初始化//////////////////////////////////////////////////////
    ftm_count_init(ftm0);   //左编码器
    gpio_init(C5,GPI,0);    //DIR初始化
    port_pull(C5);

    ftm_count_init(ftm1);   //右编码器
    gpio_init(H5,GPI,0);    //DIR初始化
    port_pull(H5);

/////////////////////////////////////////////
    gpio_init(B1,GPO,1);    //LED1
		GPIOB->PIDR |= ((uint32)1<<(E5-32));	
    GPIOB->PDDR |= ((uint32)1<<(E5-32));	//将端口设置为输入					Key_Flag1
		GPIOB->PDOR &=~((uint32)1<<(E5-32));
	  GPIOB->PDOR |=((uint32)1<<(E5-32));
    gpio_init(D2,GPO,1);		//LED4

////////////////KBI初始化/////////////////////////////////////////
//		gpio_init(B5,GPI,0);
//		kbi_init(KBI0_P13,IRQ_RISING);//上升沿触发
//
//
//		gpio_init(C2,GPI,0);
//		kbi_init(KBI0_P18,IRQ_RISING);
//		set_irq_priority(KBI0_IRQn,2);					//设置优先级,根据自己的需求设置 可设置范围为 0 - 3
//		enable_irq(KBI0_IRQn);							//打开KBI1_IRQn的中断开关
///////////////按键初始化///////////////////////////////////////////////////////////
//    gpio_init(C2,GPI,0);
//    gpio_init(B5,GPI,0);
//    port_pull(C2);
//    port_pull(B5);
//    gpio_init(C2,GPI,1);    //按键1初始化
////	GPIOA->PIDR &= ~((uint32)1<<C2);	//PTC2取消禁用输入
////	GPIOA->PDDR &= ~((uint32)1<<C2);	//将端口设置为输入或输出 0：输入 1：输出
////	PORT_PUE0 |= (uint32)1<<C2;		//PTC2上拉使能
//
////
//    gpio_init(B5,GPI,1);    //按键2初始化
//

//	Key_Message_Init();

    uart_init(DEBUG_PORT,DEBUG_BAUD);//串口初始化
/////////////////pit初始化//////////////////////////////////////////////
    pit_init_ms(pit0,1);                            //初始化pit0 周期设置为1ms
    set_irq_priority(PIT_CH0_IRQn,0);	            //设置pit0优先级
    enable_irq(PIT_CH0_IRQn);			            //开启pit0中断
    EnableInterrupts;

    Cache_OLED_P6x8Str(0,0,"ALL_INIT");
    Cache_Update_OLED();
    pit_delay_ms(pit1,1000);

    /*******参数初始化*******/
    //速度
    Speed.cross_speed_val = 13;                 //20
    Speed.wandao_speed_val = 11;                //16
    Speed.zhidao_speed_val = 15;                //25

    Servo.kp = 1.0f;
    Servo.kd = 0.2f;   //0.2
    Servo.max_dis_err = 0.0;
    Servo.distance_err_max_val = 12.0;
    Servo.max_dis_err_d = 0.0;
    Servo.distance_err_d_max_val = 5.0;

    Motor.kvff =1.00;            //20
    Motor.kaff = 10.00;           //10
    Motor.kd = 3.00;
    Motor.ki = 2.00;
    Motor.kp = 9.00;            //60

    Motor_control.Motor_Left_pid.kvff = 1.0;
    Motor_control.Motor_Left_pid.kaff = 1.0;
    Motor_control.Motor_Left_pid.kd = 8.0;
    Motor_control.Motor_Left_pid.ki = 5.0;
    Motor_control.Motor_Left_pid.kp = 15.0;

    Motor_control.Motor_Right_pid.kvff = 1.0;
    Motor_control.Motor_Right_pid.kaff = 1.0;
    Motor_control.Motor_Right_pid.kd = 8.0;
    Motor_control.Motor_Right_pid.ki = 5.0;
    Motor_control.Motor_Right_pid.kp = 15.0;   //1 1 3 2 9


    while(1)
    {
        if(time_10ms) {
            time_10ms--;
            time_10ms_serve();
        }
        if(time_20ms) {
            time_20ms--;
            time_20ms_serve();
        }
        if(time_50ms) {
            time_50ms--;
            time_50ms_serve();
        }
        if(time_100ms) {
            time_100ms--;
            time_100ms_serve();
        }
        if(time_200ms) {
            time_200ms--;
            time_200ms_serve();
        }
        if(time_500ms) {
            time_500ms--;
            time_500ms_serve();
        }
        if(time_1s) {
            time_1s--;
            time_1s_serve();
        }
        if(time_2s) {
            time_2s--;
            time_2s_serve();
        }
        if(time_5s) {
            time_5s--;
            time_5s_serve();
        }
        if(time_10s) {
            time_10s--;
            time_10s_serve();
        }
    }
}

void get_adc_int_value(void)    //中值滤波  均值滤波   求取平均值
{
//    uint8 i=0;
//    uint8 k=0;
//    uint16 temp = 0;
//    for(i=0;i<SENSOR_NUM;i++)      //先对其和进行清零
//    {
//        ad_add_val[i]=0;
//    }
//    //中值滤波
//    for(k = 0; k < 3; k++)
//    {
//        for(i = 0; i <3 ;i++)
//        {
//            ad_avr_temp[i][1] = adc_once(ADC0_SE2,ADC_12bit);
//            ad_avr_temp[i][2] = adc_once(ADC0_SE3,ADC_12bit);
//        }
//        for(i = 1;i <= SENSOR_NUM;i++)
//        {
//            if(ad_avr_temp[0][i] > ad_avr_temp[2][i]) //确保0 < 2
//            {
//                temp = ad_avr_temp[0][i];
//                ad_avr_temp[0][i] = ad_avr_temp[2][i];
//                ad_avr_temp[2][i] = temp;
//            }
//            if(ad_avr_temp[0][i] > ad_avr_temp[1][i]) //确保0 < 1
//            {
//                temp = Adc.ad_avr_temp[0][i];
//                ad_avr_temp[0][i] = ad_avr_temp[1][i];
//                ad_avr_temp[1][i] = temp;
//            }
//            if(ad_avr_temp[1][i] > ad_avr_temp[2][i]) //确保1 < 2
//            {
//                temp = Adc.ad_avr_temp[1][i];
//                ad_avr_temp[1][i] = ad_avr_temp[2][i];
//                ad_avr_temp[2][i] = temp;
//            }
//
//            ad_add_val[i] += ad_avr_temp[1][i]; //中值和
//        }
//    }
//
//    for(i = 1; i <= SENSOR_NUM; i++)
//    {
//        ad_avr_val[i] = (uint16)(ad_add_val[i]*0.333);  //均值
//    }
    ad_avr_val[1] = adc_once(ADC0_SE2,ADC_12bit);
    ad_avr_val[2] = adc_once(ADC0_SE3,ADC_12bit);
}

void deal_sensor(void)
{
    char i;
    get_adc_int_value();
//    for(i = 1;i <= SENSOR_NUM ;i++)  //一次归一化处理
//    {
    once_uni_ad[1] = 100*(float)(ad_avr_val[1]/4095.0);//百分比
    once_uni_ad[2] = 100*(float)(ad_avr_val[2]/AD_MAX_VAL);//百分比
//    }
    sum_12 = once_uni_ad[1] + once_uni_ad[2];   //电感12的和
    twice_uni_ad[1] = once_uni_ad[1] / sum_12 ;
    twice_uni_ad[2] = once_uni_ad[2] / sum_12 ;

    for(i = 9; i > 0; i--)
        Servo.error[i] = Servo.error[i-1];
    Servo.error[0] = 10*(twice_uni_ad[1] - twice_uni_ad[2]); //求出电感差值

    update_1cm_error();

}

void servo_pid_caculate(void)           //差速控制pid
{
    //位置式
    Servo.output = (int16)(Servo.kp *Servo.error[0] + Servo.kd*5.0* (Servo.error[0]-Servo.error[9]));
    /***********差方法************/
    if(Servo.output>SERVO_LIMIT_VAL||(sum_12<80.0&&Servo.output>=0))
        Servo.output = SERVO_LIMIT_VAL;
    if(Servo.output<-SERVO_LIMIT_VAL||(sum_12<80.0&&Servo.output<0))
        Servo.output = -SERVO_LIMIT_VAL;
}

void control(void)  //控制函数
{
    /*******************问题*****************
                差速是用算差还是算比
    ****************************************/
    Motor_control.Motor_Left_pid.present_value[0] = getCountNum_L;
    Motor_control.Motor_Right_pid.present_value[0] = getCountNum_R;
    deal_sensor();
#if DIR_CONTROL
    servo_pid_caculate();
#endif
//	speed_control();
    Motor.set_value[0] = 20;
    Motor_control.Motor_Left_pid.set_value[0] = Motor.set_value[0] - Servo.output;
    Motor_control.Motor_Right_pid.set_value[0] = Motor.set_value[0] + Servo.output;
//	motor_pid_caculate(&Motor);
    motor_pid_caculate(&Motor_control.Motor_Left_pid);
    motor_pid_caculate(&Motor_control.Motor_Right_pid);

    L_out_value = Motor_control.Motor_Left_pid.output;
    R_out_value = Motor_control.Motor_Right_pid.output;
    /********差方法********/
//    R_out_value = Motor.output + Servo.output;
//    L_out_value = Motor.output - Servo.output;
    /**********************/

    /********比方法*********
    Servo.output = R_out_value/L_out_value；
    R_out_value+L_out_value；= 400;

    ***********************/
    motor_set();

}

void motor_set(void)
{
    if(STOP_CAR_FLAG == true)
    {
        ftm_pwm_duty(ftm2,ftm_ch0,0);
        ftm_pwm_duty(ftm2,ftm_ch1,0);
        ftm_pwm_duty(ftm2,ftm_ch2,0);
        ftm_pwm_duty(ftm2,ftm_ch3,0);

        Motor.set_value[0] = 0;
    }
    else
    {
        if(R_out_value>0)
        {
            if(R_out_value<100)
                R_out_value = 100;
            if(R_out_value>400)
                R_out_value = 400;
            ftm_pwm_duty(ftm2,ftm_ch3,R_out_value); //右边电机输出
            ftm_pwm_duty(ftm2,ftm_ch2,0); //右边电机输出
        }
        else if(R_out_value == 0)
        {
            ftm_pwm_duty(ftm2,ftm_ch3,0); //右边电机输出
            ftm_pwm_duty(ftm2,ftm_ch2,0); //右边电机输出
        }
        else
        {
            if(R_out_value>-100)
                R_out_value = -100;
            if(R_out_value<-400)
                R_out_value = -400;
            ftm_pwm_duty(ftm2,ftm_ch2,-R_out_value); //右边电机输出
            ftm_pwm_duty(ftm2,ftm_ch3,0); //右边电机输出
        }

        if(L_out_value>0)
        {
            if(L_out_value<100)
                L_out_value = 100;
            if(L_out_value>400)
                L_out_value = 400;
            ftm_pwm_duty(ftm2,ftm_ch0,L_out_value); //左边电机输出
            ftm_pwm_duty(ftm2,ftm_ch1,0); //左边电机输出
        }
        else if(L_out_value == 0)
        {
            ftm_pwm_duty(ftm2,ftm_ch0,0); //左边电机输出
            ftm_pwm_duty(ftm2,ftm_ch1,0); //左边电机输出
        }
        else
        {
            if(L_out_value>-100)
                L_out_value = -100;
            if(L_out_value<-400)
                L_out_value = -400;
            ftm_pwm_duty(ftm2,ftm_ch1,-L_out_value); //左边电机输出
            ftm_pwm_duty(ftm2,ftm_ch0,0); //左边电机输出
        }
//		Motor.set_value[0] = 6;
    }
}

void motor_pid_caculate(Motor_pid_info *motor_info)
{
    motor_info->delta_uk = 0;
    motor_info->out_duty = 0;
    uint8 i = 0;
    for(i=9; i>0; i--)
    {
        motor_info->error[i] = motor_info->error[i-1];
    }
//  motor_info->present_value[0] = (getCountNum_L + getCountNum_R)/2;			//****************CAUTION*******************/
    motor_info->error[0] = motor_info->set_value[0]-motor_info->present_value[0];


    motor_info->speed_racc = 0;   //¼ÆËãµç»ú¼ÓËÙ¶È,¼´ËÙ¶ÈÎ¢·Ö
    for(i=0; i<1; i++)
        motor_info->speed_racc += motor_info->present_value[i];
    for(i=1; i<2; i++)
        motor_info->speed_racc -= motor_info->present_value[i];
    motor_info->delta_uk = (float)(motor_info->kvff*motor_info->kvff_param*(motor_info->set_value[0] - motor_info->set_value[1])
                                   +motor_info->kp*(motor_info->error[0] - motor_info->error[1])
                                   + motor_info->ki*motor_info->error[0]
                                   + motor_info->kd*(motor_info->error[0]-2.0*motor_info->error[1]
                                           + motor_info->error[2])
                                   -motor_info->kaff*motor_info->speed_racc*0.10
                                  );


    /*------------------------ÓöÏÞÏ÷Èõ¿¹»ý·Ö±¥ºÍµÈ´¦Àí-----------------------*/
    if(motor_info->last_uk > MOTOR_DUTY_MAX)  //·¶Î§-1000~1000
        if(motor_info->delta_uk > 0)
            motor_info->delta_uk = 0;//µ±Ç°Ò»Ê±¿Ì¿ØÖÆÁ¿(Õ¼¿Õ±È)ÒÑ¾­´ïµ½×î´óÊ±£¬ÈôÏÖÔÚÔöÁ¿ÎªÕýÔò²»ÀÛ¼Ó
    if(motor_info->last_uk < MOTOR_DUTY_MIN)
        if(motor_info->delta_uk < 0)
            motor_info->delta_uk = 0;//µ±Ç°Ò»Ê±¿Ì¿ØÖÆÁ¿(Õ¼¿Õ±È)ÒÑ¾­´ïµ½×î´óÊ±£¬ÈôÏÖÔÚÔöÁ¿ÎªÕýÔò²»ÀÛ¼Ó
    /*------------------------ÓöÏÞÏ÷Èõ¿¹»ý·Ö±¥ºÍµÈ´¦Àí½áÊø-------------------*/


    motor_info->out_duty = motor_info->last_uk + motor_info->delta_uk;//µ±Ç°ÐèÒªÊä³öµÄÊµ¼ÊÕ¼¿Õ±È

    /*-------------------------ÏÞÖÆ Êµ¼Ê×îºó×ÜÊä³öµÄ·ùÖµ--------------------*/
    if (motor_info->out_duty > MOTOR_DUTY_MAX)
        motor_info->out_duty = MOTOR_DUTY_MAX;
    else if (motor_info->out_duty < MOTOR_DUTY_MIN)
        motor_info->out_duty = MOTOR_DUTY_MIN;
    /*-------------------------ÏÞÖÆÊµ¼Ê×îºó×ÜÊä³öµÄ·ùÖµ½áÊø------------------*/

    motor_info->last_uk = motor_info->out_duty;               //¸üÐÂÉÏÒ»´ÎµÄÊµ¼Ê¿ØÖÆÁ¿Êä³ö
    motor_info->output =(int16)(motor_info->out_duty);     //µ±Ç°Õ¼¿Õ±ÈÊä³ö

    for(i = 9; i > 0; i--) //¸üÐÂËÙ¶ÈÉè¶¨Öµ¶ÓÁÐ
        motor_info->set_value[i] = motor_info->set_value[i-1];

    for(i= 9; i>0; i--)  //¸üÐÂÊµ²âËÙ¶È¶ÓÁÐ
    {
        motor_info->present_value[i] = motor_info->present_value[i-1];
    }

//  motor_set(motor_info->output);

}

void speed_control(void)
{
    uint16 speed_dt = 0;
    uint8 i = 0;
    double dis_err_d_sub_val = 0;                                         //偏差变化率                                               //速度增量

    Servo.max_dis_err = 0;                                               //偏差
    Servo.max_dis_err_d = 0;                                             //偏差变化率

    for(i=0; i<10; i++)                                                   //5cm //寻找最近10cm内单位cm最大偏差
    {
        if(fabs(Servo.dis1cm_err_store[i]) > Servo.max_dis_err)
            Servo.max_dis_err = fabs(Servo.dis1cm_err_store[i]);
    }
    if(Servo.max_dis_err > Servo.distance_err_max_val)                  //最大单位偏差限幅
        Servo.max_dis_err = Servo.distance_err_max_val;

    for(i=0; i<10; i++)                                                   //寻找最近22cm内每12cm最大偏差变化率
    {
        dis_err_d_sub_val = Servo.dis1cm_err_store[i]-Servo.dis1cm_err_store[i+12];
        if(fabs(dis_err_d_sub_val) > Servo.max_dis_err_d)
            Servo.max_dis_err_d = fabs(dis_err_d_sub_val);
    }
    if(sum_12<10);
//  Motor.set_value[0] = 1;
    else if(Servo.max_dis_err_d > Servo.distance_err_d_max_val)              //最大12cm偏差变化率限幅
        Servo.max_dis_err_d = Servo.distance_err_d_max_val;

    speed_dt =  (uint16)((Speed.zhidao_speed_val-Speed.wandao_speed_val)*Servo.max_dis_err/Servo.distance_err_max_val
                         +(Speed.zhidao_speed_val-Speed.cross_speed_val)*Servo.max_dis_err_d/Servo.distance_err_d_max_val);

    if(Speed.cross_speed_val < Speed.wandao_speed_val)           //限制最低速度
    {
        if(speed_dt > (Speed.zhidao_speed_val-Speed.cross_speed_val))
            speed_dt = Speed.zhidao_speed_val-Speed.cross_speed_val;
    }
    else
    {
        if(speed_dt > (Speed.zhidao_speed_val-Speed.wandao_speed_val))
            speed_dt = Speed.zhidao_speed_val-Speed.wandao_speed_val;
    }

    Motor.set_value[0] = Speed.zhidao_speed_val-speed_dt;
}

void update_1cm_error(void)
{
    uint8 i = 0;

    update_dis1cm_encoder += (getCountNum_L + getCountNum_R)/2;;//累加当前编码器测得脉冲数

    if(update_dis1cm_encoder >= 24) //1cm对应75个脉冲数
    {
        update_dis1cm_encoder = 0;
        for(i = 24; i > 0; i--)
            Servo.dis1cm_err_store[i] = Servo.dis1cm_err_store[i-1];
        Servo.dis1cm_err_store[0] = Servo.error[0];
    }
}

//普通计时服务
void time_20ms_serve(void)
{

}

void time_10ms_serve(void)
{
    static uint32_t time_20ms_ct = 0;

    time_20ms_ct += 1;
    if (time_20ms_ct >= 50)
    {
        time_20ms_ct = 0;
    }

    Car_Gather_Data_Key((uint8_t)20);		//Key采集
//	UI_OLED_play(20);							//UI处理

//   Car_Gather_Data_Key(10);


//		if(!((GPIOA->PDIR>>C2)&0x01))
//		{
//			systick_delay_ms(10);
//			if(!KEY1)
//			{
//				KEY_FLAG++;
//				if(KEY_FLAG>=3)
//					KEY_FLAG = 0;
//				while(!KEY1);
//			}
//		}
//		if(!KEY2)
//		{
//			systick_delay_ms(10);
//			if(!KEY2)
//			{
//				STOP_CAR_FLAG = !STOP_CAR_FLAG;
//				while(!KEY2);
//			}
//		}

    if(Key_Inquire_data(Key_Up_Read) == Key_bit_Rise)
    {
        KEY_FLAG++;
        if(KEY_FLAG>=3)
            KEY_FLAG = 0;
    }
    if(Key_Inquire_data(Key_Down_Read) == Key_bit_Rise)
    {
        STOP_CAR_FLAG = !STOP_CAR_FLAG;
    }

    switch(KEY_FLAG)
    {
    case 0:
    {

        Cache_OLED_P6x8Num(0,0,getCountNum_L);
        Cache_OLED_P6x8Num(90,0,getCountNum_R);
//        Cache_OLED_P6x8Num(90,0,ad_avr_val[2]);

        Cache_OLED_P6x8floatNum(40,0,Servo.error[0]);
        Cache_OLED_P6x8floatNum(40,1,once_uni_ad[1]);
        Cache_OLED_P6x8floatNum(40,2,sum_12);
//        Cache_OLED_P6x8Num(90,0,ad_avr_val[2]);

//				Cache_OLED_P6x8floatNum(80,0,(float)(3.24));
        Cache_OLED_Rectangle(0,  (63-((uint16_t)(ad_avr_val[1])*63/4095)),  8,   63);
        Cache_OLED_Rectangle(14, (63-((uint16_t)(L_out_value*63/1000))),  22,  63);
//        Cache_OLED_Rectangle(28, (63-((uint16_t)elec_data.Voltage_V_LL)/100), 36,  63);
//        Cache_OLED_Rectangle(42, (63-(uint16_t)((twice_uni_ad[1])*63)),  50,  63);
//        Cache_OLED_Rectangle(56, (uint16_t)(63-((twice_uni_ad[2])*63)),  64,  63);
//        Cache_OLED_Rectangle(70, (63-((uint16_t)elec_data.Voltage_V_RR)/100), 78,  63);
        Cache_OLED_Rectangle(84, (63-((uint16_t)(R_out_value*63/1000))),  92,  63);
        Cache_OLED_Rectangle(98, (63-((uint16_t)(ad_avr_val[2])*63/4095)),  106, 63);

        if(getCountNum_L>0)
            gpio_set(B1,0);
        else
            gpio_set(B1,1);
    }
    break;
    case 1:
    {
        Cache_OLED_P6x8floatNum(0,0,(float)(getCountNum_L/Speed_Ratio));
        Cache_OLED_P6x8Num(0,1,(int8_t)Servo.output);
    }
    break;
    default:
        break;
    }
    Cache_Update_OLED();
}

void time_50ms_serve(void)
{

}

void time_100ms_serve(void)
{

}

void time_200ms_serve(void)
{

}

void time_500ms_serve(void)
{
gpio_turn(E5);
}

void time_1s_serve(void)
{

}

void time_2s_serve(void)
{

}

void time_5s_serve(void)
{

}

void time_10s_serve(void)
{

}

