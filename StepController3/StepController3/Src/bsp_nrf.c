#include "bsp_nrf.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用???
//////////////////////////////////////////////////////////////////////////////////



const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //接收地址
uint8_t TR_Mode=0;

//初始???24L01的IO???
void bsp_nrf_init(void)
{
    NRF24L01_CE(0); //使能24L01
    NRF24L01_CSN(1); //SPI片选取???
}
//检???24L01是否存在
//返回???:0，成???;1，失???
uint8_t NRF24L01_Check(void)
{
    uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    uint8_t i;
    NRF24L01_Write_Buf(WRITE_24L01REG + TX_ADDR, buf, 5);   //写入5个字节的地址.
    NRF24L01_Read_Buf(TX_ADDR, buf, 5);                     //读出写入的地址
    for (i = 0; i < 5; i++)if (buf[i] != 0XA5)break;
    if (i != 5)return 1;                                    //检???24L01错误
    return 0;                                               //检测到24L01
}
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的???
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    NRF24L01_CSN(0);                                    //使能SPI传输
    HAL_SPI_TransmitReceive(&hspi2,&reg,&status,1,100); //发送寄存器???
    HAL_SPI_Transmit(&hspi2,&value,1,100);              //写入寄存器的???
    NRF24L01_CSN(1);                                    //禁止SPI传输
    return (status);                                    //返回状态???
}
//读取SPI寄存器???
//reg:要读的寄存器
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
    uint8_t reg_val;
    NRF24L01_CSN(0);                        //使能SPI传输
    HAL_SPI_Transmit(&hspi2,&reg,1,100);    //发送寄存器???
    HAL_SPI_Receive(&hspi2,&reg_val,1,100); //读取寄存器内???
    NRF24L01_CSN(1);                        //禁止SPI传输
    return (reg_val);                       //返回状态???
}
//在指定位置读出指定长度的数据
//reg:寄存???(位置)
//*pBuf:数据指针
//len:数据长度
//返回???,此次读到的状态寄存器???
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    //uint8_t uint8_t_ctr;
    NRF24L01_CSN(0);           //使能SPI传输
    HAL_SPI_TransmitReceive(&hspi2,&reg,&status,1,100);//发送寄存器???(位置),并读取状态???
    HAL_SPI_Receive(&hspi2,pBuf,len,100);
    NRF24L01_CSN(1);     //关闭SPI传输
    return status;        //返回读到的状态???
}
//在指定位置写指定长度的数???
//reg:寄存???(位置)
//*pBuf:数据指针
//len:数据长度
//返回???,此次读到的状态寄存器???
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    NRF24L01_CSN(0);          //使能SPI传输
    HAL_SPI_TransmitReceive(&hspi2,&reg,&status,1,100);//发送寄存器???(位置),并读取状态???
    HAL_SPI_Transmit(&hspi2,pBuf,len,100);//写入数据
    NRF24L01_CSN(1);       //关闭SPI传输
    return status;          //返回读到的状态???
}
//启动NRF24L01发送一次数???
//txbuf:待发送数据首地址
//返回???:发送完成状???
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
    uint8_t sta;
    //TX_Mode();
    NRF24L01_CE(0);
    NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, 32); //写数据到TX BUF  32个字???
    NRF24L01_CE(1); //启动发
    while (NRF24L01_IRQ != 0); //等待发送完???
    sta = NRF24L01_Read_Reg(STATUS); //读取状态寄存器的???
    NRF24L01_Write_Reg(WRITE_24L01REG + STATUS, sta); //清除TX_DS或MAX_RT中断标志
    if (sta & MAX_TX) //达到最大重发次???
    {
        NRF24L01_Write_Reg(FLUSH_TX, 0xff); //清除TX FIFO寄存???
        return MAX_TX;
    }
    if (sta & TX_OK) //发送完成切换到接收模式
    {
        RX_Mode();
        return TX_OK;
    }
    return 0xff;//其他原因发送失???
}
//启动NRF24L01发送一次数???
//txbuf:待发送数据首地址
//返回???:0，接收完成；其他，错误代???
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
    uint8_t sta;
  //  RX_Mode();
    sta = NRF24L01_Read_Reg(STATUS); //读取状态寄存器的???
    NRF24L01_Write_Reg(WRITE_24L01REG + STATUS, sta); //清除TX_DS或MAX_RT中断标志
    if (sta & RX_OK) //接收到数???
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, 32); //读取数据
        NRF24L01_Write_Reg(FLUSH_RX, 0xff); //清除RX FIFO寄存???
        return 0;
    }
    return 1;//没收到任何数???
}


//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高???,即进入RX模式,并可以接收数据了
void RX_Mode(void)
{
    if(TR_Mode != 2)
    {
        NRF24L01_CE(0);
        NRF24L01_Write_Buf(WRITE_24L01REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS, 5); //写RX节点地址

        NRF24L01_Write_Reg(WRITE_24L01REG + EN_AA, 0x01); //使能通道0的自动应???
        NRF24L01_Write_Reg(WRITE_24L01REG + EN_RXADDR, 0x01); //使能通道0的接收地址
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_CH, 40);   //设置RF通信频率
        NRF24L01_Write_Reg(WRITE_24L01REG + RX_PW_P0, RX_PLOAD_WIDTH); //选择通道0的有效数据宽???
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_SETUP, 0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开???
        NRF24L01_Write_Reg(WRITE_24L01REG + CONFIG, 0x0f); //配置基本工作模式的参???;PWR_UP,EN_CRC,16BIT_CRC,接收模式
        NRF24L01_CE(1); //CE为高,进入接收模式
        TR_Mode = 2;
    }
}
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数???,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高???,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发???.
void TX_Mode(void)
{
    if(TR_Mode != 1)
    {
        NRF24L01_CE(0);
        NRF24L01_Write_Buf(WRITE_24L01REG + TX_ADDR, (uint8_t*)TX_ADDRESS, 5); //写TX节点地址
        NRF24L01_Write_Buf(WRITE_24L01REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS, 5); //设置TX节点地址,主要为了使能ACK

        NRF24L01_Write_Reg(WRITE_24L01REG + EN_AA, 0x01);       //使能通道0的自动应???
        NRF24L01_Write_Reg(WRITE_24L01REG + EN_RXADDR, 0x01);   //使能通道0的接收地址
        NRF24L01_Write_Reg(WRITE_24L01REG + SETUP_RETR, 0x1a);  //设置自动重发间隔时间:500us + 86us;最大自动重发次???:10???
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_CH, 40);         //设置RF通道???10
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_SETUP, 0x0f);    //设置TX发射参数,0db增益,2Mbps,低噪声增益开???
        NRF24L01_Write_Reg(WRITE_24L01REG + CONFIG, 0x0e);      //配置基本工作模式的参???;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中???
        NRF24L01_CE(1);                                         //CE为高,10us后启动发???
        TR_Mode = 1;
    }
}

uint8_t NRF24L01_Compare(uint8_t rxbf[],const uint8_t cpbf[])
{
    for(uint8_t i=0;i<rxbf[0];i++)
    {
        if(rxbf[i+1] != cpbf[i])
            return 0;
    }
    return 1;//正确
}

