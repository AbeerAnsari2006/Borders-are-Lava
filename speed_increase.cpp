#include "speed_increase.hpp"

speed_increase::speed_increase(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, const std::string& perk_path)
    : player_perk(perk_collected, perk_time, perk_appear, perk_position, perk_path) {
}

bool speed_increase::get_active() const {
    return active;
}

void speed_increase::impact_on_player(player& p) {
    active = true;                  
    activeTimer = 0.f;  
    originalSpeed = p.get_speed();
    p.set_speed(originalSpeed * 1.7f);
    
    set_collected(true);
    set_appear(false);
}


void speed_increase::update(float deltaTime, player& p) {
    if (!active) return;

    activeTimer += deltaTime;

    if (activeTimer >= 10.f) {  
        p.set_speed(originalSpeed);  
        active = false;
    }
}