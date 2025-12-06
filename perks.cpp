#include "perks.hpp"
#include "perks.hpp"
#include "player.hpp"

Perks::Perks(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position,const std::string& perk_path)
    : collected(perk_collected), time(perk_time), appear(perk_appear), 
      position(perk_position) {
    texture.loadFromFile(perk_path);
    perk.setTexture(texture);
    perk.setScale(0.2f, 0.2f);  
    sf::FloatRect bounds = perk.getLocalBounds();
    perk.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    perk.setPosition(position);
}

bool Perks::get_collected() const {
    return collected;
}

int Perks::get_time() const {
    return time;
}

sf::Vector2f Perks::get_position() const {
    return position;
}

void Perks::set_collected(bool setcollected) {
    collected = setcollected;
}

void Perks::set_time(int settime) {
    time = settime;
}

void Perks::set_appear(bool setappear) {
    appear = setappear;
}

void Perks::set_position(const sf::Vector2f& setposition) {
    position = setposition;
    perk.setPosition(position);
}

void Perks::draw(sf::RenderWindow& window, float deltaTime) {
    if (appear && !collected) {
        window.draw(perk);
        time -= static_cast<int>(deltaTime * 1000);  
        if (time <= 0) {
            appear = false;
        }
    }
}

bool Perks::checkCollision(const player& p) {
    sf::FloatRect perkBounds = perk.getGlobalBounds();
    sf::FloatRect playerBounds = p.getPlayerBounds();
    
    return perkBounds.intersects(playerBounds);
}

void Perks::update(float deltaTime) {
        if (!collected && appear) {
            elapsed_time += deltaTime * 1000;  // convert to ms
            if (elapsed_time >= time) {
                appear = false;
            }
        }
    }