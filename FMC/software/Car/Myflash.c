#if 0
#include "myflash.h"
#include "struct.h"

/***************  Small_Cap  ********************************
 *  * 文件说明：flash读写数据
 *  * 作    者：
 *  * 日    期：2017/03/26
 *  * 版    本：V1.0
 *  * 修改者  ：
 *  * 修改日期：
***********************************************************/

//extern uint16 zhidao_multi; //25乘积阈值
//extern uint16  zhidao_sum;  //1634和阈值
//extern uint16 speed_change_flag;
//extern uint16 stop_time_value;//定时停车
//extern uint16 run_off_enable;

uint32 read_buffer1[100]={0};             //定义Flash读缓冲1
uint32 write_buffer1[100]={0};            //定义Flash写缓冲1


/**************** Small_Cap  ****************************
 *  * 函数名称 ：flash_write_data
 *  * 函数参数 : 无
 *  * 返回值   : 成功返回FLASH_OK即1，失败返回FLASH_FAILED即0
 *  * 函数功能 ：写数据到flash
 *  * 作    者 ：
 *  * 日    期 : 2017/03/26
********************************************************/

uint8 flash_write_data(void)
{
    uint8 i = 0;  
    uint8 result = 0 ;
    DisableInterrupts; //关总中断,flash读写不能被中断打断
    flash_init(); //初始化flash
 
    result = flash_erase_sector(FLASH_SECTOR);//擦除扇区 
    if(result == 0) //擦除失败
      return FLASH_FAILED;
    DELAY_MS(30);
    
    write_buffer1[0] = 0;
    
    //电感最大值
    for(i = 1; i <= 8; i++)
      write_buffer1[i] = (uint32)(Adc.ad_max_val[i]);
    
//    //舵机参数
//    write_buffer1[9] = (uint32)(Servo.pulse_width);
//    write_buffer1[10] = (uint32)(Servo.kp * 1000);
//    write_buffer1[11] = (uint32)(Servo.kd * 1000);
//    write_buffer1[12] = (uint32)(Servo.distance_err_max_val * 1000); 
//    write_buffer1[13] = (uint32)(Servo.distance_err_d_max_val * 1000);
//    write_buffer1[14] = (uint32)(Servo.mode_select * 1000); 
//    
//    //电机参数
//    write_buffer1[17] = (uint32)(Motor.kvff * 1000);
//    write_buffer1[18] = (uint32)(Motor.kaff * 1000);
//    write_buffer1[19] = (uint32)(Motor.kp * 1000);
//    write_buffer1[20] = (uint32)(Motor.ki * 1000);
//    write_buffer1[21] = (uint32)(Motor.kd * 1000);
//    write_buffer1[22] = (uint32)(Motor.set_value_temp);
    
    //阈值参数 
    //直角
//    write_buffer1[23] = (uint32)(ZhijiaoRoad.zhijiao_163425_val);
//    write_buffer1[24] = (uint32)(ZhijiaoRoad.zhijiao_25_sub);
//    write_buffer1[25] = (uint32)(ZhijiaoRoad.zhijiao_2516_sub);
//    write_buffer1[26] = (uint32)(ZhijiaoRoad.zhijiao_empty_val);
//    write_buffer1[27] = (uint32)(ZhijiaoRoad.zhijiao_servo_time);
    //丢线
//    write_buffer1[28] = (uint32)(LoseLine.lose_line_val);
    //十字
//    write_buffer1[29] = (uint32)(CrossRoad.cross_fazhi_in);
//    write_buffer1[30] = (uint32)(CrossRoad.cross_fazhi_out_in);
//    write_buffer1[31] = (uint32)(CrossRoad.cross_fazhi_out);
    //拟合系数
//    write_buffer1[32] = (uint32)(Sensor.PARAM_A*1000);
//    write_buffer1[33] = (uint32)(Sensor.PARAM_B*1000);
//    write_buffer1[34] = (uint32)(Sensor.PARAM_CROSS*1000);  
    
    //直道
//    write_buffer1[35] = (uint32)(zhidao_multi);
//    write_buffer1[36] = (uint32)(zhidao_sum);
    //变速
//    write_buffer1[37] = (uint32)speed_change_flag;
    
    //定时停车
//    write_buffer1[38] = (uint32)(stop_time_value*1000);
 

    //速度设定
//    write_buffer1[39] = (uint32)(Speed.zhidao_speed_val);
//    write_buffer1[40] = (uint32)(Speed.wandao_speed_val);
//    write_buffer1[41] = (uint32)(Speed.cross_speed_val);
//    write_buffer1[42] = (uint32)(Speed.zhijiao_speed_val);
//    write_buffer1[43] = (uint32)(Speed.ramp1_speed_val);
//    write_buffer1[44] = (uint32)(Speed.ramp2_speed_val);
    
    //坡道
//    write_buffer1[45] = (uint32)(RampRoad.ramp_judge_val);
//    write_buffer1[46] = (uint32)(RampRoad.ramp_shut_dis);    

    
//    write_buffer1[47] = (uint32)(ZhijiaoRoad.zhijiao_speed_time*1000);
//    write_buffer1[48] = (uint32)(RampRoad.ramp_enable*1000);
//    
//    write_buffer1[49] = (uint32)(RampRoad.ramp_start_dis*1000);
//    write_buffer1[50] = (uint32)(Speed.high_speed_dis*1000);
//    write_buffer1[51] = (uint32)(Speed.speed_high_mode*1000);   
//    
//    write_buffer1[52] = (uint32)(Motor.INH*1000); 
//    
//    write_buffer1[53] = (uint32)(Servo.kp_2*1000);
//    write_buffer1[54] = (uint32)(Servo.kd_2*1000);
// 
//    write_buffer1[55] = (uint32)(run_off_enable*1000);
//    
//    write_buffer1[56] = (uint32)(Speed.test_time*1000);
   
   
   
    flash_write_buf(FLASH_SECTOR,0,FLASH_DATA_SIZE,write_buffer1); //写入数据到指定扇区
    DELAY_MS(10);
 
    EnableInterrupts;

    return FLASH_OK;
}
/**************** Small_Cap  ****************************
 *  * 函数名称 ：flash_read_data
 *  * 函数参数 : 无
 *  * 返回值   : 无
 *  * 函数功能 ：读flash里的数据
 *  * 作    者 ：
 *  * 日    期 : 2017/03/26
********************************************************/
void flash_read_data(void)
{
  uint8 i = 0;
  uint32 *read_ptr; 
  DisableInterrupts;
  
  
   read_ptr = (uint32 *)(FLASH_SECTOR*1024); //读数据

  for(i = 0; i < FLASH_DATA_SIZE; i++)
    read_buffer1[i] = *(read_ptr++);
   
  for(i = 1;i <=8;i ++)   //电感最大值
  {
    Adc.ad_max_val[i]= (uint16)(read_buffer1[i]);
  }
  //舵机参数
//  Servo.pulse_width = read_buffer1[9];
//  Servo.kp = (float)(read_buffer1[10]/1000.0); 
//  Servo.kd = (float)(read_buffer1[11]/1000.0);
//  Servo.kp_2 = (float)(read_buffer1[53]/1000.0); 
//  Servo.kd_2 = (float)(read_buffer1[54]/1000.0);  
//  Servo.distance_err_max_val = (float)(read_buffer1[12]/1000.0);
//  Servo.distance_err_d_max_val = (float)(read_buffer1[13]/1000.0);
//  Servo.mode_select = (uint16)(read_buffer1[14]/1000);
  
  //电机参数
//  Motor.kvff = (float)(read_buffer1[17]/1000.0);
//  Motor.kaff = (float)(read_buffer1[18]/1000.0);
//  Motor.kp = (float)(read_buffer1[19]/1000.0);
//  Motor.ki = (float)(read_buffer1[20]/1000.0);
//  Motor.kd = (float)(read_buffer1[21]/1000.0);
//  Motor.set_value_temp = (int16)(read_buffer1[22]);
  
  //阈值参数
 
//    //直角
//  ZhijiaoRoad.zhijiao_163425_val = read_buffer1[23];
//  ZhijiaoRoad.zhijiao_25_sub = read_buffer1[24];
//  ZhijiaoRoad.zhijiao_2516_sub = read_buffer1[25];
//  ZhijiaoRoad.zhijiao_empty_val = read_buffer1[26];
//  ZhijiaoRoad.zhijiao_servo_time = read_buffer1[27];
//  ZhijiaoRoad.zhijiao_speed_time  = (uint16)(read_buffer1[47]/1000);
//    //丢线mn
//  LoseLine.lose_line_val = read_buffer1[28];
//    //十字
//  CrossRoad.cross_fazhi_in = read_buffer1[29];
//  CrossRoad.cross_fazhi_out_in  = read_buffer1[30];
//  CrossRoad.cross_fazhi_out  = read_buffer1[31];
    //拟合系数
//   Sensor.PARAM_A = (float)(read_buffer1[32]/1000.0);
//   Sensor.PARAM_B = (float)(read_buffer1[33]/1000.0);
//   Sensor.PARAM_CROSS = (float)(read_buffer1[34]/1000.0);
//    //直道
//   zhidao_multi = read_buffer1[35];
//   zhidao_sum = read_buffer1[36];
//   
//    //变速
//   speed_change_flag = read_buffer1[37];
//    
//    //定时停车
//   stop_time_value = (uint16)(read_buffer1[38]/1000);
// 
//
//    //速度设定
//   Speed.zhidao_speed_val = read_buffer1[39];
//   Speed.wandao_speed_val = read_buffer1[40];
//   Speed.cross_speed_val = read_buffer1[41];
//   Speed.zhijiao_speed_val = read_buffer1[42];
//   Speed.ramp1_speed_val = read_buffer1[43];
//   Speed.ramp2_speed_val = read_buffer1[44];
//   Speed.high_speed_dis = (uint16)(read_buffer1[50]/1000);
//   Speed.speed_high_mode = (uint16)(read_buffer1[51]/1000);
//   
//    
//    //坡道
//   RampRoad.ramp_judge_val = read_buffer1[45];
//   RampRoad.ramp_shut_dis = read_buffer1[46];   
//   RampRoad.ramp_enable = (uint16)(read_buffer1[48]/1000.0);
//   RampRoad.ramp_start_dis = (uint16)(read_buffer1[49]/1000);
//   
//   Motor.INH = (uint16)(read_buffer1[52]/1000);
//   
//   run_off_enable = (uint16)(read_buffer1[55]/1000);
//   Speed.test_time = (uint16)(read_buffer1[56]/1000);   
     
  EnableInterrupts;
}

#endif

