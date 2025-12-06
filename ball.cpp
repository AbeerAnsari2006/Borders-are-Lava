#include "ball.hpp"

Ball::Ball(int ball_size, const sf::Vector2f& ball_position, const sf::Vector2f& ball_velocity, int active_time, const sf::Color& ball_color)
    : size(ball_size), position(ball_position), velocity(ball_velocity), time(active_time), color(ball_color) {
    shape.setRadius(size);
    shape.setFillColor(color);
    shape.setOrigin(size, size);
    shape.setPosition(position);
}

int Ball::get_size() const {
    return size;
}

sf::Vector2f Ball::get_position() const {
    return position;
}

sf::Vector2f Ball::get_velocity() const {
    return velocity;
}

sf::Color Ball::get_color() const {
    return color;
}

void Ball::set_size(int new_size) {
    size = new_size;
    shape.setRadius(size);
    shape.setOrigin(size, size);
}

void Ball::set_position(const sf::Vector2f& new_position) {
    position = new_position;
    shape.setPosition(position);
}

void Ball::set_velocity(const sf::Vector2f& new_velocity) {
    velocity = new_velocity;
}

void Ball::set_time(int new_time) {
    time = new_time;
}

void Ball::set_color(const sf::Color& new_color) {
    color = new_color;
    shape.setFillColor(color);
}

bool Ball::wall_collision(float width, float height) { 
    bool hit = false;
    
    if (position.x - size <= 0) {
        position.x = size; 
        velocity.x = -velocity.x;
        hit = true;
    }
    else if (position.x + size >= width) {
        position.x = width - size;
        velocity.x = -velocity.x;
        hit = true;
    }
    
    if (position.y - size <= 0) {
        position.y = size;
        velocity.y = -velocity.y;
        hit = true;
    }
    else if (position.y + size >= height) {
        position.y = height - size;
        velocity.y = -velocity.y;
        hit = true;
    }
    
    if (hit) {
        shape.setPosition(position);
    }
    
    return hit;
}

void Ball::move(float deltaTime) {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    shape.setPosition(position);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}