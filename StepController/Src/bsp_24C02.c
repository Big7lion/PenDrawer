#include "include.h"


//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA1;	  	  
	IIC_SCL1;
	bsp_DelayUS(4);
 	IIC_SDA0;//START:when CLK is high,DATA change form high to low 
	bsp_DelayUS(4);
	IIC_SCL0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL0;
	IIC_SDA0;//STOP:when CLK is high DATA change form low to high
 	bsp_DelayUS(4);
	IIC_SCL1; 
	IIC_SDA1;//����I2C���߽����ź�
	bsp_DelayUS(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA1;bsp_DelayUS(1);	   
	IIC_SCL1;bsp_DelayUS(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL0;
	SDA_OUT();
	IIC_SDA0;
	bsp_DelayUS(2);
	IIC_SCL1;
	bsp_DelayUS(2);
	IIC_SCL0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL0;
	SDA_OUT();
	IIC_SDA1;
	bsp_DelayUS(2);
	IIC_SCL1;
	bsp_DelayUS(2);
	IIC_SCL0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA1;
		else
			IIC_SDA0;
		txd<<=1; 	  
		bsp_DelayUS(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL1;
		bsp_DelayUS(2); 
		IIC_SCL0;	
		bsp_DelayUS(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL0; 
        bsp_DelayUS(2);
		IIC_SCL1;
        receive<<=1;
        if(READ_SDA)receive++;   
		bsp_DelayUS(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}
