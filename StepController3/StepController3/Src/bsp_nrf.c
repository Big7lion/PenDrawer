#include "bsp_nrf.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���???
//////////////////////////////////////////////////////////////////////////////////



const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //���յ�ַ
uint8_t TR_Mode=0;

//��ʼ???24L01��IO???
void bsp_nrf_init(void)
{
    NRF24L01_CE(0); //ʹ��24L01
    NRF24L01_CSN(1); //SPIƬѡȡ???
}
//��???24L01�Ƿ����
//����???:0����???;1��ʧ???
uint8_t NRF24L01_Check(void)
{
    uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    uint8_t i;
    NRF24L01_Write_Buf(WRITE_24L01REG + TX_ADDR, buf, 5);   //д��5���ֽڵĵ�ַ.
    NRF24L01_Read_Buf(TX_ADDR, buf, 5);                     //����д��ĵ�ַ
    for (i = 0; i < 5; i++)if (buf[i] != 0XA5)break;
    if (i != 5)return 1;                                    //��???24L01����
    return 0;                                               //��⵽24L01
}
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���???
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    NRF24L01_CSN(0);                                    //ʹ��SPI����
    HAL_SPI_TransmitReceive(&hspi2,&reg,&status,1,100); //���ͼĴ���???
    HAL_SPI_Transmit(&hspi2,&value,1,100);              //д��Ĵ�����???
    NRF24L01_CSN(1);                                    //��ֹSPI����
    return (status);                                    //����״̬???
}
//��ȡSPI�Ĵ���???
//reg:Ҫ���ļĴ���
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
    uint8_t reg_val;
    NRF24L01_CSN(0);                        //ʹ��SPI����
    HAL_SPI_Transmit(&hspi2,&reg,1,100);    //���ͼĴ���???
    HAL_SPI_Receive(&hspi2,&reg_val,1,100); //��ȡ�Ĵ�����???
    NRF24L01_CSN(1);                        //��ֹSPI����
    return (reg_val);                       //����״̬???
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ�???(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����???,�˴ζ�����״̬�Ĵ���???
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    //uint8_t uint8_t_ctr;
    NRF24L01_CSN(0);           //ʹ��SPI����
    HAL_SPI_TransmitReceive(&hspi2,&reg,&status,1,100);//���ͼĴ���???(λ��),����ȡ״̬???
    HAL_SPI_Receive(&hspi2,pBuf,len,100);
    NRF24L01_CSN(1);     //�ر�SPI����
    return status;        //���ض�����״̬???
}
//��ָ��λ��дָ�����ȵ���???
//reg:�Ĵ�???(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����???,�˴ζ�����״̬�Ĵ���???
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    NRF24L01_CSN(0);          //ʹ��SPI����
    HAL_SPI_TransmitReceive(&hspi2,&reg,&status,1,100);//���ͼĴ���???(λ��),����ȡ״̬???
    HAL_SPI_Transmit(&hspi2,pBuf,len,100);//д������
    NRF24L01_CSN(1);       //�ر�SPI����
    return status;          //���ض�����״̬???
}
//����NRF24L01����һ����???
//txbuf:�����������׵�ַ
//����???:�������״???
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
    uint8_t sta;
    //TX_Mode();
    NRF24L01_CE(0);
    NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, 32); //д���ݵ�TX BUF  32����???
    NRF24L01_CE(1); //������
    while (NRF24L01_IRQ != 0); //�ȴ�������???
    sta = NRF24L01_Read_Reg(STATUS); //��ȡ״̬�Ĵ�����???
    NRF24L01_Write_Reg(WRITE_24L01REG + STATUS, sta); //���TX_DS��MAX_RT�жϱ�־
    if (sta & MAX_TX) //�ﵽ����ط���???
    {
        NRF24L01_Write_Reg(FLUSH_TX, 0xff); //���TX FIFO�Ĵ�???
        return MAX_TX;
    }
    if (sta & TX_OK) //��������л�������ģʽ
    {
        RX_Mode();
        return TX_OK;
    }
    return 0xff;//����ԭ����ʧ???
}
//����NRF24L01����һ����???
//txbuf:�����������׵�ַ
//����???:0��������ɣ������������???
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
    uint8_t sta;
  //  RX_Mode();
    sta = NRF24L01_Read_Reg(STATUS); //��ȡ״̬�Ĵ�����???
    NRF24L01_Write_Reg(WRITE_24L01REG + STATUS, sta); //���TX_DS��MAX_RT�жϱ�־
    if (sta & RX_OK) //���յ���???
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, 32); //��ȡ����
        NRF24L01_Write_Reg(FLUSH_RX, 0xff); //���RX FIFO�Ĵ�???
        return 0;
    }
    return 1;//û�յ��κ���???
}


//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE���???,������RXģʽ,�����Խ���������
void RX_Mode(void)
{
    if(TR_Mode != 2)
    {
        NRF24L01_CE(0);
        NRF24L01_Write_Buf(WRITE_24L01REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS, 5); //дRX�ڵ��ַ

        NRF24L01_Write_Reg(WRITE_24L01REG + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ???
        NRF24L01_Write_Reg(WRITE_24L01REG + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_CH, 40);   //����RFͨ��Ƶ��
        NRF24L01_Write_Reg(WRITE_24L01REG + RX_PW_P0, RX_PLOAD_WIDTH); //ѡ��ͨ��0����Ч���ݿ�???
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_SETUP, 0x0f); //����TX�������,0db����,2Mbps,���������濪???
        NRF24L01_Write_Reg(WRITE_24L01REG + CONFIG, 0x0f); //���û�������ģʽ�Ĳ�???;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
        NRF24L01_CE(1); //CEΪ��,�������ģʽ
        TR_Mode = 2;
    }
}
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX������???,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE���???,������RXģʽ,�����Խ���������
//CEΪ�ߴ���10us,��������???.
void TX_Mode(void)
{
    if(TR_Mode != 1)
    {
        NRF24L01_CE(0);
        NRF24L01_Write_Buf(WRITE_24L01REG + TX_ADDR, (uint8_t*)TX_ADDRESS, 5); //дTX�ڵ��ַ
        NRF24L01_Write_Buf(WRITE_24L01REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS, 5); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK

        NRF24L01_Write_Reg(WRITE_24L01REG + EN_AA, 0x01);       //ʹ��ͨ��0���Զ�Ӧ???
        NRF24L01_Write_Reg(WRITE_24L01REG + EN_RXADDR, 0x01);   //ʹ��ͨ��0�Ľ��յ�ַ
        NRF24L01_Write_Reg(WRITE_24L01REG + SETUP_RETR, 0x1a);  //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط���???:10???
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_CH, 40);         //����RFͨ��???10
        NRF24L01_Write_Reg(WRITE_24L01REG + RF_SETUP, 0x0f);    //����TX�������,0db����,2Mbps,���������濪???
        NRF24L01_Write_Reg(WRITE_24L01REG + CONFIG, 0x0e);      //���û�������ģʽ�Ĳ�???;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,����������???
        NRF24L01_CE(1);                                         //CEΪ��,10us��������???
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
    return 1;//��ȷ
}

