#include "player_perk.hpp"

player_perk::player_perk(bool perk_collected, int perk_time, bool perk_appear,  sf::Vector2f& perk_position, const std::string& perk_path)
    : Perks(perk_collected, perk_time, perk_appear, perk_position, perk_path) {
}