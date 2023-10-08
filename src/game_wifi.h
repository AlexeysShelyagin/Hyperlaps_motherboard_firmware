#ifndef GAME_WIFI_H
#define GAME_WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

class Game_wifi{
    esp_now_peer_info_t gamepads[4];
    bool connected[4] = {0, 0, 0, 0};
    uint8_t added_n = 0;

    struct Motherboard_response{
        uint8_t id = 0;
        int8_t score = 0;
    };

    struct Player_data{
        uint8_t id = 0;
        int8_t stick = 0;
        bool button = 0;
    };

    static void on_data_sent(const uint8_t *mac, const esp_now_send_status_t status);
    static void on_data_recv(const uint8_t *mac, const uint8_t *incoming_data, int len);

public:
    struct Updates{
        bool success = 0;
        uint8_t id = 0;
        int8_t stick = 0;
        bool button = 0;
    };

    Game_wifi() = default;

    bool init();

    int8_t add_gamepad(uint8_t *gamepad_mac);
    bool set_gamepad(uint8_t *gamepad_mac, uint8_t id);

    void send_score(int8_t score, uint8_t id);
    Updates get_updates();
};

#endif