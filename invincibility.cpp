#include "invincibility.hpp"

invincibility::invincibility(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, const std::string& perk_path)
    : player_perk(perk_collected, perk_time, perk_appear, perk_position, perk_path), active(false), activeTimer(0.f) {}

bool invincibility::get_active() const {
    return active;
}

void invincibility::impact_on_player(player& p) {
    active = true;
    activeTimer = 0.f;
    
    set_collected(true);
    set_appear(false);
}

void invincibility::update(float deltaTime) {
    if (!active) return;
    
    activeTimer += deltaTime;
    
    if (activeTimer >= 10.f) {
        active = false;
    }
}