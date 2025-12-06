#ifndef __STANDARD_BALL_HPP
#define __STANDARD_BALL_HPP

#include "ball.hpp"


class standard_ball: public Ball{
    public:
        standard_ball() = default;
        ~standard_ball() = default;
        standard_ball(int ball_size, const sf::Vector2f&  ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color);
        bool player_hit(const player& p) override;
};

#endif  // __STANDARD_BALL_HPP