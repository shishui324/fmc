#include "BB.h"
#include "headfile.h"


uint8_t Car_BiBi_En = 1;	//BiBi使能

uint16_t Bell_C_ms = 0;		//常响时间
uint16_t Bell_D_ms = 0;		//bibi时间
uint16_t Pulse_Time = 0;


/*BB加时间*/
void Bell_Cry(uint16_t D_ms, uint16_t C_ms)
{
	if(D_ms > Bell_D_ms)
		Bell_D_ms = D_ms;
	
	if(C_ms > Bell_C_ms)
		Bell_C_ms = C_ms;
}

/*BB扫描*/
void Bell_Play(void)
{
	if(!Car_BiBi_En)
	{
		Bell_D_ms = 0;
		Bell_C_ms = 0;
	}
	
	if(Bell_D_ms)
	{
		Bell_D_ms--;
		if(Pulse_Time % 250 == 0)
		{//Bi一声
			if(Bell_C_ms < 125)
			Bell_C_ms = 125;
		}
	}
	
	if(Bell_C_ms)
	{
		Bell_C_ms--;
			BELL_ON;
	}
	else
	{
		BELL_OFF;
	}
	
	Pulse_Time++;
	if(Pulse_Time >= 1000)
		Pulse_Time = 0;
}

