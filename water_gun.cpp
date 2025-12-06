// water_gun.cpp
#include "water_gun.hpp"
#include "player.hpp"

water_gun::water_gun(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, int perk_impact,  const std::string& perk_path)
    : defense_perks(perk_collected, perk_time, perk_appear, perk_position,  perk_impact, perk_path) {
}

void water_gun::impact_on_player(player& p) {
    p.set_current_defense(this);
    set_collected(true);
    set_appear(false);
}