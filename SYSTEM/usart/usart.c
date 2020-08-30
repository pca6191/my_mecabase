#include <stm32f4xx.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"

static char buf[PRINTF_BUFFER_SIZE];

//-----------------------------------------------------------------------------
// UART2 driver
//-----------------------------------------------------------------------------
int USART2_IRQHandler(void)
{
#if 1
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
    uint8_t ch = USART2->DR;

    while ((USART2->SR & 0X40) == 0) {
    }

    USART2->DR = (uint8_t) ch;
  }
#else
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    static u8 Flag_PID,i,j,Receive[50];
    static float Data;
    Usart2_Receive=USART2->DR;
    if(Usart2_Receive==0x4B) Turn_Flag=1;  //����ת����ƽ���
    else if(Usart2_Receive==0x49||Usart2_Receive==0x4A) {
      Turn_Flag=0;  //������ƽ���  // KC_DBG add '{}' gard
    }

    if(Run_Flag==0)  //�ٶȿ���ģʽ
    {
      if(Turn_Flag==0)  //�ٶȿ���ģʽ
      {
        if(Usart2_Receive>=0x41&&Usart2_Receive<=0x48)
        {
          Flag_Direction=Usart2_Receive-0x40;
        }
        else if(Usart2_Receive<=8)
        {
          Flag_Direction=Usart2_Receive;
        }
        else Flag_Direction=0;
      }
      else if(Turn_Flag==1)  //���������ת����ƽ���
      {
        if(Usart2_Receive==0x43) Flag_Left=0,Flag_Right=1;
        else if(Usart2_Receive==0x47) Flag_Left=1,Flag_Right=0;
        else Flag_Left=0,Flag_Right=0;
        if(Usart2_Receive==0x41||Usart2_Receive==0x45)Flag_Direction=Usart2_Receive-0x40;
        else Flag_Direction=0;
      }
    }
    //��������APP���Խ���ͨѶ
    if(Usart2_Receive==0x7B) Flag_PID=1;//APP����ָ����ʼλ
    if(Usart2_Receive==0x7D) Flag_PID=2;//APP����ָ��ֹͣλ

    if(Flag_PID==1)//�ɼ�����
    {
      Receive[i]=Usart2_Receive;
      i++;
    }
    if(Flag_PID==2)  //��������
    {
      if(Receive[3]==0x50) PID_Send=1;
      else if(Receive[3]==0x57) Flash_Send=1;
      else if(Receive[1]!=0x23)
      {
        for(j=i;j>=4;j--)
        {
          Data+=(Receive[j-1]-48)*pow(10,i-j);
        }
        switch(Receive[1])
        {
          case 0x30: RC_Velocity=Data;break;
          case 0x31: RC_Position=Data;break;
          case 0x32: Position_KP=Data;break;
          case 0x33: Position_KI=Data;break;
          case 0x34: Position_KD=Data;break;
          case 0x35: Velocity_KP=Data;break;
          case 0x36: Velocity_KI=Data;break;
          case 0x37: break;  //Ԥ��
          case 0x38: break;//Ԥ��
        }
      }
      Flag_PID=0;  //��ر�־λ����
      i=0;
      j=0;
      Data=0;
      memset(Receive, 0, sizeof(u8)*50);//��������
    }
  }
#endif

  return 0;
}


void usart2_init(uint32_t baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  //GPIO.A2.A3
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  //GPIO.A2.A3

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void usart2_send_byte(uint8_t ch)
{
  while ((USART2->SR & 0X40) == 0) {
  }

  USART2->DR = (uint8_t) ch;
}

void usart2_send_string(char *str)
{
  for (char *p = str; (*p) != '\0'; p++) {
    usart2_send_byte(*p);
  }
}

void usart2_printf(const char *format, ...)
{
  int ret = 0;
  //initial a va_list memory point
  va_list pcInputString;

  //Get the format address point
  va_start(pcInputString, format);

  //format string "Need heap memory"
  ret = vsnprintf(buf, PRINTF_BUFFER_SIZE, format, pcInputString);

  //對策 pcInputString 太長，ret 會大於 PRINTF_BUFFER_SIZE
  if (ret >= PRINTF_BUFFER_SIZE) {
    ret = PRINTF_BUFFER_SIZE;
  }  //else do the following

  //write message to UART
  usart2_send_string(buf);

  //release memory of va_list
  va_end(pcInputString);
}

