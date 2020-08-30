#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h" 

#if 0  // KC: 用到才打開
extern int Encoder_A,Encoder_B,Encoder_C,Encoder_D; // 編碼器的脈衝計數
extern long int Motor_A,Motor_B,Motor_C,Motor_D;  // 馬達 PWM 變量
extern uint8_t Flag_Left,Flag_Right,Flag_sudu,Flag_Direction; // 藍芽遙控相關變量
extern uint8_t Flag_Stop,Flag_Show;  // 預設停止、顯示打開
extern long int Target_A,Target_B,Target_C,Target_D,Rate_A,Rate_B,Rate_C,Rate_D;  // 馬達目標速度
extern  int Voltage,Voltage_Zheng,Voltage_Xiao;  // 電池相關取樣
extern float Angle_Balance,Gyro_Balance,Gyro_Turn;  // 平衡傾角 陀螺儀平衡  轉向平衡
extern float Show_Data_Mb;  // 用於顯示需要查看的數據
extern int Temperature;
extern u32 Distance;  // 超音波測距
extern uint8_t Bi_zhang,delay_50,delay_flag;
extern float Acceleration_Z;
extern int RC_Velocity,RC_Position;
extern int Encoder_A_EXTI;
extern uint8_t Run_Flag,PID_Send,Flash_Send,Turn_Flag;
extern uint8_t rxbuf[8],Urxbuf[8],txbuf[8],txbuf2[8],CAN_ON_Flag,Usart_ON_Flag,PS2_ON_Flag,Usart_Flag;
extern float Pitch,Roll,Yaw,Move_X,Move_Y,Move_Z; 
extern long int Position_A,Position_B,Position_C,Position_D;
extern uint16_t PID_Parameter[10],Flash_Parameter[10];
extern float	Position_KP,Position_KI,Position_KD;  // 位置控制 PID
extern float Velocity_KP,Velocity_KI;  // 速度控制 PID
extern int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;
#endif

#define SYSTEM_SUPPORT_OS		0
																	    
// IO 操作巨集
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
// IO 位址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
// 單一 IO port 操作, n < 16
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)

#endif











