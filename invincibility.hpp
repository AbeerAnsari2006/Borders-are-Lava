#ifndef INVINCIBILITY_HPP
#define INVINCIBILITY_HPP

#include "player_perk.hpp"

class invincibility : public player_perk {
private:
    bool active;
    float activeTimer;
    
public:
    invincibility(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, const std::string& perk_path);
    
    bool get_active() const;
    void impact_on_player(player& p) override;
    void update(float deltaTime);
};

#endif // INVINCIBILITY_HPP