/*
 *
 */


#ifndef HCShiftReg_h
#define HCShiftReg_h

#include <Arduino.h>

class ShiftR{

public:
    ShiftR(int input_pin, int output_enable_pin, int latch_pin, int shift_pin, int reset_pin);

    void enable_output();

    void disable_output();

    void reset();

    void shift();

    void latch_enable();

    void latch_disable();

    void write_data(int data[8], int data_len);

    void write_pin(int pin);

    void initialize();

private:

    int DS, OE, ST_CP, SH_CP, MR;
};

#endif