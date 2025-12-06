#ifndef __DEFENSE_PERKS_HPP
#define __DEFENSE_PERKS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "perks.hpp"

class player; // Forward declaration

class defense_perks: public Perks {
    public:
        defense_perks() = default;
        ~defense_perks() = default;
        defense_perks(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, int perk_impact, const std::string& perk_path);
        
        sf::FloatRect getDefenseBounds() const;
        virtual void impact_on_player(player& p) = 0;
        void update_racket_position(const sf::Vector2f& position, bool facingLeft);
        void draw_as_racket(sf::RenderWindow& window);

    protected:
        int impact;
        sf::Sprite defense_sprite;  
};

#endif  // __DEFENSE_PERKS_HPP