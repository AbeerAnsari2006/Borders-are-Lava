#ifndef __ELECTRIC_BALL_HPP
#define __ELECTRIC_BALL_HPP

#include "ball.hpp"
#include "water_gun.hpp"

class electric_ball : public Ball {
public:
    electric_ball() = default;
    ~electric_ball() = default;
    electric_ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color);
    bool player_hit(const player& p) override;
    static const int DAMAGE = 5;
};

#endif