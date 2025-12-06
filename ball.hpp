#ifndef __BALL_HPP
#define __BALL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "player.hpp"

class Ball {
public:

    Ball() = default;
    ~Ball() = default;
    Ball(int ball_size, const sf::Vector2f&  ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color);
    int get_size() const;
    sf::Vector2f get_position() const;
    sf::Vector2f get_velocity() const;
    int get_time() const;
    sf::Color get_color() const;
    void set_size(int new_size);
    void set_position(const sf::Vector2f& new_position);
    void set_velocity(const sf::Vector2f& new_velocity);
    void set_time(int new_time);
    void set_color(const sf::Color& new_color);
    bool wall_collision(float width, float height); //keeping it bool to deduct a life
    void move(float deltaTime);
    void draw(sf::RenderWindow& window);
    virtual bool player_hit(const player& p) = 0;


protected:
    int size;
    sf::Vector2f position;
    sf::Vector2f velocity;
    int time;
    sf::Color color; 
    sf::CircleShape shape;
};

#endif  // __BALL_HPP

