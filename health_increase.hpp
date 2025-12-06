#ifndef __HEALTH_INCREASE_HPP
#define __HEALTH_INCREASE_HPP

#include "player_perk.hpp"

class health_increase : public player_perk {
private:
    
public:
    health_increase(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, const std::string& perk_path);
    ~health_increase() = default;
    void impact_on_player(player& p) override;
};

#endif // __HEALTH_INCREASE_HPP