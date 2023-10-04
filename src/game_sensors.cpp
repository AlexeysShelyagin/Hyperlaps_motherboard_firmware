#include "game_sensors.h"

Goal_sensor::Goal_sensor(int pin1_, int pin2_, int pin3_, int pin4_){
    pin[0] = pin1_;
    pin[1] = pin2_;
    pin[2] = pin3_;
    pin[3] = pin4_;
}

void Goal_sensor::check(){
    for(int i = 0; i < 4; i++){
        
    }
}