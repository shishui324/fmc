#include "Fuzzy.h"

	/*输入量P语言值特征点*/ 
	float PFF[7]={-24,-12,-6,0,6,12,24};
	/*输入量D语言值特征点*/ 
	float DFF[5]={-6.5,-3,0,3,6.5};
	/*输出量U语言值特征点(根据赛道类型选择不同的输出值)*/
	float UFF[7]={0,15,20,35,50,80,120};

	char rule[7][5]={
		//0   1   2   3   4  ec    e
		{ 1,  1,  1,  0,  0}, //  0
		{ 4,  2,  1,  0,  0}, //  1
		{ 6,  4,  1,  1,  0}, //  2
		{ 3,  3,  0,  3,  3}, //  3
		{ 0,  1,  1,  4,  6}, //  4
		{ 0,  0,  1,  2,  4}, //  5
	 	{ 0,  0,  1,  1,  1}  //  6
	};
	
//Fuzzy比率
float Fuzzy(float P,float D)
{

	char Pn;
	float Pf;
	char Dn;
	float Df;
	char Un[4];
	float Uf[4];
	float U_out;

	//左端判定模式--判断P的隶属度
	if(P <= PFF[0])
	{
		Pn = 0;
		Pf = 1.0f;
	}
	else if(P <= PFF[1])
	{
		Pn = 0;
		Pf = (PFF[0] - P) / (PFF[1] - PFF[0]);
	}
	else if(P <= PFF[2])
	{
		Pn = 1;
		Pf = (PFF[1] - P) / (PFF[2] - PFF[1]);
	}
	else if(P <= PFF[3])
	{
		Pn = 2;
		Pf = (PFF[2] - P) / (PFF[3] - PFF[2]);
	}
	else if(P <= PFF[4])
	{
		Pn = 3;
		Pf = (PFF[3] - P) / (PFF[4] - PFF[3]);
	}
	else if(P <= PFF[5])
	{
		Pn = 4;
		Pf = (PFF[4] - P) / (PFF[5] - PFF[4]);
	}
	else if(P <= PFF[6])
	{
		Pn = 5;
		Pf = (PFF[5] - P) / (PFF[6] - PFF[5]);
	}
	else
	{
		Pn = 5;
		Pf = 0.0f;
	}

	//左端判定模式--判断D的隶属度
	if(D <= DFF[0])
	{
		Dn = 0;
		Df = 1.0f;
	}
	else if(D <= DFF[1])
	{
		Dn = 0;
		Df = (DFF[1] - D) / (DFF[1] - DFF[0]);
	}
	else if(D <= DFF[2])
	{
		Dn = 1;
		Df = (DFF[2] - D) / (DFF[2] - DFF[1]);
	}
	else if(D <= DFF[3])
	{
		Dn = 2;
		Df = (DFF[3] - D) / (DFF[3] - DFF[2]);
	}
	else if(D <= DFF[4])
	{
		Dn = 3;
		Df = (DFF[4] - D) / (DFF[4] - DFF[3]);
	}
	else
	{
		Dn = 3;
		Df = 0.0f;
	}

	//判断U的隶属度
	Un[0] = rule[Pn][Dn];
	Un[1] = rule[Pn][Dn+1];
	Un[2] = rule[Pn+1][Dn];
	Un[3] = rule[Pn+1][Dn+1];

	Uf[0] = Pf * Df;
	Uf[1] = Pf * (1 - Df);
	Uf[2] = (1- Pf) * Df;
	Uf[3] = (1- Pf) * (1 - Df);

	U_out	= UFF[Un[0]] * Uf[0]
			+ UFF[Un[1]] * Uf[1]
			+ UFF[Un[2]] * Uf[2]
			+ UFF[Un[3]] * Uf[3];

	return U_out;
}












