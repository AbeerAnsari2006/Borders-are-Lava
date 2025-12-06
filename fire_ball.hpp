#ifndef __FIRE_BALL_HPP
#define __FIRE_BALL_HPP

#include "ball.hpp"
#include "ice_hockey_stick.hpp"

class fire_ball : public Ball {
public:
    fire_ball() = default;
    ~fire_ball() = default;
    fire_ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color);
    bool player_hit(const player& p) override;
    static const int DAMAGE = 5;
};

#endif