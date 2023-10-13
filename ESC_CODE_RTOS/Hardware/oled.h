#ifndef __OLED_H
#define __OLED_H 

#include "stm32f10x.h"                  // Device header

#include "stdlib.h"	

//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_15)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOD,GPIO_Pin_15)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_14)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOD,GPIO_Pin_14)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_13)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOD,GPIO_Pin_13)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_12)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_12)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_11)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_11)


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

