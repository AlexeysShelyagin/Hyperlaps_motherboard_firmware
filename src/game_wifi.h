#ifndef GAME_WIFI_H
#define GAME_WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

class Game_wifi{
    esp_now_peer_info_t gamepads[4];
    uint8_t added_n = 0;

    struct Player_data{
        uint8_t id = 0;
        int8_t stick = 0;
        bool button = 0;
    };

    struct Motherboard_response{
        uint8_t id = 0;
        uint8_t score = 0;
    };

    static void on_data_sent(const uint8_t *mac, const esp_now_send_status_t status);
    static void on_data_recv(const uint8_t *mac, const uint8_t *incoming_data, int len);

public:
    Game_wifi() = default;

    bool init();

    int8_t add_gamepad(uint8_t *gamepad_mac);

    void send_score(uint8_t score, uint8_t id);
};

#endif