#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "perks.hpp"
#include "defense_perks.hpp"

class player {
public:
    player() = default;
    ~player() = default;
    player(const sf::Vector2f& player_speed, const sf::Vector2f& player_position, 
           const std::string& sprite_path, const std::string& racket_path);
    
    int get_health() const;
    sf::Vector2f get_speed() const;
    sf::Vector2f get_position() const;
    bool get_alive() const;
    defense_perks* get_current_defense() const;
    sf::FloatRect getRacketBounds() const;
    sf::FloatRect getPlayerBounds() const;
    
    void set_health(int sethealth);
    void set_speed(const sf::Vector2f& setspeed);
    void set_position(const sf::Vector2f& setposition);
    void set_alive(bool setalive);
    void set_current_defense(defense_perks* defense);
    
    void move(float deltaTime, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, 
              float windowWidth, float windowHeight);
    void restricted_area(sf::FloatRect& area);
    void draw(sf::RenderWindow& window);
    
    void check_restricted_area(const sf::FloatRect& restricted_bounds, float deltaTime);

protected:
    int health;
    sf::Vector2f speed;
    sf::Vector2f position;
    bool alive;
    sf::Texture texture;
    sf::Sprite sprite;
    defense_perks* current;
    sf::Texture racketTexture;
    sf::Sprite racketSprite;
    
    float damage_accumulator;
};

#endif  // __PLAYER_HPP