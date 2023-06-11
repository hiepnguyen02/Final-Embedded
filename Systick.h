#include <stm32f10x.h>

void SysTick_Init(void);
void SysTick_Handler(void);
uint32_t millis(void);
void delay(uint32_t ms);
void delay_us(uint32_t us);
