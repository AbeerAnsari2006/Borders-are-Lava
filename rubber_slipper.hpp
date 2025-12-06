// rubber_slipper.hpp
#ifndef __RUBBER_SLIPPER_HPP
#define __RUBBER_SLIPPER_HPP

#include "defense_perks.hpp"

class rubber_slipper : public defense_perks {
public:
    rubber_slipper() = default;
    ~rubber_slipper() = default;
    rubber_slipper(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, int perk_impact, const std::string& perk_path);
    void impact_on_player(player& p) override;
};

#endif  // __RUBBER_SLIPPER_HPP
