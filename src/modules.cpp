#include "modules.h"
#include "stepper.h"

Solenoid::Solenoid(int pin_){
    pin = pin_;
}

void Solenoid::fire(){
    if(millis() - fire_time < SOLENOID_CALLDOWN)
        return;
        
    if(!is_fire)
        fire_time = millis();
    is_fire = true;
}

void Solenoid::update(){
    if(is_fire && millis() - fire_time <= SOLENOID_SAFE_FIRE_TIME){
        digitalWrite(pin, 1);
    }
    else{
        is_fire = false;
        digitalWrite(pin, 0);
    }
}

Goal_sensor::Goal_sensor(int pin1_, int pin2_, int pin3_, int pin4_, uint16_t threshold_){
    pins[0] = pin1_;
    pins[1] = pin2_;
    pins[2] = pin3_;
    pins[3] = pin4_;
    threshold = threshold_;
}

int8_t Goal_sensor::check(){
    for(int i = 0; i < 4; i++){
        if(analogRead(pins[i]) < threshold){
            return i;
        }
    }

    return -1;
}

Endstop_array::Endstop_array(Stepper_array *motors_, int end1_pin, int end2_pin, int end3_pin, int end4_pin){
    motors = motors_;
    pins[0] = end1_pin;
    pins[1] = end2_pin;
    pins[2] = end3_pin;
    pins[3] = end4_pin;
}

void Endstop_array::invert_input(){
    inverted = !inverted;
}

void Endstop_array::check(){
    for(int i = 0; i < 4; i++){
        double current_speed = motors -> get_speed(i);

        if(digitalRead(pins[i]) != inverted){
            if(current_speed == stopped_dir[i] || stopped_dir[i] == 0){
                stopped_dir[i] = current_speed / abs(current_speed);
                motors -> set_speed(i, 0);
                motors -> lock_dir(i, stopped_dir[i]);
            }
        }
        else{
            stopped_dir[i] = 0;
            motors -> unlock_dir(i);
        }
    }
}