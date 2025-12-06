// ice_hockey_stick.hpp
#ifndef __ICE_HOCKEY_STICK_HPP
#define __ICE_HOCKEY_STICK_HPP

#include "defense_perks.hpp"

class ice_hockey_stick : public defense_perks {
public:
    ice_hockey_stick() = default;
    ~ice_hockey_stick() = default;
    ice_hockey_stick(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, int perk_impact, const std::string& perk_path);
    void impact_on_player(player& p) override;
};

#endif  // __ICE_HOCKEY_STICK_HPP
