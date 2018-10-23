#include "i2c.h"

/*
	additional universal i2c functions just in case
*/




I2C_InitTypeDef i2c;


/* i2cInit
	-i2C bus Initialization 
*/
void i2cInit(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	I2C_StructInit(&i2c);
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_ClockSpeed = 100000;
	I2C_Init(I2C1, &i2c);
	I2C_Cmd(I2C1, ENABLE);
} 
/* i2c findAddress
	-pairing with  i2c slave 
*/
uint8_t findAddress(uint32_t deviceAddress)
{
	uint8_t addressSet = 0;
	uint32_t addressReceived;
	i2cReveive(DEVICE_MEMORY_ADDRESS,&addressReceived, sizeof(addressReceived));
	if ( addressReceived== deviceAddress)
		{ addressSet = 1;}
	else
	{addressSet = 0;}
	
	return addressSet;
}
/* i2c setRegiste
	-setting the slave device memeory register to write data
*/
void setRegister(uint32_t DeviceMemoryAddress)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
 
	I2C_SendData(I2C1, DeviceMemoryAddress);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
}
	
/* i2cSend
	-Sending data to other devices
*/
void i2cSend(uint32_t deviceMemoryAddress , const void* data, int size)
{
	  int i;
 const uint8_t* buffer = (uint8_t*)data;
 
	setRegister(deviceMemoryAddress);
 for (i = 0; i < size; i++) {
 I2C_SendData(I2C1, buffer[i]);
 while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
 }
 I2C_GenerateSTOP(I2C1, ENABLE);
	
}	

// do konfiguracji
int i2cReveive( uint32_t deviceMemoryAddress , const void* data, int size)
{
	 int i;
	uint8_t* buffer = (uint8_t*)data;
 
	setRegister(DEVICE_ADDRESS);
 
	I2C_GenerateSTART(I2C1, ENABLE);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
 
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Send7bitAddress(I2C1, DEVICE_ADDRESS, I2C_Direction_Receiver);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
 
	for (i = 0; i < size - 1; i++) 
	{
		 while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
     buffer[i] = I2C_ReceiveData(I2C1);
	}
 I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
    buffer[i] = I2C_ReceiveData(I2C1);
	return (int)buffer;
	
}
