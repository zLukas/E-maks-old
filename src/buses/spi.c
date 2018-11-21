#include "spi.h"

SPI_InitTypeDef spi;

/* spiInit
	-spi bus initialization
*/
void spiInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_CPOL = SPI_CPOL_Low;
	spi.SPI_CPHA = SPI_CPHA_1Edge;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	spi.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&spi);
 
	SPI_Cmd(SPI2, ENABLE);
}

/* spisendReceive
	-sending then receiving function
*/
uint8_t spiSendReceive(uint8_t byte)
{
 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
 SPI_I2S_SendData(SPI2, byte);
 
 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
 return SPI_I2S_ReceiveData(SPI1);
}


/*
	 set chip enable state 
*/
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state)
    {
        GPIO_SetBits(NRF_PORT, NRF_CE);
    }
    else
    {
        GPIO_ResetBits(NRF_PORT, NRF_CE);
    }
}
/*
	 set chip select state 
*/
void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state)
    {
        GPIO_SetBits(NRF_PORT, NRF_CS);
    }
    else
    {
        GPIO_ResetBits(NRF_PORT, NRF_CS);
    }
}
