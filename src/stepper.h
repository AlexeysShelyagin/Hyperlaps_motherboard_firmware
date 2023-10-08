#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include <limits.h>
#include "config.h"

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
    Stepper *motors[4];
    bool def[4];
    bool dirs[4];

    uint64_t timeouts[4] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};
    double speeds[4] = {0, 0, 0, 0};
    uint64_t last_step[4] = {0, 0, 0, 0};
    double steps_to_do[4] = {0, 0, 0, 0};

    int8_t locked_dirs[4] = {0, 0, 0, 0};

    uint8_t state = 0;

    bool check_id(uint8_t id);

public:
    Stepper_array(Register_74HC595 reg_= Register_74HC595(), Stepper *mot1_ = nullptr, Stepper *mot2_ = nullptr, Stepper *mot3_ = nullptr, Stepper *mot4_ = nullptr);

    void step(uint8_t id);
    //void step(uint8_t id, bool dir);

    void set_speed(uint8_t id, double speed);
    double get_speed(uint8_t id);

    void lock_dir(uint8_t id, int8_t dir);
    void unlock_dir(uint8_t id);

    void send();
};

#endif