#include "standard_ball.hpp"

standard_ball::standard_ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color)
    : Ball(ball_size, ball_position, ball_velocity, active_time, ball_color) {
}


bool standard_ball::player_hit(const player& p) {
    if (!p.get_alive()) {
        return false;
    }
    
    sf::FloatRect ballBounds = shape.getGlobalBounds();
    sf::FloatRect racket_bound = p.getRacketBounds();
    
    if (!ballBounds.intersects(racket_bound)) {
        return false;
    }
    
    // Calculate overlap on each side
    float ballLeft = ballBounds.left;
    float ballRight = ballBounds.left + ballBounds.width;
    float ballTop = ballBounds.top;
    float ballBottom = ballBounds.top + ballBounds.height;
    
    float racketLeft = racket_bound.left;
    float racketRight = racket_bound.left + racket_bound.width;
    float racketTop = racket_bound.top;
    float racketBottom = racket_bound.top + racket_bound.height;
    
    // Calculate overlap distances
    float overlapLeft = ballRight - racketLeft;
    float overlapRight = racketRight - ballLeft;
    float overlapTop = ballBottom - racketTop;
    float overlapBottom = racketBottom - ballTop;
    
    // Find the smallest overlap (that's the collision side)
    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
    
    // Bounce based on which side has minimum overlap
    if (minOverlap == overlapLeft && velocity.x > 0) {
        // Hit left side of racket
        velocity.x = -velocity.x;
        position.x = racketLeft - ballBounds.width / 2.0f - 2.0f;
        return true;
    }
    else if (minOverlap == overlapRight && velocity.x < 0) {
        // Hit right side of racket
        velocity.x = -velocity.x;
        position.x = racketRight + ballBounds.width / 2.0f + 2.0f;
        return true;
    }
    else if (minOverlap == overlapTop && velocity.y > 0) {
        // Hit top side of racket
        velocity.y = -velocity.y;
        position.y = racketTop - ballBounds.height / 2.0f - 2.0f;
        return true;
    }
    else if (minOverlap == overlapBottom && velocity.y < 0) {
        // Hit bottom side of racket
        velocity.y = -velocity.y;
        position.y = racketBottom + ballBounds.height / 2.0f + 2.0f;
        return true;
    }
    
    shape.setPosition(position);
    return false;
}

