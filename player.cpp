#include "player.hpp"

player::player(const sf::Vector2f& player_speed, const sf::Vector2f& player_position, const std::string& sprite_path, const std::string& racket_path): health(100), speed(player_speed), position(player_position), alive(true), current(nullptr), damage_accumulator(0.0f) {  

    texture.loadFromFile(sprite_path);
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);  
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    sprite.setPosition(position);

    racketTexture.loadFromFile(racket_path);
    racketSprite.setTexture(racketTexture);
    racketSprite.setScale(0.2f, 0.2f);
    
    sf::FloatRect racketBounds = racketSprite.getLocalBounds();
    racketSprite.setOrigin(racketBounds.width / 2.0f, racketBounds.height / 2.0f);
    racketSprite.setPosition(position);
}

int player::get_health() const {
    return health;
}

sf::Vector2f player::get_speed() const {
    return speed;
}

sf::Vector2f player::get_position() const {
    return position;
}

bool player::get_alive() const {
    return alive;
}

defense_perks* player::get_current_defense() const {
    return current;
}

sf::FloatRect player::getRacketBounds() const {
    if (current == nullptr) {
        return racketSprite.getGlobalBounds();
    }
    return current->getDefenseBounds();
}

sf::FloatRect player::getPlayerBounds() const {
    return sprite.getGlobalBounds();
}

void player::set_health(int sethealth) {
    health = sethealth;
    if (health <= 0) {
        health = 0;
        alive = false; 
    }
}

void player::set_speed(const sf::Vector2f& setspeed) {
    speed = setspeed;
}

void player::set_position(const sf::Vector2f& setposition) {
    position = setposition;
    sprite.setPosition(position);
    
    // Update racket position too
    sf::Vector2f racketOffset(40.0f, -5.0f);  
    if (sprite.getScale().x < 0) {  
        racketOffset.x = -40.0f;  
    }
    racketSprite.setPosition(position + racketOffset);
}

void player::set_current_defense(defense_perks* defense) {
    current = defense;
}

void player::check_restricted_area(const sf::FloatRect& restricted_bounds, float deltaTime) {
    if (!alive) {
        return;
    }
    
    sf::FloatRect playerBounds = sprite.getGlobalBounds();
    
    if (restricted_bounds.intersects(playerBounds)) {
        damage_accumulator += 5.0f * deltaTime;
        
        if (damage_accumulator >= 1.0f) {
            int damage = static_cast<int>(damage_accumulator);
            health -= damage;
            damage_accumulator -= damage; 
    
            
            if (health <= 0) {
                health = 0;
                alive = false;
            }
        } 
    } else {
        damage_accumulator = 0.0f;
    }
}

void player::move(float deltaTime, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, float windowWidth, float windowHeight) {
    if (!alive) {
        return;
    }
    
    if (moveLeft) {
        position.x -= speed.x * deltaTime;
        sprite.setScale(-0.2f, 0.2f);
        racketSprite.setScale(-0.2f, 0.2f);
    }
    if (moveRight) {
        position.x += speed.x * deltaTime;
        sprite.setScale(0.2f, 0.2f);
        racketSprite.setScale(0.2f, 0.2f); 
    }
    if (moveUp) {
        position.y -= speed.y * deltaTime;
    }
    if (moveDown) {
        position.y += speed.y * deltaTime;
    }
    
    sprite.setPosition(position);
    
    sf::Vector2f racketOffset(40.0f, -5.0f);  
    if (sprite.getScale().x < 0) {  
        racketOffset.x = -40.0f;  
    }
    racketSprite.setPosition(position + racketOffset);
    
    sf::FloatRect playerBounds = sprite.getGlobalBounds();
    sf::FloatRect racketBounds = racketSprite.getGlobalBounds();
    
    float leftMost = std::min(playerBounds.left, racketBounds.left);
    float rightMost = std::max(playerBounds.left + playerBounds.width, 
                               racketBounds.left + racketBounds.width);
    float topMost = std::min(playerBounds.top, racketBounds.top);
    float bottomMost = std::max(playerBounds.top + playerBounds.height,
                                racketBounds.top + racketBounds.height);
    
    if (leftMost < 0) {
        float correction = 0 - leftMost;
        position.x += correction;
    }
    
    if (rightMost > windowWidth) {
        float correction = rightMost - windowWidth;
        position.x -= correction;
    }
    
    if (topMost < 0) {
        float correction = 0 - topMost;
        position.y += correction;
    }
    
    if (bottomMost > windowHeight) {
        float correction = bottomMost - windowHeight;
        position.y -= correction;
    }
    
    sprite.setPosition(position);
    racketSprite.setPosition(position + racketOffset);
}

void player::draw(sf::RenderWindow& window) {
    if (alive) {
        window.draw(sprite); 
        
        if (current == nullptr) {
            // Draw standard racket
            window.draw(racketSprite);
        } else {
            // Update defense perk position to match racket position before drawing
            sf::Vector2f racketOffset(40.0f, -5.0f);  
            bool facingLeft = (sprite.getScale().x < 0);
            
            if (facingLeft) {  
                racketOffset.x = -40.0f;
            }
            
            // Update the defense perk's position and orientation
            current->update_racket_position(position + racketOffset, facingLeft);
            
            // Draw defense perk as racket replacement
            current->draw_as_racket(window);
        }
    }
}