#ifndef __SPEED_INCREASE_HPP
#define __SPEED_INCREASE_HPP

#include "player_perk.hpp"

class speed_increase : public player_perk {
    private:
        sf::Vector2f originalSpeed;
        bool active = false;
        float activeTimer = 0.f;
        
    public:
        speed_increase(bool perk_collected, int perk_time, bool perk_appear, 
                        sf::Vector2f& perk_position, const std::string& perk_path);
        ~speed_increase() = default;
        
        void impact_on_player(player& p) override;
        void update(float deltaTime, player& p);
        bool get_active() const;
    };

#endif // __SPEED_INCREASE_HPP