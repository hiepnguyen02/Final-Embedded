#include <stm32f10x.h>
#include "I2C.h"


void I2C_Init(void) {
		// enable clock and gpio clock
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
		// configure B6 and B7 to alternative open drain function, maximum output rate is 50MHz
		GPIOB->CRL |= (GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
		GPIOB->CRL |= (GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
	
		// reset I2C
		I2C1->CR1 |= I2C_CR1_SWRST;
		I2C1->CR1 &= ~I2C_CR1_SWRST;
		
		// set I2C frequency = 100k
		I2C1->CR2 |= I2C_CR2_FREQ_3; 	// I2C T_PCLK1 = 1/8e6
		I2C1->CCR |= (40u<<0); 				// CRR[11:0] = 40 --> T_high = T_low = CCR * T_PCLK1 = 1/2e6
		I2C1->TRISE |= (9u<<0);				// Maximum rise time
		
		// enable I2C
		I2C1->CR1 |= I2C_CR1_PE;	
}

void I2C_Start(void) {
		I2C1->CR1 |= I2C_CR1_ACK;				// enable ack
		I2C1->CR1 |= I2C_CR1_START;			// send start conditon
		while(!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C_Stop(void) {
		I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_Address_Send(uint8_t addr) {
		I2C1->DR = addr;  											// send the address
		while (!(I2C1->SR1 & I2C_SR1_ADDR));  	// wait for ADDR bit to set
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  	// read SR1 and SR2 to clear the ADDR bit(EV6)
}	

void I2C_Read(uint8_t addr, uint8_t reg, uint8_t *buff, uint8_t size) {
		I2C_Start();
		I2C_Address_Send(addr);
		while (!(I2C1->SR1 & I2C_SR1_TXE));  // wait for TXE bit to set
		I2C1->DR = reg;                      // truyen du lieu la dia chi thanh ghi reg ra duong SDA cua I2C
		while (!(I2C1->SR1 & I2C_SR1_BTF));  // wait for BTF bit to set
		I2C_Start();  // repeated start
		
		// bat dau doc du lieu gui ve
		I2C_Address_Send(addr + 1);
		int i = 0;
		if(size == 1) {
				I2C1->CR1 &= ~I2C_CR1_ACK;  								// clear the ACK bit 
				I2C1->CR1 |= I2C_CR1_STOP;  								// Stop I2C		
				while (!(I2C1->SR1 & I2C_SR1_RXNE));
				buff[0] = I2C1->DR;
		} else {
				for(i = 0; i < size - 1; i++) {
						while (!(I2C1->SR1 & I2C_SR1_RXNE));
						buff[i] = I2C1->DR;
						I2C1->CR1 |= I2C_CR1_ACK;
				}
				I2C1->CR1 &= ~I2C_CR1_ACK;  							
				I2C1->CR1 |= I2C_CR1_STOP;  
				while (!(I2C1->SR1 & I2C_SR1_RXNE));
				buff[i] = I2C1->DR;
		}
		// ket thuc doc du lieu
		I2C_Stop();
}

void I2C_Write_To_Reg(uint8_t addr, uint8_t reg, uint8_t data) {
		I2C_Start();
		I2C_Address_Send(addr);
		while (!(I2C1->SR1 & I2C_SR1_TXE));  // wait for TXE bit to set
		I2C1->DR = reg;                      // truyen du lieu la dia chi thanh ghi reg ra duong SDA cua I2C
		while (!(I2C1->SR1 & I2C_SR1_BTF));  // wait for BTF bit to set
		while (!(I2C1->SR1 & I2C_SR1_TXE));  // wait for TXE bit to set
		I2C1->DR = data;                      // truyen du lieu la dia chi thanh ghi reg ra duong SDA cua I2C
		while (!(I2C1->SR1 & I2C_SR1_BTF));  // wait for BTF bit to set
		I2C_Stop();
}

void I2C_Write(uint8_t addr, uint8_t data) {
		I2C_Start();
		I2C_Address_Send(addr);
		while (!(I2C1->SR1 & I2C_SR1_TXE));  // wait for TXE bit to set
		I2C1->DR = data;                      // truyen du lieu la dia chi thanh ghi reg ra duong SDA cua I2C
		while (!(I2C1->SR1 & I2C_SR1_BTF));  // wait for BTF bit to set
		I2C_Stop();
}
