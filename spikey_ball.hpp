#ifndef __SPIKEY_BALL_HPP
#define __SPIKEY_BALL_HPP

#include "ball.hpp"

class spikey_ball : public Ball {
public:
    spikey_ball() = default;
    ~spikey_ball() = default;
    spikey_ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color);
    bool player_hit(const player& p) override;
    static const int DAMAGE = 5;
};

#endif  // __SPIKEY_BALL_HPP