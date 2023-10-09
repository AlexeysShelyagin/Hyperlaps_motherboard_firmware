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

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < LASER_BUFFER_SIZE; j++)
            buffer[i][j] = 4096;
    }
}

bool Goal_sensor::check(uint8_t id){
    for(int j = 1; j < LASER_BUFFER_SIZE; j++)
        buffer[id][j] = buffer[id][j - 1];
    
    buffer[id][0] = analogRead(pins[id]);

    if(buffer[id][0] < threshold){
        for(int j = 0; j < LASER_BUFFER_SIZE; j++){
            if(buffer[id][j] >= threshold)
                return false;
        }
        
        return true;
    }

    return false;
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
    for(int i = 0; i < 4; i++)
        initial_unpressed[i] = !initial_unpressed[i];
}

void Endstop_array::check_initial(){
    for(int i = 0; i < 4; i++)
        initial_unpressed[i] = !digitalRead(pins[i]);
}

void Endstop_array::check(){
    for(int i = 0; i < 4; i++){
        double current_speed = motors -> get_speed(i);
        int8_t current_dir = current_speed / abs(current_speed);

        if(digitalRead(pins[i]) != inverted){
            if(initial_unpressed[i] && stopped_dir[i] == 0){
                stopped_dir[i] = current_dir;
                motors -> set_speed(i, 0);
                motors -> lock_dir(i, stopped_dir[i]);
            }
        }
        else{
            stopped_dir[i] = 0;
            motors -> unlock_dir(i);
            initial_unpressed[i] = true;
        }
    }
}