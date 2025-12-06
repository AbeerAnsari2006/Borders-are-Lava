#include "defense_perks.hpp"

defense_perks::defense_perks(bool perk_collected, int perk_time, bool perk_appear, sf::Vector2f& perk_position, int perk_impact,  const std::string& perk_path)
    : Perks(perk_collected, perk_time, perk_appear, perk_position, perk_path), impact(perk_impact) {
    // Initialize defense sprite with the same texture as the pickup
    defense_sprite.setTexture(texture);
    defense_sprite.setScale(0.2f, 0.2f);
    sf::FloatRect bounds = defense_sprite.getLocalBounds();
    defense_sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    defense_sprite.setPosition(perk_position);
}

sf::FloatRect defense_perks::getDefenseBounds() const {
    return defense_sprite.getGlobalBounds();
}

void defense_perks::update_racket_position(const sf::Vector2f& new_position, bool facingLeft) {
    if (facingLeft) {
        defense_sprite.setScale(-0.2f, 0.2f);
    } else {
        defense_sprite.setScale(0.2f, 0.2f);
    }
    defense_sprite.setPosition(new_position);
}

void defense_perks::draw_as_racket(sf::RenderWindow& window) {
    window.draw(defense_sprite);
}