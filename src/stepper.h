#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

class Register_74HC595{
public:
    int latch_pin, sck_pin, data_pin;

    Register_74HC595() = default;
    Register_74HC595(int latch_pin_, int sck_pin_, int data_pin_);
};

class Stepper{
    Register_74HC595 reg;

public:
    uint8_t step_byte, dir_byte;
    int enable_pin;

    Stepper(Register_74HC595 reg_, uint8_t step_byte_, uint8_t dir_byte_, int enable_pin_ = -1);

    void step(bool dir = 0);
    
    void enable();
    void disable();
};

class Stepper_array{
    Register_74HC595 reg;
    Stepper *mot1, *mot2, *mot3, *mot4;
    bool def1, def2, def3, def4;

    uint8_t state = 0;
    void step(Stepper *mot, bool dir);
public:
    Stepper_array(Register_74HC595 reg_= Register_74HC595(), Stepper *mot1_ = nullptr, Stepper *mot2_ = nullptr, Stepper *mot3_ = nullptr, Stepper *mot4_ = nullptr);

    void step1(bool dir = 0);
    void step2(bool dir = 0);
    void step3(bool dir = 0);
    void step4(bool dir = 0);

    void send();
};

#endif