#include "game_wifi.h"

void Game_wifi::on_data_sent(const uint8_t *mac, const esp_now_send_status_t status){
    //Serial.println("sent");
}

void Game_wifi::on_data_recv(const uint8_t *mac, const uint8_t *incoming_data, int len){
    Player_data data;
    memcpy(&data, incoming_data, sizeof(Player_data));
    Serial.print(data.id);
    Serial.print(": ");
    Serial.print(data.stick);
    Serial.print(", ");
    Serial.println(data.button);
}

bool Game_wifi::init(){
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
        return 0;

    esp_now_register_send_cb(on_data_sent);
    esp_now_register_recv_cb(on_data_recv);

    return 1;
}

int8_t Game_wifi::add_gamepad(uint8_t *gamepad_mac){
    if(added_n >= 4)
        return -1;

    memcpy(gamepads[added_n].peer_addr, gamepad_mac, 6);
    gamepads[added_n].channel = 0;
    gamepads[added_n].encrypt = false;

    esp_now_add_peer(&gamepads[added_n]);
    
    added_n++;

    return added_n - 1;
}

void Game_wifi::send_score(uint8_t score, uint8_t id){
    Motherboard_response resp;
    resp.score = score;
    resp.id = id;
    for(int i = 0; i < 4; i++){
        esp_now_send(gamepads[i].peer_addr, (uint8_t *) &resp, sizeof(Motherboard_response));
    }
}