#include "MPU6050.h"
#include "I2C.h"
#include <math.h>

double MPU_read_Acc_total(void) {
		uint8_t data[6];
		I2C_Read(MPU6050_ADDR, ACCEL_XOUT_H, data, 6);
		int16_t accX = (data[0] << 8) | data[1];
		int16_t accY = (data[2] << 8) | data[3];
		int16_t accZ = (data[4] << 8) | data[5];
		double total_acc = 1.0*accX*accX + 1.0*accY*accY + 1.0*accZ*accZ;
		total_acc = sqrt(total_acc)* 1.0/ ACC_SENS;		
		return total_acc;
}
 
void MPU6050_init(void) {

	uint8_t check;
	uint8_t Data;
	I2C_Read(MPU6050_ADDR, WHO_AM_I, &check, 1);
	if (check == 0x68)  // cam bien gui lai du lieu 0x68 neu khoi tao khong co van de gi
	{
		Data = 0;
		I2C_Write_To_Reg(MPU6050_ADDR, PWR_MGMT_1, Data);

		// chon toc do lay mau  = 8000 / (Data + 1)
		Data = 7;
		I2C_Write_To_Reg(MPU6050_ADDR, SMPLRT_DIV, Data);
	}
}
