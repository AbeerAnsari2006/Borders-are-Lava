#include "fire_ball.hpp"

fire_ball::fire_ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color)
    : Ball(ball_size, ball_position, ball_velocity, active_time, ball_color) {}

bool fire_ball::player_hit(const player& p) {
    if (!p.get_alive()) {
        return false;
    }
    
    sf::FloatRect ballBounds = shape.getGlobalBounds();
    sf::FloatRect racket_bound = p.getRacketBounds();
    
    if (!ballBounds.intersects(racket_bound)) {
        return false;
    }
    
    defense_perks* defense = p.get_current_defense();
    bool has_protection = (defense != nullptr && 
                          dynamic_cast<ice_hockey_stick*>(defense) != nullptr);
    
    float ballLeft = ballBounds.left;
    float ballRight = ballBounds.left + ballBounds.width;
    float ballTop = ballBounds.top;
    float ballBottom = ballBounds.top + ballBounds.height;
    
    float racketLeft = racket_bound.left;
    float racketRight = racket_bound.left + racket_bound.width;
    float racketTop = racket_bound.top;
    float racketBottom = racket_bound.top + racket_bound.height;
    
    float overlapLeft = ballRight - racketLeft;
    float overlapRight = racketRight - ballLeft;
    float overlapTop = ballBottom - racketTop;
    float overlapBottom = racketBottom - ballTop;
    
    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
    
    bool hit = false;
    
    if (minOverlap == overlapLeft && velocity.x > 0) {
        velocity.x = -velocity.x;
        position.x = racketLeft - ballBounds.width / 2.0f - 2.0f;
        hit = true;
    }
    else if (minOverlap == overlapRight && velocity.x < 0) {
        velocity.x = -velocity.x;
        position.x = racketRight + ballBounds.width / 2.0f + 2.0f;
        hit = true;
    }
    else if (minOverlap == overlapTop && velocity.y > 0) {
        velocity.y = -velocity.y;
        position.y = racketTop - ballBounds.height / 2.0f - 2.0f;
        hit = true;
    }
    else if (minOverlap == overlapBottom && velocity.y < 0) {
        velocity.y = -velocity.y;
        position.y = racketBottom + ballBounds.height / 2.0f + 2.0f;
        hit = true;
    }
    
    if (hit) {
        shape.setPosition(position);
        if (!has_protection) {
            const_cast<player&>(p).set_health(p.get_health() - DAMAGE);
        }
    }
    
    return hit;
}