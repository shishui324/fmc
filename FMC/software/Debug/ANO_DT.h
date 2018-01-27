#ifndef _ANO_DT_H
#define	_ANO_DT_H

#include "common.h"

typedef struct 
{
		uint8 send_version;
		uint8 send_status;
		uint8 send_senser;
		uint8 send_pid1;
		uint8 send_pid2;
		uint8 send_pid3;
		uint8 send_pid4;
		uint8 send_pid5;
		uint8 send_pid6;
		uint8 send_rcdata;
		uint8 send_offset;
		uint8 send_motopwm;
		uint8 send_power;

}dt_flag_t;

extern dt_flag_t f;

void ANO_DT_Data_Exchange(void);
void ANO_DT_Data_Receive_Prepare(uint8 data);
void ANO_DT_Data_Receive_Anl(uint8 *data_buf,uint8 num);
void ANO_DT_Send_Version(uint8 hardware_type, uint16 hardware_ver,uint16 software_ver,uint16 protocol_ver,uint16 bootloader_ver);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, int32 alt, uint8 fly_model, uint8 armed);
void ANO_DT_Send_Senser(int16 a_x,int16 a_y,int16 a_z,int16 g_x,int16 g_y,int16 g_z,int16 m_x,int16 m_y,int16 m_z);
void ANO_DT_Send_RCData(uint16 thr,uint16 yaw,uint16 rol,uint16 pit,uint16 aux1,uint16 aux2,uint16 aux3,uint16 aux4,uint16 aux5,uint16 aux6);
void ANO_DT_Send_Power(uint16 votage, uint16 current);
void ANO_DT_Send_MotoPWM(uint16 m_1,uint16 m_2,uint16 m_3,uint16 m_4,uint16 m_5,uint16 m_6,uint16 m_7,uint16 m_8);
void ANO_DT_Send_PID(uint8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);


#endif

