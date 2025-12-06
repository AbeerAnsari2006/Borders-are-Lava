// ice_ball.hpp
#ifndef __ICE_BALL_HPP
#define __ICE_BALL_HPP

#include "ball.hpp"

class ice_ball : public Ball {
public:
    ice_ball() = default;
    ~ice_ball() = default;
    ice_ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color);
    bool player_hit(const player& p) override;
    static const int DAMAGE = 5;
};

#endif