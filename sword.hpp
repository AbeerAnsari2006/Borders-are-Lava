// sword.hpp
#ifndef __SWORD_HPP
#define __SWORD_HPP

#include "defense_perks.hpp"

class sword : public defense_perks {
public:
    sword() = default;
    ~sword() = default;
    sword(bool perk_collected, int perk_time, bool perk_appear,  sf::Vector2f& perk_position, int perk_impact, const std::string& perk_path);
    void impact_on_player(player& p) override;
};

#endif  // __SWORD_HPP
