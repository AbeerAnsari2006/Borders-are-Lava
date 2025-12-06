// poison_ball.hpp
#ifndef __POISON_BALL_HPP
#define __POISON_BALL_HPP

#include "ball.hpp"

class poison_ball : public Ball {
public:
    poison_ball() = default;
    ~poison_ball() = default;
    poison_ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color);
    bool player_hit(const player& p) override;
    static const int DAMAGE = 5;
};

#endif