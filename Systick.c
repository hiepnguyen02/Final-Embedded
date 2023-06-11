#include <stm32f10x.h>
#include "Systick.h"
uint32_t msTick = 0;

void SysTick_Init(void) {
		SysTick->LOAD |= SystemCoreClock / 1000 - 1;
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE | SysTick_CTRL_TICKINT;
		SysTick->CTRL |= SysTick_CTRL_ENABLE;
		NVIC_SetPriority(SysTick_IRQn, 4);
		NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void) {
		SysTick->CTRL |= SysTick_CTRL_COUNTFLAG;
		msTick++;
}

uint32_t millis(void) {
		return msTick;
}

void delay(uint32_t ms) {
		ms += millis();
		while(millis() < ms);
}
