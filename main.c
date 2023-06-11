#include <stm32f10x.h>
#include <stdio.h>
#include <math.h>
#include "LCD.h"
#include "Systick.h"
#include "TIMER.h"
#include "GPIO.h"
#include "I2C.h"
#include "MPU6050.h"
#include "FIRFilter.h"

uint32_t STEP_NUMBER = 0;
uint8_t MODE = 1;
// MODE = 1 : trang thai dem buoc chan, den xanh nhay
// MODE = 0 : trang thai dung, khong dem, den do bat
uint32_t time;
FIRFilter fir;

int main(void) {
		GPIO_Init();
		SysTick_Init();
		I2C_Init();
		MPU6050_init();
		LCD_Init();
		TIMER_Config();
		FIRFilter_init(&fir);
		LCD_SetCursor(0, 0);
		LCD_WriteString("So buoc:");
		LCD_SetCursor(1, 0);
		LCD_WriteString("0");	
		time = millis(); 
		while(1) {	
				if(MODE == 1) {			// nhay den moi 1s khi mode haot dong la mode 0
						if(millis() > time + 1000) {
								GPIOC->ODR ^= GPIO_ODR_ODR13;
								time = millis(); 
						}
				}
		}
}

