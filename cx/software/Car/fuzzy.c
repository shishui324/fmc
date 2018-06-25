#include "common.h"
#include "fuzzy.h"

int8_t E_Rule[7]  = {-12,-8,-4,0,4,8,12};		//误差E的模糊论域
int8_t EC_Rule[5] = {-2,-1,0,1,2};		//误差EC的模糊论域
float E_Fuzzy[2]  = {0.0,0.0};
float EC_Fuzzy[2] = {0.0,0.0};
//float Servo_Kp[10] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,10.0};

//uint8_t Kp_Rule[7][7] =  //kp的模糊控制表
//{
////EC -6 -4 -2  0  2  4  6	
//	  9, 9, 8, 7, 6, 4, 3, // -12
//	  9, 8, 6, 4, 3, 0, 0, // -8
//	  7, 6, 4, 3, 2, 0, 0, // -4
//	  5, 3, 1, 0, 1, 3, 5, //  0
//	  0, 0, 2, 3, 4, 6, 7, //  4
//	  0, 0, 3, 4, 6, 8, 9, //  8
//	  3, 4, 6, 7, 8, 9, 9  //  12
//};

//float Kp_Rule[7][7] =  //kp的模糊控制表
//{
////EC -6 -4 -2  0  2  4  6	
//	  3.0, 3.0, 2.7, 2.2, 2.0, 1.2, 1.0, // -12
//	  3.0, 2.7, 2.0, 1.2, 1.0, 0.0, 0.0, // -8
//	  2.2, 2.0, 1.2, 1.0, 0.6, 0.0, 0.0, // -4
//	  0.9, 0.6, 0.2, 0.0, 0.2, 0.6, 0.9, //  0
//	  0.0, 0.0, 0.6, 1.0, 1.2, 2.0, 2.2, //  4
//	  0.0, 0.0, 1.0, 1.2, 2.0, 2.7, 3.0, //  8
//	  1.0, 1.2, 2.0, 2.2, 2.7, 3.0, 3.0  //  12
//};

//float Kp_Rule[7][7] =  //kp的模糊控制表
//{
////EC -6 -4 -2  0  2  4  6	
//	  3.0, 3.0, 2.7, 2.2, 2.0, 1.2, 1.0, // -12
//	  3.0, 2.7, 2.0, 1.2, 1.0, 0.0, 0.0, // -8
//	  1.2, 1.0, 0.8, 0.6, 0.4, 0.0, 0.0, // -4
//	  0.6, 0.6, 0.2, 0.0, 0.2, 0.6, 0.6, //  0
//	  0.0, 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, //  4
//	  0.0, 0.0, 1.0, 1.2, 2.0, 2.7, 3.0, //  8
//	  1.0, 1.2, 2.0, 2.2, 2.7, 3.0, 3.0  //  12
//};

float Kp_Rule[5][7] =  //kp的模糊控制表
{
//EC  -2  	-1  0  	  1    2  	
	  1.9, 1.8, 1.7, 1.6, 1.5, // -12
	  1.4, 1.3, 1.2, 1.0, 0.9, // -8
	  1.2, 1.0, 0.8, 0.7, 0.6, // -4
	  0.5, 0.5, 0.4, 0.4, 0.4, //  0
	  0.6, 0.7, 0.8, 1.0, 1.2, //  4
	  0.9, 1.0, 1.2, 1.3, 1.4, //  8
	  1.5, 1.6, 1.7, 1.8, 1.9, //  12
};

