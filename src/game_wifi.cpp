#include "game_wifi.h"

namespace Recieve{
    uint8_t *recieved_data;
    bool updates_available;
}

void Game_wifi::on_data_sent(const uint8_t *mac, const esp_now_send_status_t status){
    //Serial.println("sent");
}

void Game_wifi::on_data_recv(const uint8_t *mac, const uint8_t *incoming_data, int len){
    Recieve::recieved_data = const_cast < uint8_t* > (incoming_data);
    Recieve::updates_available = true;
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

    if(esp_now_add_peer(&gamepads[added_n]) == ESP_OK){
        connected[added_n] = true;
    }
    else{
        connected[added_n] = false;
        return -1;
    }
    
    added_n++;

    return added_n - 1;
}

bool Game_wifi::set_gamepad(uint8_t *gamepad_mac, uint8_t id){
    if(id < 0 || id > 3)
        return false;

    esp_now_del_peer(gamepads[id].peer_addr);
    connected[id] = false;

    memcpy(gamepads[id].peer_addr, gamepad_mac, 6);
    gamepads[id].channel = 0;
    gamepads[id].encrypt = false;

    if(esp_now_add_peer(&gamepads[id]) == ESP_OK){
        connected[id] = true;
    }
    else{
        connected[id] = false;
        return false;
    }
    
    return true;
}

void Game_wifi::send_score(int8_t score, uint8_t id){
    Motherboard_response resp;
    resp.score = score;
    resp.id = id;
    for(int i = 0; i < 4; i++){
        if(connected[i])
            esp_now_send(gamepads[i].peer_addr, (uint8_t *) &resp, sizeof(Motherboard_response));
    }
}

Game_wifi::Updates Game_wifi::get_updates(){
    if(!Recieve::updates_available)
        return Updates();
    
    Recieve::updates_available = false;

    Player_data data;
    memcpy(&data, Recieve::recieved_data, sizeof(Player_data));

    Updates res;
    res.success = true;
    res.id = data.id;
    res.stick = data.stick;
    res.button = data.button;

    return res;
}