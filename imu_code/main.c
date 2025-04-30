#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "uart.h"

// IMU packages
#include "i2c.h"
#include "icm20948.h"
#include "imu.h"
#define IMU_BUFFER_SIZE 100

// IMU variables
IMU_Data imu_offset;
IMU_Data prev_imu;
IMU_Data imu_buffer;
float ax, ay, az, gx, gy, gz;

float zp_dot = 0.0;
float zp = 15;
uint8_t volume = 15;

float zr_dot = 0.0;
float zr = 30;
int8_t eq = 0;

void imu_init() {
    uart_init();
    
    _delay_ms(3000);
    printf("Setting up and calibrating IMU. DONT MOVE IT!!!\n");
    uint8_t whoami = setup_imu();
    printf("WHOAMI: %d\n", whoami);
    calibrate_imu(&imu_offset); // do this if the values are ass
    printf("IMU setup done\n");
    
    sei();
    song_uart_init();
}

ISR(TIMER1_COMPA_vect) {
    read_accel_gyro(&imu_buffer);
    ax = (imu_buffer.ax - imu_offset.ax) / 16384.0 * 9.81;
    ay = (imu_buffer.ay - imu_offset.ay) / 16384.0 * 9.81;
    az = (imu_buffer.az - imu_offset.az) / 16384.0 * 9.81;
    gx = (imu_buffer.gx - imu_offset.gx) / 131.0;
    gy = (imu_buffer.gy - imu_offset.gy) / 131.0;
    gz = (imu_buffer.gz - imu_offset.gz) / 131.0;
//    printf("Accel (m/s^2): X=%.2f Y=%.2f Z=%.2f | Gyro (deg / s): X=%.2f Y=%.2f Z=%.2f\n", ax, ay, az, gx, gy, gz);
    
    if (-2 < ax && ax < 2 && -2 < ay && ay < 2) {
        // Handle scratch effect (around z axis) (angle range: 180 degrees)
        zr_dot += (0.5 * (prev_imu.gz + gz) * 0.01);
        zr += zr_dot * 0.01;
        if (zr > 360) {
            zr -= 360;
        }
        if (zr < 0) {
            zr += 360;
        }
        printf("%d", (int8_t) (zr / 60));
        if (((int8_t) (zr / 60)) != eq) {
            eq = (int8_t) (zr / 60);
            set_track_eq(eq);
        }
        printf("zr: %.2f, zr_dot: %.2f, Prev gz: %.2f, EQ: %d\n", zr, zr_dot, prev_imu.gz, eq);
        
        // Handle volume (z axis acceleration) (1 meter total range)
        zp_dot += (0.5 * (prev_imu.az + az) * 0.01);
        zp += (zp_dot * 0.01) * 30.0 * 30.0;
        if (zp < 0) {
            zp = 0;
        }
        if (zp > 30) {
            zp = 30;
        }
        
        if ((uint8_t) zp != volume) {
            volume = (uint8_t) zp;
            change_track_volume(volume);
        }
//        printf("az: %.2f, zp_dot: %.2f, Prev az: %.2f, Volume: %.2f\n", az, zp_dot, prev_imu.az, volume);
//        if (az < -0.5 || 0.5 < az) {
//            
//        }
//        else {
//            volume_dot = 0.0;
//        }
    }
    else {
        zp_dot = 0.0;
    }
    
    prev_imu.ax = ax;
    prev_imu.ay = ay;
    prev_imu.az = az;
    prev_imu.gx = gx;
    prev_imu.gy = gy;
    prev_imu.gz = gz;
}

int main(void) {
    
    imu_init();
    
    //play_track(5); // Play back in black normal speed
    //change_track_volume(volume);
    //set_track_eq(eq);
    
    while (1) {
        play_track(5);
    }
}
