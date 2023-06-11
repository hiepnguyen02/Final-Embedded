#include "LCD.h"
#include "I2C.h"
#include "Systick.h"
#include <stm32f10x.h>

void LCD_SetCursor(uint8_t rows, uint8_t cols) {
		uint8_t ddram_addr = 0x80 + rows * 0x40 + cols;
		LCD_WriteChar(ddram_addr, CTRL_COMMAND);
		delay(1);
}

void LCD_WriteString(char* string) {
		while(*string != '\0') LCD_WriteChar(*string++, CTRL_DATA);
		delay(1);
}

void LCD_Init(void) {		
		//khoi tao LCD o che do 4 bit
		delay(50);
		LCD_WriteChar(0x30, CTRL_COMMAND);
		delay(5);
		LCD_WriteChar(0x30, CTRL_COMMAND);
		delay(5);
		LCD_WriteChar(0x32, CTRL_COMMAND);
		LCD_WriteChar(0x20, CTRL_COMMAND);

		LCD_WriteChar(LCD_CM_FUNCTIONSET_DEFAULT, CTRL_COMMAND);
		LCD_WriteChar(LCD_CM_INPUTSET_DEFAULT, CTRL_COMMAND); 			// che do 4 bit, 2 dong hien thi
		LCD_WriteChar(LCD_CM_DISPLAYSWITCH_DEFAULT, CTRL_COMMAND); // bat man hinh
		LCD_WriteChar(LCD_CM_CLEAR, CTRL_COMMAND); 								// xoa man hinh	
		delay(2); //can cho thoi gian xoa man hinh > 1.6s
}

void LCD_ClearScrean(void) {
		LCD_WriteChar(LCD_CM_CLEAR, CTRL_COMMAND);
		delay(2); //can cho thoi gian xoa man hinh >1.6s
}

void LCD_WriteChar(uint8_t data, uint8_t mode) {
		uint8_t Data_H;
		uint8_t Data_L;
		Data_H = data & 0xF0;
		Data_L = (data<<4) & 0xF0;
		Data_H |= BL_ON | mode; 
		Data_L |= BL_ON | mode; 

		Data_H |= CTRL_EN;
		I2C_Write(LCD_ADDRESS, Data_H);
		Data_H &= ~CTRL_EN;
		I2C_Write(LCD_ADDRESS, Data_H);
		Data_L |= CTRL_EN;
		I2C_Write(LCD_ADDRESS, Data_L);
		Data_L &= ~CTRL_EN;
		I2C_Write(LCD_ADDRESS, Data_L);
}
