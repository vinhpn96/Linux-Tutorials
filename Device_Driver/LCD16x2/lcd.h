#ifndef _LCD_H_
#define _LCD_H_
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "..\myLib\mylib.h"
#include "stm8s_clk.h"
#include "stm8s_conf.h"
#include "stdio.h"


#define LCD_RS  GPIO_PIN_1
#define RS_PORT GPIOE

#define LCD_RW  GPIO_PIN_2
#define RW_PORT GPIOE

#define LCD_EN  GPIO_PIN_3
#define EN_PORT GPIOE

#define LCD_D4  GPIO_PIN_1
#define D4_PORT GPIOG

#define LCD_D5  GPIO_PIN_0	
#define D5_PORT GPIOG

#define LCD_D6  GPIO_PIN_7	
#define D6_PORT GPIOC

#define LCD_D7  GPIO_PIN_6
#define D7_PORT GPIOC






void LCD_Enable();
void LCD_Send4bit( uint8_t  Data) ;
void LCD_guilenh(uint8_t lenh);
void LCD_Init ();
void LCD_Gotoxy(uint8_t  x, uint8_t  y);
void LCD_clear();
void LCD_PutChar ( uint8_t  Data );
void  LCD_Puts (uint8_t *s);
#endif