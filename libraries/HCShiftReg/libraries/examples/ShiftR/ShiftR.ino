#include "HCShiftReg.h"

int DS = 12;     //input pin (SER/DS)
int OE = 11;     //output enable pin (OE)
int ST_CP = 10;  //latch pin (RCLK/ST_CP)
int SH_CP = 9;   //shift pin (SRCLK/SH_CP)
int MR = 8;      //reset pin (SRCLR/MR)

ShiftR reg = ShiftR(DS, OE, ST_CP, SH_CP, MR);

int data[7] = {1, 1, 1, 0, 1, 1, 1};

void setup() {}

void loop() {
  reg.enable_output();        //enable output
  reg.initialize();           //erase all data from the latch and enable it
  reg.write_data(data, 7);    //write some data in array
  delay(3000);
  reg.reset();                //erase all data
  delay(100);
  reg.write_pin(1);           //Set HIGH on pin you choose, Low on others
  delay(3000);
  reg.reset();
}
