#include "stepper.h"

Register_74HC595::Register_74HC595(int latch_pin_, int sck_pin_, int data_pin_){
    latch_pin = latch_pin_;
    sck_pin = sck_pin_;
    data_pin = data_pin_;
}

Stepper::Stepper(Register_74HC595 reg_, uint8_t step_byte_, uint8_t dir_byte_, int enable_pin_){
    reg = reg_;
    step_byte = step_byte_;
    dir_byte = dir_byte_;
    enable_pin = enable_pin_;
}

void Stepper::step(bool dir){
    digitalWrite(reg.latch_pin, LOW);
    shiftOut(reg.data_pin, reg.sck_pin, MSBFIRST, ((1 << step_byte) | (dir << dir_byte)));
    digitalWrite(reg.latch_pin, HIGH);

    Serial.println(((1 << step_byte) | (dir << dir_byte)), BIN);

    delayMicroseconds(1);

    digitalWrite(reg.latch_pin, LOW);
    shiftOut(reg.data_pin, reg.sck_pin, MSBFIRST, ((0 << step_byte) | (dir << dir_byte)));
    digitalWrite(reg.latch_pin, HIGH);
}

void Stepper::enable(){
    digitalWrite(enable_pin, 0);
}

void Stepper::disable(){
    digitalWrite(enable_pin, 1);
}

Stepper_array::Stepper_array(Register_74HC595 reg_, Stepper *mot1_, Stepper *mot2_, Stepper *mot3_, Stepper *mot4_){
    reg = reg_;

    mot1 = mot1_;
    mot2 = mot2_;
    mot3 = mot3_;
    mot4 = mot4_;

    def1 = (mot1 != nullptr);
    def2 = (mot1 != nullptr);
    def3 = (mot1 != nullptr);
    def4 = (mot1 != nullptr);
}

void Stepper_array::step(Stepper *mot, bool dir){
    state |= (1 << mot -> step_byte);
    state |= (dir << mot -> dir_byte);
}

void Stepper_array::step1(bool dir){
    step(mot1, dir);
}

void Stepper_array::step2(bool dir){
    step(mot2, dir);
}

void Stepper_array::step3(bool dir){
    step(mot3, dir);
}

void Stepper_array::step4(bool dir){
    step(mot4, dir);
}

void Stepper_array::send(){
    digitalWrite(reg.latch_pin, LOW);
    shiftOut(reg.data_pin, reg.sck_pin, MSBFIRST, state);
    digitalWrite(reg.latch_pin, HIGH);

    delayMicroseconds(1);

    state &= ~(1 << mot1 -> step_byte);
    state &= ~(1 << mot2 -> step_byte);
    state &= ~(1 << mot3 -> step_byte);
    state &= ~(1 << mot4 -> step_byte);

    digitalWrite(reg.latch_pin, LOW);
    shiftOut(reg.data_pin, reg.sck_pin, MSBFIRST, state);
    digitalWrite(reg.latch_pin, HIGH);

    state = 0;
}