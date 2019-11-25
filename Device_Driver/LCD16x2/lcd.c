

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"





void LCD_Enable()
{
    GPIO_WriteHigh(EN_PORT,LCD_EN);
    //LCD_EN=1;  
    Delay_us(3);
    GPIO_WriteLow(EN_PORT,LCD_EN);
    //LCD_EN=0;
    Delay_us(50);
}





void LCD_Send4bit( uint8_t  Data )
{
    
    //LCD_D4=(Data>>0)&0x01;
    if(Data&0x08)
    {
        GPIO_WriteHigh(D7_PORT,LCD_D7);
    }else
    {
        GPIO_WriteLow(D7_PORT,LCD_D7);
    }
    //LCD_D5=(Data>>1)&0x01;
    
    if(Data&0x04)
    {
        GPIO_WriteHigh(D6_PORT,LCD_D6);
    }else
    {
        GPIO_WriteLow(D6_PORT,LCD_D6);
    }
    //LCD_D6=(Data>>2)&0x01;
    
    if(Data&0x02)
    {
        GPIO_WriteHigh(D5_PORT,LCD_D5);
    }else
    {
        GPIO_WriteLow(D5_PORT,LCD_D5);
    }
    // LCD_D7=(Data>>3)&0x01;
    
    if(Data&0x01)
    {
        GPIO_WriteHigh(D4_PORT,LCD_D4);
    }else
    {
        GPIO_WriteLow(D4_PORT,LCD_D4);
    }
}





void LCD_guilenh(uint8_t lenh)
{
    LCD_Send4bit(lenh>>4);
    LCD_Enable();
    LCD_Send4bit(lenh);
    LCD_Enable();
    Delay_ms(2);
}





void LCD_Init ()
{
    GPIO_Init(RS_PORT, LCD_RS,GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(RW_PORT, LCD_RW,GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(EN_PORT, LCD_EN,GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(D4_PORT, LCD_D4,GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(D5_PORT, LCD_D5,GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(D6_PORT, LCD_D6,GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(D7_PORT, LCD_D7,GPIO_MODE_OUT_PP_LOW_SLOW);
    
    LCD_Send4bit(0x00);
    Delay_ms(20);
    GPIO_WriteLow(RS_PORT,LCD_RS);
    //LCD_RS=0;
    GPIO_WriteLow(RW_PORT,LCD_RW);
    //LCD_RW=0;
    LCD_Send4bit(0x03);
    LCD_Enable();
    Delay_ms(5);
    LCD_Enable();
    Delay_us(100);
    LCD_Enable();
    LCD_Send4bit(0x02);
    LCD_Enable();
    LCD_guilenh( 0x28 );         // giao tiep 4 bit, hien thi 2 hang, ki tu 5x8
    LCD_guilenh( 0x0c);          // cho phep hien thi man hinh
    LCD_guilenh( 0x06 );         // tang ID, khong dich khung hinh
    LCD_clear();                     // xoa toan bo khung hinh
}






void LCD_Gotoxy(uint8_t  x, uint8_t  y)
{
    uint8_t  vitri;
    if(!y)
        vitri = (0x80+x);
    else
        vitri = (0xC0+x);
    Delay_ms(1);
    LCD_guilenh(vitri);
    Delay_ms(5);
}



void LCD_clear()
{
    LCD_guilenh(0x01);  
    Delay_ms(5);
}




void LCD_PutChar ( uint8_t  Data )
{
    GPIO_WriteHigh(RS_PORT,LCD_RS);
    //LCD_RS=1;
    LCD_guilenh( Data );
    GPIO_WriteLow(RS_PORT,LCD_RS);
    //LCD_RS=0;
}





void  LCD_Puts (uint8_t *s)
{
    while (*s)
    {
        LCD_PutChar(*s);
        s++;
    }
}



