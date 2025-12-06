#ifndef __PERKS_HPP
#define __PERKS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
class player;

class Perks {
protected:
    bool collected;
    int time;
    bool appear;
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite perk;
    float elapsed_time; 
    
public:
    Perks(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, const std::string& perk_path);
    ~Perks() = default;
    
    bool get_collected() const;
    int get_time() const;
    bool get_appear() const { return appear; }
    sf::Vector2f get_position() const;
    void set_collected(bool setcollected);
    void set_time(int settime);
    void set_appear(bool setappear);
    void set_position(const sf::Vector2f& setposition);
    void draw(sf::RenderWindow& window, float deltaTime);
    bool checkCollision(const player& p);
    void update(float deltaTime);
};

#endif // __PERKS_HPP
