
#include "headfile.h"

#define HIGH 1

#define MPU_SDA_IN()  gpio_init(A1,GPI,HIGH)
#define MPU_SDA_OUT() gpio_init(A1,GPO,HIGH)


//#define MPU_IIC_SCL        PTH4_OUT 		//SCL  h4
//#define MPU_IIC_SDA_OUT    PTH3_OUT		//SDA	 
//#define MPU_IIC_SDA_IN     PTH3_IN		//SDA
#define MPU_IIC_SCL(x)     		gpio_set(A0,x)//GPIOB->PDOR&=~((uint32_t)x<<(H4-32)) // GPIOB->PDOR.PDOR28		//SCL  H4
#define MPU_IIC_SDA_OUT(x)    gpio_set(A1,x)//GPIOB->PDOR&=~((uint32_t)x<<(H3-32))  //GPIOB->PDOR.PDOR27		//SDA	 H3
#define MPU_IIC_SDA_IN    		gpio_get(A1)//((GPIOB->PDIR>>(F3-32))&0x01)//((GPIOB->PDIR>>(H3-32))&0x01)   //GPIOB->PDIR.PDIR27		//SDA

void MPU_IIC_Delay(void)
{
//  pit_delay_us(pit1,2);
}


void MPU_IIC_Init(void)
{					     
  gpio_init(A1,GPI,1);
  gpio_init(A0,GPO,1);
  port_pull(A0);//PORT->PUE1 |= (uint32)(1<<(F3-32));//H3????
}

//??IIC????
void MPU_IIC_Start(void)
{
  MPU_SDA_OUT();     //sda???
  MPU_IIC_SDA_OUT(1);	  	  
  MPU_IIC_SCL(1);
  MPU_IIC_Delay();
  MPU_IIC_SDA_OUT(0);//START:when CLK is high,DATA change form high to low 
  MPU_IIC_Delay();
  MPU_IIC_SCL(0);//??I2C??,????????? 
}	  
//??IIC????
void MPU_IIC_Stop(void)
{
  MPU_SDA_OUT();//sda???
  MPU_IIC_SCL(0);
  MPU_IIC_SDA_OUT(0);//STOP:when CLK is high DATA change form low to high
  MPU_IIC_Delay();
  MPU_IIC_SCL(1); 
  MPU_IIC_SDA_OUT(1);//??I2C??????
  MPU_IIC_Delay();							   	
}
//????????
//???:1,??????
//        0,??????
uint8_t MPU_IIC_Wait_Ack(void)
{
  uint8_t ucErrTime=0;
  MPU_SDA_IN();      //SDA?????  
  MPU_IIC_Delay();	   
  MPU_IIC_SCL(1);
  MPU_IIC_Delay();	 
  while(MPU_IIC_SDA_IN)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      MPU_IIC_Stop();
      return 1;//??????
    }
  }
  MPU_IIC_SCL(0);//????0 	   
  return 0;  
} 
//??ACK??
void MPU_IIC_Ack(void)
{
  MPU_IIC_SCL(0);
  MPU_SDA_OUT();
  MPU_IIC_SDA_OUT(0);
  MPU_IIC_Delay();
  MPU_IIC_SCL(1);
  MPU_IIC_Delay();
  MPU_IIC_SCL(0);
}
//???ACK??		    
void MPU_IIC_NAck(void)
{
  MPU_IIC_SCL(0);
  MPU_SDA_OUT();
  MPU_IIC_SDA_OUT(1);
  MPU_IIC_Delay();
  MPU_IIC_SCL(1);
  MPU_IIC_Delay();
  MPU_IIC_SCL(0);
}					 				     
//IIC??????
//????????
//1,???
//0,???			  
void MPU_IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;   
  MPU_SDA_OUT(); 	    
  MPU_IIC_SCL(0);//??????????
  for(t=0;t<8;t++)
  {              
    MPU_IIC_SDA_OUT((txd&0x80)>>7);
    txd<<=1; 	  
    MPU_IIC_SCL(1);
    MPU_IIC_Delay(); 
    MPU_IIC_SCL(0);	
    MPU_IIC_Delay();
  }	 
} 	    
//?1???,ack=1?,??ACK,ack=0,??nACK   
uint8_t MPU_IIC_Read_Byte(unsigned char ack)
{
  unsigned char i,receive=0;
  MPU_SDA_IN();//SDA?????
  for(i=0;i<8;i++ )
  {
    MPU_IIC_SCL(0); 
    MPU_IIC_Delay();
    MPU_IIC_SCL(1);
    receive<<=1;
    if(MPU_IIC_SDA_IN)receive++;   
    MPU_IIC_Delay(); 
  }					 
  if (!ack)
    MPU_IIC_NAck();//??nACK
  else
    MPU_IIC_Ack(); //??ACK   
  return receive;
}

