#if 0
#include "servo.h"
#include "math.h"
#include "OLED.h"


/*************  成电疾影  **********************/

 Servo_pid_info Servo;



 extern float sub_25;
 extern uint16 straight_road_flag;
 extern  uint16 special_cross_flag;
 extern uint16 speed_change_flag;
 extern uint16 special_left_flag;
 extern uint16 special_right_flag;

 extern float sub_16;
 extern float sub_78;

extern uint16  zhidao_sum;
 
//舵机初始化，频率250Hz,初始占空比给0
void servo_init(void)
{
  uint8 i = 0;
  
  Servo.kp = 0;
  Servo.kd = 0;
  for(i = 0;i <20;i++)
   Servo.error[i] = 0;
  Servo.output = 0;
  Servo.pulse_width = SERVO_MID_VAL;
  
  Servo.distance_err_max_val = 0;
  Servo.distance_err_d_max_val = 0;
  for(i = 0; i <24;i++)
    Servo.dis1cm_err_store[i] = 0;


  tpm_pwm_init(TPM2,TPM_CH0,250,0);//舵机pwm初始化,250Hz

}

//舵机控制
void servo_pid_caculate(void)
{
  uint8 i = 0;
  float sum = 0;
  float sum_special = 0;
                        
  if(speed_change_flag == 3) //舵机测试，找中值用
  {  
    Servo.output = Servo.pulse_width;
  }
  //高速启动或者停车时减弱PD矫正
  else if((Speed.stop_car_enable == 1))
  {
    Servo.output = (uint16)(Servo.pulse_width + (Servo.kp *Servo.error[0] + Servo.kd*5.0* (Servo.error[0]-Servo.error[9])));
  }
  
  /******** 直角优先 ***************/
  else if(ZhijiaoRoad.zhijiao_servo_flag== 1 )
  {
    zhijiao_cnt++;
    //BELL_ON;

    if(zhijiao_cnt > ZhijiaoRoad.zhijiao_servo_time) //连续若干个周期
    {
      ZhijiaoRoad.zhijiao_servo_flag = 0;
      zhijiao_cnt = 0;
      //BELL_OFF;

    }
     
    for(i = 0;i < 10;i++)
    {
      sum += Servo.error[i];   
    }                                             
    if(sub_25 >= 10.0)     //左转直角
    {
    
     Servo.output = SERVO_LEFT_VAL;
    }
    else if(sub_25 <= -10.0) //右转直角
    {

     Servo.output = SERVO_RIGHT_VAL ;
    }
    else if(sum >= 0)
    {
      Servo.output = SERVO_LEFT_VAL ;
    }
    else
    {
      Servo.output = SERVO_RIGHT_VAL ;
    } 

    
  }
  /******** 弯道接十字的校正 ***************/
  else if(special_cross_flag == 1)
  {

     if(sub_78 >= 10.0)
        Servo.output = SERVO_LEFT_VAL ;
     if(sub_78 <= -10.0)
       Servo.output = SERVO_RIGHT_VAL;

 
  }
    /******** 丢线处理 ***************/
  else if(LoseLine.turn_left_flag==1)
  {
   

    LoseLine.turn_left_flag = 0;
    Servo.output = SERVO_LEFT_VAL;       
  }
 else if(LoseLine.turn_right_flag==1)        
  {
   

    LoseLine.turn_right_flag = 0;
    Servo.output = SERVO_RIGHT_VAL;
  }
 

  else     //普通路径正常控制 //右转线速度比左转慢，乘以系数校正
  {
        if(Servo.mode_select == 0)  //正跑速度较低，
            Servo.output = (uint16)(Servo.pulse_width + (Servo.kp *Servo.error[0] + Servo.kd*5.0* (Servo.error[0]-Servo.error[9])));//10ms
        else 
            Servo.output = (uint16)(Servo.pulse_width + (Servo.kp_2 *Servo.error[0] + Servo.kd_2*5.0* (Servo.error[0]-Servo.error[9])));//10ms        
        
  }
  
  /********* 输出限幅 **************/
  if(Servo.output <SERVO_RIGHT_VAL)
  {
    Servo.output = SERVO_RIGHT_VAL;
  }
  if(Servo.output > SERVO_LEFT_VAL)
  {
    Servo.output = SERVO_LEFT_VAL; 
  }
  
  tpm_pwm_duty(TPM2,TPM_CH0,Servo.output); //输出PWM作用于舵机


 
}
#endif
