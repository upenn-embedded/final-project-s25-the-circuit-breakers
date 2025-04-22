#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

// IMU packages
#include "i2c.h"
#include "icm20948.h"
#include "imu.h"
#define IMU_BUFFER_SIZE 100

// IMU variables
IMU_Data imu_buffer;
float ax, ay, az, gx, gy, gz;
float volume = 0.5;
float tempo = 0;
float pitch = 0;

ISR(TIMER1_COMPA_vect) {
    read_accel_gyro(&imu_buffer);
    ax = imu_buffer.ax / 16384.0 * 9.81;
    ay = imu_buffer.ay / 16384.0 * 9.81;
    az = imu_buffer.az / 16384.0 * 9.81;
    gx = imu_buffer.gx / 131.0;
    gy = imu_buffer.gy / 131.0;
    gz = imu_buffer.gz / 131.0;
//    printf("Accel (m/s^2): X=%.2f Y=%.2f Z=%.2f | Gyro (deg / s): X=%.2f Y=%.2f Z=%.2f\n", ax, ay, az, gx, gy, gz);
    
    if (-2 < ax && ax < 2 && -2 < ay && ay < 2) {
        // Handle scratch effect (around z axis) (angle range: 180 degrees)
        if (gz < -50 || 50 < gz) {
            // Do scratch sound effect with audio module
            // Pausing playback
            // Seeking forward or backward by a few milliseconds
            // Resuming playback
            // Repeat quickly to create a stuttering, jagged sound effect ? mimicking the rhythm of scratching.
//            printf("scratch effect\n");
        }
        
        // Handle volume (z axis acceleration) (1 meter total range)
        if (az < 9.4 || 10 < az) {
            volume += ((az - 9.70) * 0.01) * 30.0;
            if (volume < 0) {
                volume = 0;
            }
            if (volume > 30) {
                volume = 30;
            }
            printf("az: %.2f, Volume: %.2f\n", az, volume);
        }
    }
    
    // 
}

int main(void) {
//    float ax_offset;
//    float ay_offset;
//    float az_offset;
//    float gx_offset;
//    float gy_offset;
//    float gz_offset;
    
    printf("Setting up and calibrating IMU. DONT MOVE IT!!!\n");
    uint8_t whoami = setup_imu();
    printf("WHOAMI: %d\n", whoami);
//    calibrate_imu(&ax_offset, &ay_offset, &az_offset, &gx_offset, &gy_offset, &gz_offset); // do this if the values are ass
    printf("IMU setup done\n");
    
    sei();
    uart_init();
    
    while (1) {
        
    }
}
