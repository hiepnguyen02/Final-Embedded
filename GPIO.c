#include <stm32f10x.h>
#include "Systick.h"
#include "LCD.h"
#include "GPIO.h"

#define PAUSE_SWITCH (1u<<5)
#define RESET_SWITCH (1u<<6)

extern uint8_t MODE;
extern uint32_t STEP_NUMBER;

void GPIO_Init(void) {
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;	
		//khoi tao len xanh tai chan 13 port C	  
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);  // xoa cac bit cai dat ve 0
    GPIOC->CRH |= GPIO_CRH_MODE13_0;                    // chi set bit mode_0 = 1 --> output pp	
		//khoi tao led do
		GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_MODE0_0;
	
		//Khoi tao 2 nut bam la input pull up
		GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
		GPIOA->CRL |= GPIO_CRL_CNF5_1;  									// input push-pull
		GPIOA->ODR |= GPIO_ODR_ODR5;  										// cai dat A5 la pull up
		GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);   // tuong tu cai cho chan A6
		GPIOA->CRL |= GPIO_CRL_CNF6_1;  								
		GPIOA->ODR |= GPIO_ODR_ODR6; 
		//cau hinh ngat tai suong xuong cho cac chan
		AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI5_PA;					// Ngat tai port A chu khong phai B C D E
		EXTI->IMR |= PAUSE_SWITCH; 												// xoa mat na ngat
		EXTI->FTSR |= PAUSE_SWITCH;												// Ngat tai suong xuong
		AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI6_PA;					// Tuong tu cho nut reset tai chan a6
		EXTI->IMR |= RESET_SWITCH; 												
		EXTI->FTSR |= RESET_SWITCH;											
		// set muc do uu tien va cho phep ngat
		NVIC_SetPriority(EXTI9_5_IRQn, 5);								
		NVIC_EnableIRQ (EXTI9_5_IRQn);		
}

void EXTI9_5_IRQHandler(void) {	
	if (EXTI->PR & PAUSE_SWITCH) {
			EXTI->PR |= PAUSE_SWITCH;
			if (MODE == 0) {
					GPIOA->BRR |= (1u<<0);      		// tat den do tai chan A0
					MODE = 1;
					TIM3->DIER |= TIM_DIER_UIE;    	// timer3 cho phep ngat
			} else {
					GPIOA->BSRR |= (1u<<0);					// bat den do tai chan A0
					GPIOC->BSRR |= (1u<<13);        // tat den xang tai chan C13
					TIM3->DIER &= ~TIM_DIER_UIE;    // khong cho phep ngat timer3
					MODE = 0;
			}
			delay(400);
	} else if (EXTI->PR & RESET_SWITCH) {
			EXTI->PR |= RESET_SWITCH;
			STEP_NUMBER = 0;
			LCD_SetCursor(0, 0);
			LCD_ClearScrean();
			LCD_WriteString("So buoc:");
			LCD_SetCursor(1, 0);
			LCD_WriteString("0");
			delay(400);
	}
}
