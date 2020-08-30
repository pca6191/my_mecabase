#include "IOI2C.h"
#include "delay.h"

void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;  //GPIOB10.11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100 MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //GPIOB

  GPIOB->BSRRH = GPIO_Pin_10;
  GPIOB->BSRRH = GPIO_Pin_11;
}

int IIC_Start(void)
{
  SDA_OUT()
  ;
  IIC_SDA = 1;
  if (!READ_SDA)
    return 0;
  IIC_SCL = 1;
  delay_us(1);
  IIC_SDA = 0;  //START:when CLK is high,DATA change form high to low
  if (READ_SDA)
    return 0;
  delay_us(1);
  IIC_SCL = 0;
  return 1;
}

void IIC_Stop(void)
{
  SDA_OUT()
  ;
  IIC_SCL = 0;
  IIC_SDA = 0;  //STOP:when CLK is high DATA change form low to high
  delay_us(1);
  IIC_SCL = 1;
  IIC_SDA = 1;
  delay_us(1);
}

int IIC_Wait_Ack(void)
{
  uint8_t ucErrTime = 0;
  SDA_IN()
  ;
  IIC_SDA = 1;
  delay_us(1);
  IIC_SCL = 1;
  delay_us(1);
  while (READ_SDA) {
    ucErrTime++;
    if (ucErrTime > 50) {
      IIC_Stop();
      return 0;
    }
    delay_us(1);
  }
  IIC_SCL = 0;
  return 1;
}

void IIC_Ack(void)
{
  IIC_SCL = 0;
  SDA_OUT()
  ;
  IIC_SDA = 0;
  delay_us(1);
  IIC_SCL = 1;
  delay_us(1);
  IIC_SCL = 0;
}

void IIC_NAck(void)
{
  IIC_SCL = 0;
  SDA_OUT()
  ;
  IIC_SDA = 1;
  delay_us(1);
  IIC_SCL = 1;
  delay_us(1);
  IIC_SCL = 0;
}

void IIC_Send_Byte(uint8_t txd)
{
  uint8_t t;
  SDA_OUT()
  ;
  IIC_SCL = 0;
  for (t = 0; t < 8; t++) {
    IIC_SDA = (txd & 0x80) >> 7;
    txd <<= 1;
    delay_us(1);
    IIC_SCL = 1;
    delay_us(1);
    IIC_SCL = 0;
    delay_us(1);
  }
}

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
  int i;
  if (!IIC_Start())
    return 1;
  IIC_Send_Byte(addr << 1);
  if (!IIC_Wait_Ack()) {
    IIC_Stop();
    return 1;
  }
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  for (i = 0; i < len; i++) {
    IIC_Send_Byte(data[i]);
    if (!IIC_Wait_Ack()) {
      IIC_Stop();
      return 0;
    }
  }
  IIC_Stop();
  return 0;
}

int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
  if (!IIC_Start())
    return 1;
  IIC_Send_Byte(addr << 1);
  if (!IIC_Wait_Ack()) {
    IIC_Stop();
    return 1;
  }
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  IIC_Start();
  IIC_Send_Byte((addr << 1) + 1);
  IIC_Wait_Ack();
  while (len) {
    if (len == 1)
      *buf = IIC_Read_Byte(0);
    else
      *buf = IIC_Read_Byte(1);
    buf++;
    len--;
  }
  IIC_Stop();
  return 0;
}

uint8_t IIC_Read_Byte(unsigned char ack)
{
  unsigned char i, receive = 0;
  SDA_IN()
  ;
  for (i = 0; i < 8; i++) {
    IIC_SCL = 0;
    delay_us(2);
    IIC_SCL = 1;
    receive <<= 1;
    if (READ_SDA)
      receive++;
    delay_us(2);
  }
  if (ack)
    IIC_Ack();
  else
    IIC_NAck();
  return receive;
}

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
  unsigned char res = 0;

  IIC_Start();
  IIC_Send_Byte(I2C_Addr);
  res++;
  IIC_Wait_Ack();
  IIC_Send_Byte(addr);
  res++;
  IIC_Wait_Ack();
  //IIC_Stop();
  IIC_Start();
  IIC_Send_Byte(I2C_Addr + 1);
  res++;
  IIC_Wait_Ack();
  res = IIC_Read_Byte(0);
  IIC_Stop();

  return res;
}

uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
  uint8_t count = 0;

  IIC_Start();
  IIC_Send_Byte(dev);
  IIC_Wait_Ack();
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  IIC_Start();
  IIC_Send_Byte(dev + 1);
  IIC_Wait_Ack();

  for (count = 0; count < length; count++) {

    if (count != length - 1)
      data[count] = IIC_Read_Byte(1);
    else
      data[count] = IIC_Read_Byte(0);
  }
  IIC_Stop();
  return count;
}

uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
  uint8_t count = 0;
  IIC_Start();
  IIC_Send_Byte(dev);
  IIC_Wait_Ack();
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  for (count = 0; count < length; count++) {
    IIC_Send_Byte(data[count]);
    IIC_Wait_Ack();
  }
  IIC_Stop();

  return 1;  //status == 0;
}

uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
{
  *data = I2C_ReadOneByte(dev, reg);
  return 1;
}

unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
  return IICwriteBytes(dev, reg, 1, &data);
}

uint8_t IICwriteBits(uint8_t dev, uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data)
{
  uint8_t b;
  if (IICreadByte(dev, reg, &b) != 0) {
    uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    data <<= (8 - length);
    data >>= (7 - bitStart);
    b &= mask;
    b |= data;
    return IICwriteByte(dev, reg, b);
  }
  else {
    return 0;
  }
}

uint8_t IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data)
{
  uint8_t b;
  IICreadByte(dev, reg, &b);
  b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
  return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