float fuzzy_kp(int8_t E,int8_t EC)
{
	uint8_t pe,pec;//,num
	uint8_t Un[4];
	float 	Uf[4];
	
	/********误差变化率EC的隶属度********/
	if(E<E_Rule[0])
	{
		E_Fuzzy[0] = 1.0;
		pe = 0;
	}
	else if(E>=E_Rule[0]&&E<E_Rule[1])
	{
		E_Fuzzy[0] = (float)(E_Rule[1] - E)/(E_Rule[1] - E_Rule[0]);
		pe = 0;
	}
	else if(E>=E_Rule[1]&&E<E_Rule[2])
	{
		E_Fuzzy[0] = (float)(E_Rule[2] - E)/(E_Rule[2] - E_Rule[1]);
		pe = 1;
	}
	else if(E>=E_Rule[2]&&E<E_Rule[3])
	{
		E_Fuzzy[0] = (float)(E_Rule[3] - E)/(E_Rule[3] - E_Rule[2]);
		pe = 2;
	}
	else if(E>=E_Rule[3]&&E<E_Rule[4])
	{
		E_Fuzzy[0] = (float)(E_Rule[4] - E)/(E_Rule[4] - E_Rule[3]);
		pe = 3;
	}
	else if(E>=E_Rule[4]&&E<E_Rule[5])
	{
		E_Fuzzy[0] = (float)(E_Rule[5] - E)/(E_Rule[5] - E_Rule[4]);
		pe = 4;
	}
	else if(E>=E_Rule[5]&&E<E_Rule[6])
	{
		E_Fuzzy[0] = (float)(E_Rule[6] - E)/(E_Rule[6] - E_Rule[5]);
		pe = 5;
	}
	else
	{
		E_Fuzzy[0] = 0.0;
		pe = 5;
	}
	
	E_Fuzzy[1] = 1 - E_Fuzzy[0];
	
	/********误差变化率EC的隶属度********/
	
	if(EC<EC_Rule[0])
	{
		EC_Fuzzy[0] = 1.0;
		pec = 0;
	}
	else if(EC>=EC_Rule[0]&&EC<EC_Rule[1])
	{
		EC_Fuzzy[0] = (float)(EC_Rule[1] - EC)/(EC_Rule[1] - EC_Rule[0]);
		pec = 0;
	}
	else if(EC>=EC_Rule[1]&&EC<EC_Rule[2])
	{
		EC_Fuzzy[0] = (float)(EC_Rule[2] - EC)/(EC_Rule[2] - EC_Rule[1]);
		pec = 1;
	}
	else if(EC>=EC_Rule[2]&&EC<EC_Rule[3])
	{
		EC_Fuzzy[0] = (float)(EC_Rule[3] - EC)/(EC_Rule[3] - EC_Rule[2]);
		pec = 2;
	}
	else if(EC>=EC_Rule[3]&&EC<EC_Rule[4])
	{
		EC_Fuzzy[0] = (float)(EC_Rule[4] - EC)/(EC_Rule[4] - EC_Rule[3]);
		pec = 3;
	}
////	else if(EC>=EC_Rule[4]&&EC<EC_Rule[5])
////	{
////		EC_Fuzzy[0] = (float)(EC_Rule[5] - EC)/(EC_Rule[5] - EC_Rule[4]);
////		pec = 4;
////	}
////	else if(EC>=EC_Rule[5]&&EC<EC_Rule[6])
////	{
////		EC_Fuzzy[0] = (float)(EC_Rule[6] - EC)/(EC_Rule[6] - EC_Rule[5]);
////		pec = 5;
////	}
	else
	{
		EC_Fuzzy[0] = 0.0;
		pec = 5;
	}
	
	EC_Fuzzy[1] = 1 - EC_Fuzzy[0];
	
	/********查询模糊规则表********/
	Un[0] = Kp_Rule[pe][pec];
	Un[1] = Kp_Rule[pe][pec+1];
	Un[2] = Kp_Rule[pe+1][pec];
	Un[3] = Kp_Rule[pe+1][pec+1];
	
	Uf[0] = (float)E_Fuzzy[0]*EC_Fuzzy[0];
	Uf[1] = (float)E_Fuzzy[0]*EC_Fuzzy[1];
	Uf[2] = (float)E_Fuzzy[1]*EC_Fuzzy[0];
	Uf[3] = (float)E_Fuzzy[1]*EC_Fuzzy[1];
	 
	return (float)((float)(Un[0]*Uf[0])
					+(float)(Un[1]*Uf[1])
					+(float)(Un[2]*Uf[2])
					+(float)(Un[3]*Uf[3]));
					
}

















