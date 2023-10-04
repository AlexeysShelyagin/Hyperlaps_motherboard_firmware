#ifndef GAME_SENSORS_H
#define GAME_SENSORS_H

#include "config.h"

class Goal_sensor{
    int pin[4];
    double threshold = DEFAULT_LASER_THRESHOLD;
public:
    Goal_sensor(int pin1_, int pin2_, int pin3_, int pin4_);

    void check();
};

#endif