#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

typedef struct {
    int16_t ax, ay, az, gx, gy, gz;
} IMU_Data;

uint8_t setup_imu(void);
int16_t read_imu(uint8_t regH, uint8_t regL);
//void calibrate_imu(float* ax_offset, float* ay_offset, float* az_offset, float* gx_offset, float* gy_offset, float* gz_offset);
void read_accel_gyro(IMU_Data* data);
void imu_timer_init();

void dfPlayerPlay(uint16_t track);
void dfPlayerVolume(uint16_t volume);


#endif
