#ifndef __PLAYER_PERK_HPP
#define __PLAYER_PERK_HPP

#include "perks.hpp"
#include "player.hpp"

class player_perk : public Perks {
public:
    player_perk(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, const std::string& perk_path);
    ~player_perk() = default;
    
    virtual void impact_on_player(player& p) = 0;  
};

#endif // __PLAYER_PERK_HPP