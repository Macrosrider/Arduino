/*
 *
 */


#ifndef GySensor_h
#define GySensor_h

#include <Arduino.h>
#include <Wire.h>

#define ACC_ID       0x00
#define ACC_ADDRESS (0xA7 >> 1)  //I2C Address
#define ACC_POWER    0x2D
#define ACC_DATA_X0  0x32
#define ACC_DATA_X1  0x33
#define ACC_DATA_Y0  0x34
#define ACC_DATA_Y1  0x35
#define ACC_DATA_Z0  0x36
#define ACC_DATA_Z1  0x37

#define GYR_ID       0x0F
#define GYR_ADDRESS (0xD2 >> 1)  //I2C Address
#define GYR_POWER    0x20        // to turn on with default settings - 0x0F
#define GYR_DATA_X0  0x28
#define GYR_DATA_X1  0x29
#define GYR_DATA_Y0  0x2A
#define GYR_DATA_Y1  0x2B
#define GYR_DATA_Z0  0x2C
#define GYR_DATA_Z1  0x2D

#define MAG_ADDRESS  0x1E       //I2C Address
#define MAG_MODE     0x02
#define MAG_CONF1    0x00
#define MAG_DATA_X0  0x03
#define MAG_DATA_X1  0x04
#define MAG_DATA_Z0  0x05
#define MAG_DATA_Z1  0x06
#define MAG_DATA_Y0  0x07
#define MAG_DATA_Y1  0x08

#define BMP180_ADDRESS 0x77     //I2C Address

#define FILTERED     0
#define NONFILTERED  1
#define MMHG         0
#define PA           1

class GySensor{

public:
    GySensor();

    int AcX, AcY, AcZ;

    double fAcX;
    double fAcY;
    double fAcZ;

    int GyX, GyY, GyZ;

    int MgX, MgY, MgZ;

    short temperature;
    long pressure;
    float altitude;

    void begin();

    void get_accel_data();

    void get_gyro_data();

    void get_mag_data();

    void accel_low_pass_filter();

    int get_roll(int status);

    int get_pitch(int status);

    int get_rotation();

    void get_temperature();

    void get_pressure(int data_type);

    void get_altitude();

private:

    const float alpha = 0.5;

    const unsigned char OSS = 0;

    int ac1, ac2, ac3, b1, b2, mb, mc, md;
    unsigned int ac4, ac5, ac6;
    long b5;
    const float p0 = 101325;
    //unsigned int ut;
    //unsigned long up;

    char bmp180Read(unsigned char address);

    int bmp180ReadInt(unsigned char address);

    void bmp180Calibration();

    unsigned int bmp180ReadUT();

    unsigned long bmp180ReadUP();

};

#endif
