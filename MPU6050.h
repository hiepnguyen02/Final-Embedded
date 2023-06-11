#include <stm32f10x.h>

#define MPU6050_ADDR 0xD0
#define ACC_SENS 16384.0
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SMPLRT_DIV 0x19
#define ACCEL_XOUT_H 0x3B

double MPU_read_Acc_total(void);
void MPU6050_init(void);
