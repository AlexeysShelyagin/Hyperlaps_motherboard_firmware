#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "stepper.h"
#include "game_wifi.h"
#include "modules.h"

Register_74HC595 steppers_reg(LATCH_PIN, SCK_PIN, DATA_PIN);

Stepper motor1(steppers_reg, STEP1_BYTE, DIR1_BYTE, MOTOR_ENABLE);
Stepper motor2(steppers_reg, STEP2_BYTE, DIR2_BYTE, MOTOR_ENABLE);
Stepper motor3(steppers_reg, STEP3_BYTE, DIR3_BYTE, MOTOR_ENABLE);
Stepper motor4(steppers_reg, STEP4_BYTE, DIR4_BYTE, MOTOR_ENABLE);

Stepper_array motors(steppers_reg, &motor1, &motor2, &motor3, &motor4);
Endstop_array endstops(&motors, ENDSTOP1, ENDSTOP2, ENDSTOP3, ENDSTOP4);

Solenoid solenoids[] = {
     Solenoid(SOL1),
     Solenoid(SOL2),
     Solenoid(SOL3),
     Solenoid(SOL4)
};

Goal_sensor goal_sensor(DETECT1, DETECT2, DETECT3, DETECT4, DEFAULT_LASER_THRESHOLD);

Game_wifi wifi;

void setup() {
    Serial.begin(115200);

    pinMode(LATCH_PIN, OUTPUT);
    pinMode(SCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    digitalWrite(LATCH_PIN, HIGH);

    pinMode(SOL1, OUTPUT);
    pinMode(SOL2, OUTPUT);
    pinMode(SOL3, OUTPUT);
    pinMode(SOL4, OUTPUT);

    pinMode(ENDSTOP1, INPUT);
    pinMode(ENDSTOP2, INPUT);
    pinMode(ENDSTOP3, INPUT);
    pinMode(ENDSTOP4, INPUT);

    pinMode(MOTOR_ENABLE, OUTPUT);
    //digitalWrite(MOTOR_ENABLE, 1);

    endstops.invert_input();

    wifi.init();

#ifdef STATIC_GAMEPAD_ADDR
    wifi.add_gamepad(gamepad1_mac);
    wifi.add_gamepad(gamepad2_mac);
    wifi.add_gamepad(gamepad3_mac);
    wifi.add_gamepad(gamepad4_mac);
#endif

    for(uint8_t i = 0; i < 4; i++)
        wifi.send_score(10, i);

    Serial.println("initialized");
}

double speeds[4] = {DEFAULT_SPEED, DEFAULT_SPEED, DEFAULT_SPEED, DEFAULT_SPEED};
int8_t stick_states[4] = {0, 0, 0, 0};
uint64_t last_accel_call[4] = {0, 0, 0, 0};
bool is_goal;
//int8_t goal_id = -1;

uint64_t iteration = 0, iteration_goal = 0;
uint8_t goal_check_id = 0;

void loop() {
    if(iteration % 50 == 0){
        Game_wifi::Updates updates = wifi.get_updates();
    
        if(updates.success){  
            if(updates.button)
                solenoids[updates.id].fire();

            if(INVERT_MOTORS)
                updates.stick = -updates.stick;

            if(stick_states[updates.id] != updates.stick){
                stick_states[updates.id] = updates.stick;
                speeds[updates.id] = START_SPEED;
            }
        }

        endstops.check();

        for(int i = 0; i < 4; i++)
            solenoids[i].update();

        iteration = 0;
    }

    if(iteration_goal % 100 == 0){
        is_goal = goal_sensor.check(goal_check_id);
        if(is_goal){
            bool paused = true;
            wifi.send_score(-1, goal_check_id);
            
            while(paused){
                Game_wifi::Updates updates = wifi.get_updates();
                if(updates.success && updates.button && updates.id == goal_check_id)
                    paused = false;
                
                delay(10);
            }
            
        }

        goal_check_id++;
        goal_check_id %= 4;

        iteration_goal = 0;
    }

    for(int i = 0; i < 4; i++){
        if(speeds[i] < DEFAULT_SPEED){
            if(micros() - last_accel_call[i] > 1500){
                motors.set_speed(i, stick_states[i] * speeds[i]);
                speeds[i] += 1;
                last_accel_call[i] = micros();
            }
        }
    }

    motors.step(0);
    motors.step(1);
    motors.step(2);
    motors.step(3);
    motors.send();

    iteration++;
    iteration_goal++;
}
