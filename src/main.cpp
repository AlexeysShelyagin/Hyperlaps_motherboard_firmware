#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "stepper.h"
#include "game_wifi.h"

Register_74HC595 steppers_reg(LATCH_PIN, SCK_PIN, DATA_PIN);

Stepper motor1(steppers_reg, STEP1_BYTE, DIR1_BYTE, MOTOR_ENABLE);
Stepper motor2(steppers_reg, STEP2_BYTE, DIR2_BYTE, MOTOR_ENABLE);
Stepper motor3(steppers_reg, STEP3_BYTE, DIR3_BYTE, MOTOR_ENABLE);
Stepper motor4(steppers_reg, STEP4_BYTE, DIR4_BYTE, MOTOR_ENABLE);

Stepper_array motors(steppers_reg, &motor1, &motor2, &motor3, &motor4);

Game_wifi wifi;

int solenoids[] = {SOL1, SOL2, SOL3, SOL4};

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

    wifi.init();

#ifdef STATIC_GAMEPAD_ADDR
    wifi.add_gamepad(gamepad1_mac);
    wifi.add_gamepad(gamepad2_mac);
    wifi.add_gamepad(gamepad3_mac);
    wifi.add_gamepad(gamepad4_mac);
#endif

    Serial.println("initialized");
}

double speeds[4] = {DEFAULT_SPEED, DEFAULT_SPEED, DEFAULT_SPEED, DEFAULT_SPEED};
int8_t stick_states[4] = {0, 0, 0, 0};
uint64_t last_accel_call[4] = {0, 0, 0, 0};

void loop() {
    Game_wifi::Updates updates = wifi.get_updates();

    if(updates.success){
        /*
        Serial.print(updates.id);
        Serial.print(": ");
        Serial.print(updates.stick);
        Serial.print(" ");
        Serial.println(updates.button);
        */

        digitalWrite(solenoids[updates.id], updates.button);

        stick_states[updates.id] = updates.stick;
        speeds[updates.id] = START_SPEED;
    }

    for(int i = 0; i < 4; i++){
        if(speeds[i] < DEFAULT_SPEED){
            if(micros() - last_accel_call[i] > 1000){
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
}
