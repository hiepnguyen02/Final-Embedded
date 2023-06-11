#include <stm32f10x.h>
#include <stdio.h>
#include "Systick.h"
#include "TIMER.h"
#include "MPU6050.h"
#include "LCD.h"
#include "FIRFilter.h"

extern uint16_t STEP_NUMBER;
extern FIRFilter fir;

void TIMER_Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 			// cap clk cho khoi timer
    TIM3->PSC = 3599;													// chia tan so 3600 lan
    TIM3->ARR = 499;                    			// tan so goi ham timer la 25 ms 72.000.000/((3599+1)x(499+1))
    TIM3->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE; 	// enable auto reload
		TIM3->EGR |= TIM_EGR_UG;  								// cap nhat trang thai cho cac thanh ghi
		TIM3->DIER |= TIM_DIER_UIE;         			// cho phep ngat 
		NVIC_SetPriority(TIM3_IRQn, 5);
		NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
				TIM3->SR &= ~TIM_SR_UIF;  				// Xoa co ngat
				// dectect buoc chan NEU CO thi in ra LCD
				double total_acc = MPU_read_Acc_total();
				if (checkStep(&fir, total_acc) != 0) {
						char buf[7];
						LCD_SetCursor(1,0);
						STEP_NUMBER++;
						sprintf(buf, "%d", STEP_NUMBER);
						LCD_WriteString(buf);  
				}
    }
}
