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

    motors[0] = mot1_;
    motors[1] = mot2_;
    motors[2] = mot3_;
    motors[3] = mot4_;

    for(int i = 0; i < 4; i++)
        def[i] = (motors[i] != nullptr);
}

void Stepper_array::step(uint8_t id){
    if(id < 0 || id >= 4)
        return;
    
    if(timeouts[id] != INT_MAX)
        steps_to_do[id] += min((double) (micros() - last_step[id]) / timeouts[id], 1.0);
    if(steps_to_do[id] < 1)
        return;

    steps_to_do[id] = max(steps_to_do[id] - 1, 0.0);

    last_step[id] = micros();


    state &= ~(1 << motors[id] -> dir_byte);

    state |= (1 << motors[id] -> step_byte);
    state |= (dirs[id] << motors[id] -> dir_byte);

}

void Stepper_array::step(uint8_t id, bool dir){
    state &= ~(1 << motors[id] -> dir_byte);

    state |= (1 << motors[id] -> step_byte);
    state |= (dir << motors[id] -> dir_byte);
}

void Stepper_array::send(){
    digitalWrite(reg.latch_pin, LOW);
    shiftOut(reg.data_pin, reg.sck_pin, MSBFIRST, state);
    digitalWrite(reg.latch_pin, HIGH);

    delayMicroseconds(2);

    for(int i = 0; i < 4; i++)
        state &= ~(1 << motors[i] -> step_byte);

    digitalWrite(reg.latch_pin, LOW);
    shiftOut(reg.data_pin, reg.sck_pin, MSBFIRST, state);
    digitalWrite(reg.latch_pin, HIGH);
}

void Stepper_array::set_speed(uint8_t id, double speed){
    if(speed < 0)
        dirs[id] = 0;
    else
        dirs[id] = 1;

    speeds[id] = speed;
    speed = abs(speed);

    if(speed == 0)
        timeouts[id] = INT_MAX;
    else
        timeouts[id] = 1e6 / speed / STEPS_PER_MM - 2;
}

double Stepper_array::get_speed(uint8_t id){
    return speeds[id];
}