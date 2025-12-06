#ifndef __WATER_GUN_HPP
#define __WATER_GUN_HPP

#include "defense_perks.hpp"

class water_gun : public defense_perks {
public:
    water_gun() = default;
    ~water_gun() = default;
    water_gun(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, int perk_impact, const std::string& perk_path);
    void impact_on_player(player& p) override;
};

#endif  // __WATER_GUN_HPP