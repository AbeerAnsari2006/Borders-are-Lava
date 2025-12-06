#ifndef __GAME_HPP
#define __GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <typeinfo>
#include "player.hpp"
#include "ball.hpp"
#include "standard_ball.hpp"
#include "spikey_ball.hpp"
#include "electric_ball.hpp"
#include "poison_ball.hpp"
#include "fire_ball.hpp"
#include "ice_ball.hpp"
#include "player_perk.hpp"
#include "defense_perks.hpp"
#include "health_increase.hpp"
#include "speed_increase.hpp"
#include "invincibility.hpp"
#include "water_gun.hpp"
#include "rubber_slipper.hpp"
#include "poison_resistant_racket.hpp"
#include "ice_hockey_stick.hpp"
#include "sword.hpp"

enum class BallType {
    STANDARD,
    SPIKEY,
    ELECTRIC,
    POISON,
    FIRE,
    ICE
};

class Game {
public:
    Game();
    ~Game();
    void welcome_screen();
    void game_screen();
    void lives_screen();
    void game_over_screen();
    void resetGame();
    void randomize_positions();
    void draw_score();

    // Restricted area functions
    void spawn_restricted_area();
    void update_restricted_area(float deltaTime);
    void draw_restricted_area();
    void draw_health_bars();
    
    // Ball management
    void switch_ball_type();
    Ball* create_ball(BallType type);
    void spawn_defense_perk_for_ball_type(BallType type);
    void update_ball_system(float deltaTime);
    void cleanup_defense_perks();
    
    // Perk functions
    void spawn_random_player_perk();
    void update_player_perks(float deltaTime);
    void update_defense_perks(float deltaTime);
    void draw_player_perks();
    void draw_defense_perks();
    void cleanup_all_perks();

private:
    static constexpr float WINDOW_WIDTH = 1920.0f;
    static constexpr float WINDOW_HEIGHT = 1080.0f;
    static int high_score;
    
    player red_player;
    player blue_player;
    Ball* game_ball;
    int lives;
    sf::Color background_color;
    bool game_start;
    sf::RenderWindow window;
    int score;
    
    // Ball system
    BallType current_ball_type;
    BallType next_ball_type;
    sf::Clock ball_type_timer;
    float current_ball_duration;
    bool defense_perk_spawned;
    defense_perks* active_defense_perk;
    
    // Restricted area
    sf::RectangleShape restricted_area;
    bool is_red_player_restricted;
    sf::Clock restricted_area_spawn_clock;
    float restricted_area_duration;
    bool restricted_area_active;
    
    // Perks
    std::vector<player_perk*> player_perks;
    sf::Clock playerPerkSpawnTimer;
    float nextPlayerPerkSpawnTime;
    speed_increase* red_speed_perk; 
    speed_increase* blue_speed_perk;
    invincibility* invincibility_perk;
};

#endif // __GAME_HPP