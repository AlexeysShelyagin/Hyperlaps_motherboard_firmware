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

    wifi.init();
    wifi.add_gamepad(gamepad1_mac);
    wifi.add_gamepad(gamepad2_mac);
    wifi.add_gamepad(gamepad3_mac);
    wifi.add_gamepad(gamepad4_mac);

    Serial.println("initialized");
}

uint8_t a = 0;
void loop() {
    wifi.send_score(a, 0);
    a = (a + 1) % 11;
    delay(1000);
}
