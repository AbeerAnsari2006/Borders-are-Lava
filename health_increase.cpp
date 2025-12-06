#include "health_increase.hpp"

health_increase::health_increase(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, const std::string& perk_path)
    : player_perk(perk_collected, perk_time, perk_appear, perk_position, perk_path) {}

void health_increase::impact_on_player(player& p) {
    int current_health = p.get_health();
    if (current_health < 100) {
        p.set_health(current_health * 1.05);
    }
    
    set_collected(true);
    set_appear(false);
}