/*
* cac bit 0-7 cua module dieu khien ung voi RS; R/W, EN, BL, P4 ~ P7.
* can khoi tao LCD o che do 4 bit
*/
#include <stm32f10x.h>

#define LCD_ADDRESS        	0x4E
#define RS_MASK							0x01
#define RW_MASK							0x02
#define EN_MASK 						0x04
#define BF_MASK							0x08

#define CTRL_COMMAND				0x00
#define CTRL_DATA 					RS_MASK
#define CTRL_READ						RW_MASK
#define CTRL_WRITE					0x00
#define BL_ON								0x08
#define CTRL_BL_OFF					0x00
#define CTRL_BL_DEFAULT			BL_MASK
#define CTRL_EN							0x04

#define	LCD_CM_CLEAR								0x01

#define LCD_CM_INPUTSET							0x04  //set the moving direction of cursor
#define LCD_CM_INPUTSET_INCR				0x02
#define LCD_CM_INPUTSET_DECR				0x00
#define LCD_CM_INPUTSET_ENSHIFT			0x01
#define LCD_CM_INPUTSET_DISSHIFT		0x00
#define LCD_CM_INPUTSET_DEFAULT 		(LCD_CM_INPUTSET | LCD_CM_INPUTSET_INCR | LCD_CM_INPUTSET_DISSHIFT)

#define LCD_CM_DISPLAYSWITCH						0x08
#define LCD_CM_DISPLAYSWITCH_DISPON			0x04  // DISPLAY ON
#define LCD_CM_DISPLAYSWITCH_DEFAULT		(LCD_CM_DISPLAYSWITCH | LCD_CM_DISPLAYSWITCH_DISPON)

#define LCD_CM_SHIFT							0x10
#define LCD_CM_SHIFT_DISPLAY			0x08
#define LCD_CM_SHIFT_CURSOR				0x00
#define LCD_CM_SHIFT_RIGHT				0x40
#define LCD_CM_SHIFT_LEFT					0x00
#define LCD_CM_SHIFT_DEFAULT			(LCD_CM_SHIFT | LCD_CM_SHIFT_CURSOR | LCD_CM_SHIFT_RIGHT)

#define LCD_CM_FUNCTIONSET					0x20
#define LCD_CM_FUNCTIONSET_8DL			0x10  //Data length is 8
#define LCD_CM_FUNCTIONSET_4DL			0x00	//Data length is 4
#define LCD_CM_FUNCTIONSET_2R				0x08	//Number of display line is 2
#define LCD_CM_FUNCTIONSET_DEFAULT	(LCD_CM_FUNCTIONSET | LCD_CM_FUNCTIONSET_4DL | LCD_CM_FUNCTIONSET_2R)


void LCD_WriteChar(uint8_t data, uint8_t mode);
void LCD_WriteString(char* string);
void LCD_SetCursor(uint8_t rows, uint8_t cols);
void LCD_Init(void);
void LCD_ClearScrean(void);
