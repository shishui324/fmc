#ifndef __DEBUG_H__
#define __DEBUG_H__

extern uint8_t Park_or_Lock;	//Park_1	Lock_0
extern uint8_t Write_Buffer_mode;
extern uint8_t Lock_remove_signal;		//—” ±∑¢≥µ
extern uint8_t S_S;

typedef union {
	int8_t *add_i8;
	uint8_t *add_u8;
	int16_t *add_i16;
	uint16_t *add_u16;
	int32_t *add_i32;
	int32_t *add_u32;
	float *add_float;
}Add_Typedef;

typedef struct{
	Add_Typedef Add;
//	float *Add;
	int8_t Step;
	char Name[13];
}UI_Units_TypeDef;

void UI_Init(void);
void UI_Display();
void UI_Adjust(uint8_t Record);

#endif	//DEBUG.h