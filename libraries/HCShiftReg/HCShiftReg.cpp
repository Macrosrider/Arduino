#include <Arduino.h>
#include "HCShiftReg.h"


ShiftR::ShiftR(int input_pin, int output_enable_pin, int latch_pin, int shift_pin, int reset_pin) {
    pinMode(input_pin, OUTPUT);
    DS = input_pin;
    pinMode(output_enable_pin, OUTPUT);
    OE = output_enable_pin;
    pinMode(latch_pin, OUTPUT);
    ST_CP = latch_pin;
    pinMode(shift_pin, OUTPUT);
    SH_CP = shift_pin;
    pinMode(reset_pin, OUTPUT);
    MR = reset_pin;
}

void ShiftR::enable_output(){
    digitalWrite(OE, LOW);
}

void ShiftR::disable_output(){
    digitalWrite(OE, HIGH);
}

void ShiftR::reset(){
    digitalWrite(MR, LOW);
    delay(100);
    digitalWrite(MR, HIGH);
}

void ShiftR::shift(){
    digitalWrite(SH_CP, HIGH);
    digitalWrite(SH_CP, LOW);
}

void ShiftR::latch_enable(){
    digitalWrite(ST_CP, HIGH);
}

void ShiftR::latch_disable(){
    digitalWrite(ST_CP, LOW);
}

void ShiftR::write_data(int data[8], int data_len){
    for(int i =0; i < data_len; ++i){
        latch_disable();
        digitalWrite(DS, data[i]);
        shift();
        latch_enable();
    }
}

void ShiftR::write_pin(int pin){
    latch_disable();
    int counter = 8;
    for(int i = 0; i < 8; ++i){
        if(counter == pin){
            digitalWrite(DS, HIGH);
            shift();
            --counter;
        }else{
            digitalWrite(DS, LOW);
            shift();
            --counter;
        }
    }
    latch_enable();
}

void ShiftR::initialize(){
    latch_disable();
    digitalWrite(MR, HIGH);
    delay(100);
    latch_enable();
}