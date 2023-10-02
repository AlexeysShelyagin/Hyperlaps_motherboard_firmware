#ifndef CONFIG_H
#define CONFIG_H

static uint8_t gamepad1_mac[] = {0x98, 0xF4, 0xAB, 0xF1, 0xF6, 0x52};
static uint8_t gamepad2_mac[] = {0x68, 0xC6, 0x3A, 0xFC, 0xCD, 0xAA};
static uint8_t gamepad3_mac[] = {0xEC, 0xFA, 0xBC, 0xCB, 0x86, 0xA3};
static uint8_t gamepad4_mac[] = {0x8C, 0xAA, 0xB5, 0x06, 0xEB, 0x9A};



#define LATCH_PIN 27
#define SCK_PIN 12
#define DATA_PIN 14

#define MOTOR_ENABLE 16

#define STEP3_BYTE 7
#define DIR3_BYTE 6
#define DIR4_BYTE 5
#define STEP4_BYTE 4
#define DIR1_BYTE 3
#define STEP1_BYTE 2
#define DIR2_BYTE 1
#define STEP2_BYTE 0

#define DETECT1 2
#define DETECT2 35
#define DETECT3 32
#define DETECT4 26

#define SOL1 13
#define SOL2 19
#define SOL3 23
#define SOL4 33

#define ENDSTOP1 15
#define ENDSTOP2 4
#define ENDSTOP3 34
#define ENDSTOP4 25

#endif