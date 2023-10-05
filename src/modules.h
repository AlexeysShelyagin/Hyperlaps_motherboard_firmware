#ifndef MODULES_H
#define MODULES_H

#include <Arduino.h>
#include "stepper.h"

class Solenoid{
    int pin;
    bool is_fire = false;
    uint64_t fire_time = 0;
public:
    Solenoid(int pin_);

    void fire();
    void update();
};

class Goal_sensor{
    int pins[4];
    double threshold;
    bool inverted = false;
public:
    Goal_sensor(int pin1_, int pin2_, int pin3_, int pin4_, uint16_t threshold_);

    int8_t check();
};

class Endstop_array{
    Stepper_array *motors;
    int pins[4];
    int8_t stopped_dir[4];
    bool inverted = false;
public:
    Endstop_array(Stepper_array *motors_, int end1_pin, int end2_pin, int end3_pin, int end4_pin);

    void invert_input();

    void check();
};

#endif