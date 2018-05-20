#include <Arduino.h>
#include "GySensor.h"
#include <Wire.h>


GySensor::GySensor() {

}

void GySensor::begin(){
  //setup accelerometer
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(ACC_POWER);              //turn on the accelerometer
  Wire.write(0x08);                      //can be 0x08
  Wire.endTransmission();
  delay(100);
  //setup gyroscope
  Wire.beginTransmission(GYR_ADDRESS);
  Wire.write(GYR_POWER);              //turn on the gyroscope
  Wire.write(0x0F);
  Wire.endTransmission();
  delay(100);
  //setup magnetometer
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.write(MAG_MODE);              //turn on the magnetometer
  Wire.write(MAG_CONF1);             //set measurement mode to 1
  Wire.endTransmission();
  delay(100);

  bmp180Calibration();

  fAcX = 0;
  fAcY = 0;
  fAcZ = 0;
}

void GySensor::get_accel_data(){
  //get raw accelerometer measurment data
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(ACC_DATA_X0);
  Wire.endTransmission(false);
  Wire.requestFrom(ACC_ADDRESS, 6, true);

  AcX=Wire.read()<<8|Wire.read() / 256;
  AcY=Wire.read()<<8|Wire.read() / 256;
  AcZ=Wire.read()<<8|Wire.read() / 256;

}

void GySensor::get_gyro_data(){
  //get raw gyroscope measurment data
  Wire.beginTransmission(GYR_ADDRESS);
  Wire.write(GYR_DATA_X0);
  Wire.endTransmission(false);
  Wire.requestFrom(GYR_ADDRESS, 6, true);

  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();

}

void GySensor::get_mag_data(){
  //get raw magnetometer measurment data
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.write(MAG_DATA_X0);
  Wire.endTransmission(false);
  Wire.requestFrom(MAG_ADDRESS, 6, true);

  MgX=Wire.read()<<8|Wire.read();
  MgZ=Wire.read()<<8|Wire.read();
  MgY=Wire.read()<<8|Wire.read();

}

void GySensor::accel_low_pass_filter(){
  //low-pass filter for accelerometer
  fAcX = (AcX * alpha) + (fAcX * alpha);
  fAcY = (AcY * alpha) + (fAcY * alpha);
  fAcZ = (AcZ * alpha) + (fAcZ * alpha);

}

int GySensor::get_roll(int status){   //use constants FILTERED/NONFILTERED
  //get roll, data must be after low-pass filter
  if(status == FILTERED){

     return (atan2(-fAcY, fAcZ)*180.0)/M_PI;

  } else if (status == NONFILTERED){

     return (atan2(-AcY, AcZ)*180.0)/M_PI;

  }

}

int GySensor::get_pitch(int status){   //use constants FILTERED/NONFILTERED
  //get-pitch, data must be after low-pass filter
  if(status == FILTERED){

     return (atan2(fAcX, sqrt(fAcY*fAcY + fAcZ*fAcZ))*180.0)/M_PI;

  } else if (status == NONFILTERED){

     return (atan2(AcX, sqrt(AcY*AcY + AcZ*AcZ))*180.0)/M_PI;

  }

}

int GySensor::get_rotation(){
  //get rotation angle between nord and your position
  float heading = atan2(MgY, MgX);
  if(heading == 0){

    heading += 2*PI;

  }

  int angle = heading * 180/M_PI;
  return angle;

}

char GySensor::bmp180Read(unsigned char address){

  unsigned char data;

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(BMP180_ADDRESS, 1);

  return Wire.read();

}

int GySensor::bmp180ReadInt(unsigned char address){

  unsigned char msb, lsb;

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(BMP180_ADDRESS, 2);

  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;

}

void GySensor::bmp180Calibration(){

  ac1 = bmp180ReadInt(0xAA);
  ac2 = bmp180ReadInt(0xAC);
  ac3 = bmp180ReadInt(0xAE);
  ac4 = bmp180ReadInt(0xB0);
  ac5 = bmp180ReadInt(0xB2);
  ac6 = bmp180ReadInt(0xB4);
  b1 = bmp180ReadInt(0xB6);
  b2 = bmp180ReadInt(0xB8);
  mb = bmp180ReadInt(0xBA);
  mc = bmp180ReadInt(0xBC);
  md = bmp180ReadInt(0xBE);

}

unsigned int GySensor::bmp180ReadUT(){

  unsigned long ut;

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  delay(5);

  ut = bmp180ReadInt(0xF6);
  
  return ut;

}

unsigned long GySensor::bmp180ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  delay(2 + (3<<OSS));

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP180_ADDRESS, 3);

  delay(50);

  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;

}

void GySensor::get_temperature(){

  long x1, x2;
  
  unsigned int ut = bmp180ReadUT();

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  temperature = ((b5 + 8)>>4) * 0.1;

}

void GySensor::get_pressure(int data_type){     //use constants MMHG/PA

  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  unsigned long up = bmp180ReadUP();

  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  if(data_type == PA){

     pressure = p;

  }else if(data_type == MMHG){

    pressure = p * 0.75006375541921;

  }

}

void GySensor::get_altitude(){

  altitude = (float)44330 * (1 - pow(((float) pressure/p0), 0.190295));

}
