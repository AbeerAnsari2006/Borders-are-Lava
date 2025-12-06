#include "game.hpp"

// Initialize static variable
int Game::high_score = 0;

Game::Game(): 
    red_player(sf::Vector2f(600.0f, 600.0f), sf::Vector2f(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2), "red_player.png", "red_racket.png"),
    blue_player(sf::Vector2f(600.0f, 600.0f), sf::Vector2f(3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2), "blue_player.png", "blue_racket.png"),
    game_ball(nullptr), 
    lives(3), 
    background_color(sf::Color::Black), 
    game_start(false), 
    window(sf::VideoMode(1920, 1080), "Welcome To The Borders Are Lava", sf::Style::Default),
    is_red_player_restricted(false),
    restricted_area_duration(0),
    restricted_area_active(false),
    nextPlayerPerkSpawnTime(10.0f),
    red_speed_perk(nullptr),
    blue_speed_perk(nullptr),
    invincibility_perk(nullptr),
    score(0),
    current_ball_type(BallType::STANDARD),
    next_ball_type(BallType::STANDARD),
    current_ball_duration(0),
    defense_perk_spawned(false),
    active_defense_perk(nullptr)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    randomize_positions();
    game_ball = create_ball(BallType::STANDARD);
}

Game::~Game() {
    if (game_ball != nullptr) {
        delete game_ball;
    }
    cleanup_all_perks();
}

Ball* Game::create_ball(BallType type) {
    sf::Vector2f position(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    sf::Vector2f velocity(200.0f, 150.0f);
    
    switch(type) {
        case BallType::SPIKEY:
            return new spikey_ball(10, position, velocity, 0, sf::Color(128, 128, 128)); // Gray
        case BallType::ELECTRIC:
            return new electric_ball(10, position, velocity, 0, sf::Color::Yellow);
        case BallType::POISON:
            return new poison_ball(10, position, velocity, 0, sf::Color::Green);
        case BallType::FIRE:
            return new fire_ball(10, position, velocity, 0, sf::Color::Red);
        case BallType::ICE:
            return new ice_ball(10, position, velocity, 0, sf::Color::Cyan);
        default:
            return new standard_ball(10, position, velocity, 0, sf::Color::White);
    }
}

void Game::spawn_defense_perk_for_ball_type(BallType type) {
    if (type == BallType::STANDARD) {
        return; 
    }
    
    float x = 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_WIDTH - 200));
    float y = 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_HEIGHT - 200));
    sf::Vector2f position(x, y);
    int duration = 10000; 
    
    defense_perks* perk = nullptr;
    
    switch(type) {
        case BallType::SPIKEY:
            perk = new rubber_slipper(false, duration, true, position, 0, "rubber_slipper.png");
            break;
        case BallType::ELECTRIC:
            perk = new water_gun(false, duration, true, position, 0, "water_gun.png");
            break;
        case BallType::POISON:
            perk = new poison_resistant_racket(false, duration, true, position, 0, "poison_racket.png");
            break;
        case BallType::FIRE:
            perk = new ice_hockey_stick(false, duration, true, position, 0, "ice_stick.png");
            break;
        case BallType::ICE:
            perk = new sword(false, duration, true, position, 0, "sword.png");
            break;
        default:
            break;
    }
    
    if (perk != nullptr) {
        active_defense_perk = perk;
        defense_perk_spawned = true;
    }
}

void Game::switch_ball_type() {
    current_ball_duration = 10.0f + static_cast<float>(std::rand() % 11);
    
    // Use the pre-determined next ball type
    current_ball_type = next_ball_type;
    
    // Delete old ball and create new one
    if (game_ball != nullptr) {
        sf::Vector2f current_pos = game_ball->get_position();
        sf::Vector2f current_vel = game_ball->get_velocity();
        delete game_ball;
        
        game_ball = create_ball(current_ball_type);
        game_ball->set_position(current_pos);
        game_ball->set_velocity(current_vel);
    }
    
    // Clean up any existing defense perk
    if (active_defense_perk != nullptr && !active_defense_perk->get_collected()) {
        delete active_defense_perk;
        active_defense_perk = nullptr;
    } else if (active_defense_perk != nullptr && active_defense_perk->get_collected()) {
        // Perk was collected - set to null but DON'T delete
        active_defense_perk = nullptr;
    }
    
    defense_perk_spawned = false;
    ball_type_timer.restart();    
}

void Game::update_ball_system(float deltaTime) {
    float elapsed = ball_type_timer.getElapsedTime().asSeconds();
    
    // Spawn defense perk 5 seconds before ball change for NEXT ball type
    if (!defense_perk_spawned && elapsed >= (current_ball_duration - 5.0f)) {
        // Determine what the next ball will be
        int next_type_choice = std::rand() % 6;
        next_ball_type = static_cast<BallType>(next_type_choice);
        
        // Spawn the defense perk for the next ball type
        spawn_defense_perk_for_ball_type(next_ball_type);
    }
    
    // Switch ball type after duration
    if (elapsed >= current_ball_duration) {
        switch_ball_type();
    }
}

void Game::cleanup_defense_perks() {
    if (active_defense_perk != nullptr) {
        if (red_player.get_current_defense() == active_defense_perk) {
            red_player.set_current_defense(nullptr);
        }
        if (blue_player.get_current_defense() == active_defense_perk) {
            blue_player.set_current_defense(nullptr);
        }
        
        delete active_defense_perk;
        active_defense_perk = nullptr;
    }
}

void Game::update_defense_perks(float deltaTime) {
    if (active_defense_perk == nullptr) {
        return;
    }
    
    // If already collected, don't do anything - just keep it alive for the player to use
    if (active_defense_perk->get_collected()) {
        return;  
    }
    
    // Perk not collected yet - check for collision and update timer
    if (active_defense_perk->get_appear()) {
        active_defense_perk->update(deltaTime);
        
        if (active_defense_perk->checkCollision(red_player)) {
            active_defense_perk->impact_on_player(red_player);
            return;  
        }
        else if (active_defense_perk->checkCollision(blue_player)) {
            active_defense_perk->impact_on_player(blue_player);
            return;  
        }
    }
    
    if (!active_defense_perk->get_appear()) {
        delete active_defense_perk;
        active_defense_perk = nullptr;
    }
}

void Game::draw_defense_perks() {
    if (active_defense_perk != nullptr && active_defense_perk->get_appear()) {
        active_defense_perk->draw(window, 0);
    }
}

void Game::spawn_restricted_area() {
    float width = 200.0f + static_cast<float>(std::rand() % 201);
    float height = 200.0f + static_cast<float>(std::rand() % 201);
    
    float x = static_cast<float>(std::rand() % static_cast<int>(WINDOW_WIDTH - width));
    float y = static_cast<float>(std::rand() % static_cast<int>(WINDOW_HEIGHT - height));
    
    is_red_player_restricted = (std::rand() % 2 == 0);
    
    restricted_area.setSize(sf::Vector2f(width, height));
    restricted_area.setPosition(x, y);
    
    if (is_red_player_restricted) {
        restricted_area.setFillColor(sf::Color(255, 0, 0, 100)); 
        restricted_area.setOutlineColor(sf::Color::Red);
    } else {
        restricted_area.setFillColor(sf::Color(0, 0, 255, 100)); 
        restricted_area.setOutlineColor(sf::Color::Blue);
    }
    restricted_area.setOutlineThickness(3.0f);
    
    restricted_area_duration = 10.0f + static_cast<float>(std::rand() % 11);
    restricted_area_active = true;
    restricted_area_spawn_clock.restart();
}

void Game::update_restricted_area(float deltaTime) {
    if (!restricted_area_active) {
        return;
    }
    
    if (restricted_area_spawn_clock.getElapsedTime().asSeconds() >= restricted_area_duration) {
        restricted_area_active = false;
        return;
    }
    
    if (is_red_player_restricted) {
        red_player.check_restricted_area(restricted_area.getGlobalBounds(), deltaTime);
    } else {
        blue_player.check_restricted_area(restricted_area.getGlobalBounds(), deltaTime);
    }
}

void Game::draw_restricted_area() {
    if (restricted_area_active) {
        window.draw(restricted_area);
    }
}

void Game::draw_health_bars() {
    float barWidth = 300.0f;
    float barHeight = 30.0f;
    float barSpacing = 50.0f;
    float rightMargin = 20.0f;
    float topMargin = 20.0f;
    
    float redX = WINDOW_WIDTH - barWidth - rightMargin;
    float redY = topMargin;
    
    sf::RectangleShape redBg(sf::Vector2f(barWidth, barHeight));
    redBg.setPosition(redX, redY);
    redBg.setFillColor(sf::Color(50, 50, 50));
    redBg.setOutlineThickness(2.0f);
    redBg.setOutlineColor(sf::Color::White);
    
    float redHealthPercent = red_player.get_health() / 100.0f;
    sf::RectangleShape redFill(sf::Vector2f(barWidth * redHealthPercent, barHeight));
    redFill.setPosition(redX, redY);
    redFill.setFillColor(sf::Color(220, 20, 60)); 
    
    sf::Font font;
    if (font.loadFromFile("./AmongUs-regular.ttf")) {
        sf::Text redLabel;
        redLabel.setFont(font);
        redLabel.setString("RED: " + std::to_string(red_player.get_health()) + "%");
        redLabel.setCharacterSize(20);
        redLabel.setFillColor(sf::Color::White);
        redLabel.setPosition(redX + 5, redY + 5);
        
        window.draw(redBg);
        window.draw(redFill);
        window.draw(redLabel);
    } else {
        window.draw(redBg);
        window.draw(redFill);
    }
    
    float blueX = WINDOW_WIDTH - barWidth - rightMargin;
    float blueY = topMargin + barHeight + barSpacing;
    
    sf::RectangleShape blueBg(sf::Vector2f(barWidth, barHeight));
    blueBg.setPosition(blueX, blueY);
    blueBg.setFillColor(sf::Color(50, 50, 50));
    blueBg.setOutlineThickness(2.0f);
    blueBg.setOutlineColor(sf::Color::White);
    
    float blueHealthPercent = blue_player.get_health() / 100.0f;
    sf::RectangleShape blueFill(sf::Vector2f(barWidth * blueHealthPercent, barHeight));
    blueFill.setPosition(blueX, blueY);
    blueFill.setFillColor(sf::Color(30, 144, 255)); 
    
    if (font.loadFromFile("./AmongUs-regular.ttf")) {
        sf::Text blueLabel;
        blueLabel.setFont(font);
        blueLabel.setString("BLUE: " + std::to_string(blue_player.get_health()) + "%");
        blueLabel.setCharacterSize(20);
        blueLabel.setFillColor(sf::Color::White);
        blueLabel.setPosition(blueX + 5, blueY + 5);
        
        window.draw(blueBg);
        window.draw(blueFill);
        window.draw(blueLabel);
    } else {
        window.draw(blueBg);
        window.draw(blueFill);
    }
}

void Game::draw_score() {
    sf::Font font;
    if (!font.loadFromFile("./AmongUs-regular.ttf")) {
        return;
    }
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(35);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 80);
    
    window.draw(scoreText);
}

void Game::randomize_positions() {
    float red_x = 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_WIDTH / 2 - 200));
    float red_y = 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_HEIGHT - 200));
    red_player.set_position(sf::Vector2f(red_x, red_y));
    
    float blue_x = WINDOW_WIDTH / 2 + 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_WIDTH / 2 - 200));
    float blue_y = 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_HEIGHT - 200));
    blue_player.set_position(sf::Vector2f(blue_x, blue_y));
    
    float ball_x = WINDOW_WIDTH / 4 + static_cast<float>(std::rand() % static_cast<int>(WINDOW_WIDTH / 2));
    float ball_y = WINDOW_HEIGHT / 4 + static_cast<float>(std::rand() % static_cast<int>(WINDOW_HEIGHT / 2));
    if (game_ball != nullptr) {
        game_ball->set_position(sf::Vector2f(ball_x, ball_y));
    }
}

void Game::spawn_random_player_perk() {
    float x = 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_WIDTH - 200));
    float y = 100.0f + static_cast<float>(std::rand() % static_cast<int>(WINDOW_HEIGHT - 200));
    sf::Vector2f position(x, y);
    int duration = 5000;
    
    player_perk* perk = nullptr;
    
    int perkType = std::rand() % 3;
    if (perkType == 0) {
        perk = new health_increase(false, duration, true, position, "health_perk.png");
    } else if (perkType == 1) {
        perk = new speed_increase(false, duration, true, position, "speed_perk.png");
    } else {
        perk = new invincibility(false, duration, true, position, "invincibility.png");
    }
    
    if (perk != nullptr) {
        player_perks.push_back(perk);
    }
    
    nextPlayerPerkSpawnTime = 15.0f + static_cast<float>(std::rand() % 11);
    playerPerkSpawnTimer.restart();
}

void Game::update_player_perks(float deltaTime) {
    for (auto it = player_perks.begin(); it != player_perks.end(); ) {
        player_perk* perk = *it;
        
        if (perk->get_appear() && !perk->get_collected()) {
            perk->update(deltaTime);
            
            if (perk->checkCollision(red_player)) {
                perk->impact_on_player(red_player);
                
                speed_increase* speedPerk = dynamic_cast<speed_increase*>(perk);
                if (speedPerk != nullptr) {
                    if (red_speed_perk != nullptr) {
                        red_speed_perk->update(0, red_player);
                    }
                    red_speed_perk = speedPerk;
                }
                
                invincibility* invincPerk = dynamic_cast<invincibility*>(perk);
                if (invincPerk != nullptr) {
                    if (invincibility_perk != nullptr && invincibility_perk->get_active()) {
                        invincibility_perk->update(10.0f);
                    }
                    invincibility_perk = invincPerk;
                }
            }
            else if (perk->checkCollision(blue_player)) {
                perk->impact_on_player(blue_player);
                
                speed_increase* speedPerk = dynamic_cast<speed_increase*>(perk);
                if (speedPerk != nullptr) {
                    if (blue_speed_perk != nullptr) {
                        blue_speed_perk->update(0, blue_player);
                    }
                    blue_speed_perk = speedPerk;
                }
                
                invincibility* invincPerk = dynamic_cast<invincibility*>(perk);
                if (invincPerk != nullptr) {
                    if (invincibility_perk != nullptr && invincibility_perk->get_active()) {
                        invincibility_perk->update(10.0f);
                    }
                    invincibility_perk = invincPerk;
                }
            }
        }
        
        if (!perk->get_appear()) {
            if (perk == red_speed_perk) {
                red_speed_perk = nullptr;
            }
            if (perk == blue_speed_perk) {
                blue_speed_perk = nullptr;
            }
            if (perk == invincibility_perk) {
                invincibility_perk = nullptr;
            }
            
            delete perk;
            it = player_perks.erase(it);
        } else {
            ++it;
        }
    }
    
    if (red_speed_perk != nullptr) {
        red_speed_perk->update(deltaTime, red_player);
        if (!red_speed_perk->get_active()) {
            red_speed_perk = nullptr;
        }
    }
    
    if (blue_speed_perk != nullptr) {
        blue_speed_perk->update(deltaTime, blue_player);
        if (!blue_speed_perk->get_active()) {
            blue_speed_perk = nullptr;
        }
    }
    
    if (invincibility_perk != nullptr) {
        invincibility_perk->update(deltaTime);
        if (!invincibility_perk->get_active()) {
            invincibility_perk = nullptr;
        }
    }
}

void Game::draw_player_perks() {
    for (player_perk* perk : player_perks) {
        perk->draw(window, 0);
    }
}

void Game::cleanup_all_perks() {
    for (player_perk* perk : player_perks) {
        delete perk;
    }
    player_perks.clear();
    
    cleanup_defense_perks();
    
    red_speed_perk = nullptr;
    blue_speed_perk = nullptr;
    invincibility_perk = nullptr;
}

void Game::game_screen() {
    sf::Clock clock;
    sf::Clock restrictedAreaSpawnTimer; 
    playerPerkSpawnTimer.restart();
    ball_type_timer.restart();
    current_ball_duration = 15.0f; // Initial ball duration
    
    float nextRestrictedAreaSpawnTime = 5.0f;
    
    while (window.isOpen() && game_start && lives > 0) {
        float deltaTime = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }
        
        if (restrictedAreaSpawnTimer.getElapsedTime().asSeconds() >= nextRestrictedAreaSpawnTime) {
            spawn_restricted_area();
            restrictedAreaSpawnTimer.restart();
            nextRestrictedAreaSpawnTime = 15.0f + static_cast<float>(std::rand() % 11);
        }
        
        if (playerPerkSpawnTimer.getElapsedTime().asSeconds() >= nextPlayerPerkSpawnTime) {
            spawn_random_player_perk();
        }
        
        // Update ball system
        update_ball_system(deltaTime);
        
        bool redMoveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool redMoveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool redMoveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        bool redMoveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        
        bool blueMoveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool blueMoveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool blueMoveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool blueMoveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        
        red_player.move(deltaTime, redMoveLeft, redMoveRight, redMoveUp, redMoveDown, 
                       WINDOW_WIDTH, WINDOW_HEIGHT);
        blue_player.move(deltaTime, blueMoveLeft, blueMoveRight, blueMoveUp, blueMoveDown, 
                        WINDOW_WIDTH, WINDOW_HEIGHT);
        
        if (game_ball != nullptr) {
            game_ball->move(deltaTime);
            bool red_hit = game_ball->player_hit(red_player);
            bool blue_hit = game_ball->player_hit(blue_player);
            
            if (red_hit || blue_hit) {
                score++;
            }
        }
        
        update_restricted_area(deltaTime);
        update_player_perks(deltaTime);
        update_defense_perks(deltaTime);
        
        bool hit = game_ball->wall_collision(WINDOW_WIDTH, WINDOW_HEIGHT);
        if (hit && (invincibility_perk == nullptr || !invincibility_perk->get_active())) {
            lives--;
        }
        
        window.clear(background_color);
        
        draw_restricted_area(); 
        draw_player_perks();
        draw_defense_perks();
        red_player.draw(window);
        blue_player.draw(window);
        if (game_ball != nullptr) {
            game_ball->draw(window);
        }
        
        lives_screen();
        draw_health_bars();
        draw_score(); 
        
        window.display();
    }
    
    cleanup_all_perks();
    
    if (lives <= 0) {
        if (score > high_score) {
            high_score = score;
        }
        game_over_screen();
    }
}

void Game::resetGame() {
    lives = 3;
    game_start = false;
    score = 0;
    
    if (game_ball != nullptr) {
        delete game_ball;
    }
    current_ball_type = BallType::STANDARD;
    next_ball_type = BallType::STANDARD;
    game_ball = create_ball(BallType::STANDARD);
    
    red_player.set_health(100);
    blue_player.set_health(100);
    red_player.set_current_defense(nullptr);
    blue_player.set_current_defense(nullptr);
    
    restricted_area_active = false;
    defense_perk_spawned = false;
    
    cleanup_all_perks();
    nextPlayerPerkSpawnTime = 10.0f;
    
    randomize_positions();
}

void Game::welcome_screen() {
    window.clear(sf::Color::Black);
    window.display();
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setPosition(0.f, 0.f);
    background.setFillColor(sf::Color::Black);
    
    sf::Texture amongUs;
    amongUs.loadFromFile("welcome_screen.png");
    sf::Sprite spriteAmongUs(amongUs);
    spriteAmongUs.setPosition(418, 220);
    
    sf::Texture button;
    button.loadFromFile("button.png");
    
    sf::Font titleFont;
    titleFont.loadFromFile("./AmongUs-caps.ttf");
    
    sf::Font regularFont;
    regularFont.loadFromFile("./AmongUs-regular.ttf");
    
    sf::Text titleText;
    titleText.setFont(titleFont);
    titleText.setString("THE BORDERS ARE LAVA");
    titleText.setCharacterSize(125);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(75, 115);
    
    sf::Sprite spriteButton1(button);
    spriteButton1.setPosition(440, 850);
    sf::RectangleShape rectangle1(sf::Vector2f(500.f, 70.f));
    rectangle1.setPosition(440.f, 850.f);
    rectangle1.setFillColor(sf::Color::Black);
    
    sf::Text buttonText1;
    buttonText1.setFont(regularFont);
    buttonText1.setString("Dark Mode");
    buttonText1.setCharacterSize(50);
    sf::FloatRect bounds1 = buttonText1.getLocalBounds();
    buttonText1.setFillColor(sf::Color::White);
    buttonText1.setPosition(690 - ((bounds1.width) / 2), 850);
    
    sf::Sprite spriteButton2(button);
    spriteButton2.setPosition(980, 850);
    sf::RectangleShape rectangle2(sf::Vector2f(500.f, 70.f));
    rectangle2.setPosition(980.f, 850.f);
    rectangle2.setFillColor(sf::Color::Black);
    
    sf::Text buttonText2;
    buttonText2.setFont(regularFont);
    buttonText2.setString("Light Mode");
    buttonText2.setCharacterSize(50);
    sf::FloatRect bounds2 = buttonText2.getLocalBounds();
    buttonText2.setFillColor(sf::Color::White);
    buttonText2.setPosition(1230 - ((bounds2.width) / 2), 850);
    
    sf::Sprite spriteButton3(button);
    spriteButton3.setPosition(710, 940);
    sf::RectangleShape rectangle3(sf::Vector2f(500.f, 70.f));
    rectangle3.setPosition(710.f, 940.f);
    rectangle3.setFillColor(sf::Color::Black);
    
    sf::Text buttonText3;
    buttonText3.setFont(regularFont);
    buttonText3.setString("Start Game");
    buttonText3.setCharacterSize(50);
    sf::FloatRect bounds3 = buttonText3.getLocalBounds();
    buttonText3.setFillColor(sf::Color::White);
    buttonText3.setPosition(960 - ((bounds3.width) / 2), 940);

    background_color = sf::Color(background.getFillColor()); 
    
    while (window.isOpen() && !game_start) {
        sf::Vector2i mousePosHover = sf::Mouse::getPosition(window);
        
        if (rectangle3.getGlobalBounds().contains(mousePosHover.x, mousePosHover.y)) {
            rectangle3.setFillColor(sf::Color(200, 200, 200));
        } else {
            rectangle3.setFillColor(sf::Color(background.getFillColor()));
        }
        
        if (rectangle2.getGlobalBounds().contains(mousePosHover.x, mousePosHover.y)) {
            rectangle2.setFillColor(sf::Color(200, 200, 200));
        } else {
            rectangle2.setFillColor(sf::Color(background.getFillColor()));
        }
        
        if (rectangle1.getGlobalBounds().contains(mousePosHover.x, mousePosHover.y)) {
            rectangle1.setFillColor(sf::Color(200, 200, 200));
        } else {
            rectangle1.setFillColor(sf::Color(background.getFillColor()));
        }
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                std::cout << "Closed the window";
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                
                if (rectangle1.getGlobalBounds().contains(mousePos)) {
                    rectangle1.setFillColor(sf::Color::Black);
                    rectangle2.setFillColor(sf::Color::Black);
                    rectangle3.setFillColor(sf::Color::Black);
                    background.setFillColor(sf::Color::Black);
                    buttonText1.setFillColor(sf::Color::White);
                    buttonText2.setFillColor(sf::Color::White);
                    buttonText3.setFillColor(sf::Color::White);
                    background_color = sf::Color::Black;
                }
                
                if (rectangle2.getGlobalBounds().contains(mousePos)) {
                    rectangle1.setFillColor(sf::Color(173, 216, 230));
                    rectangle2.setFillColor(sf::Color(173, 216, 230));
                    rectangle3.setFillColor(sf::Color(173, 216, 230));
                    background.setFillColor(sf::Color(173, 216, 230));
                    buttonText1.setFillColor(sf::Color::Black);
                    buttonText2.setFillColor(sf::Color::Black);
                    buttonText3.setFillColor(sf::Color::Black);
                    background_color = sf::Color(173, 216, 230);
                }
                
                if (rectangle3.getGlobalBounds().contains(mousePos)) {
                    window.clear(sf::Color::Black);
                    game_start = true;
                    game_screen();
                }
            }
        }
        
        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(spriteAmongUs);
        window.draw(rectangle1);
        window.draw(rectangle2);
        window.draw(rectangle3);
        window.draw(spriteButton1);
        window.draw(spriteButton2);
        window.draw(spriteButton3);
        window.draw(titleText);
        window.draw(buttonText1);
        window.draw(buttonText2);
        window.draw(buttonText3);
        window.display();
    }
}

void Game::lives_screen() {
    sf::Texture heartTexture;
    heartTexture.loadFromFile("life_heart.png");
    
    sf::Sprite heart1(heartTexture);
    heart1.setPosition(10, 10);
    
    sf::Sprite heart2(heartTexture);
    heart2.setPosition(70, 10);
    
    sf::Sprite heart3(heartTexture);
    heart3.setPosition(130, 10);
    
    if (lives >= 1) window.draw(heart1);
    if (lives >= 2) window.draw(heart2);
    if (lives >= 3) window.draw(heart3);
}

void Game::game_over_screen() {
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setPosition(0.f, 0.f);
    background.setFillColor(background_color);
    
    sf::Font titleFont;
    if (!titleFont.loadFromFile("./AmongUs-caps.ttf")) {
        std::cerr << "ERROR: Failed to load AmongUs-caps.ttf" << std::endl;
        return;
    }
    
    sf::Font regularFont;
    if (!regularFont.loadFromFile("./AmongUs-regular.ttf")) {
        std::cerr << "ERROR: Failed to load AmongUs-regular.ttf" << std::endl;
        return;
    }
    
    sf::Text loseText;
    loseText.setFont(titleFont);
    loseText.setString("YOU LOSE");
    loseText.setCharacterSize(150);
    loseText.setFillColor(sf::Color::Red);
    sf::FloatRect loseBounds = loseText.getLocalBounds();
    loseText.setPosition((WINDOW_WIDTH - loseBounds.width) / 2, WINDOW_HEIGHT / 5);
    
    // Score display
    sf::Text scoreText;
    scoreText.setFont(regularFont);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(60);
    scoreText.setFillColor(sf::Color::White);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition((WINDOW_WIDTH - scoreBounds.width) / 2, WINDOW_HEIGHT / 2 - 50);
    
    // High score display
    sf::Text highScoreText;
    highScoreText.setFont(regularFont);
    highScoreText.setString("High Score: " + std::to_string(high_score));
    highScoreText.setCharacterSize(60);
    highScoreText.setFillColor(sf::Color::Yellow);
    sf::FloatRect highScoreBounds = highScoreText.getLocalBounds();
    highScoreText.setPosition((WINDOW_WIDTH - highScoreBounds.width) / 2, WINDOW_HEIGHT / 2 + 30);
    
    sf::Texture button;
    if (!button.loadFromFile("button.png")) {
        std::cerr << "ERROR: Failed to load button.png" << std::endl;
        return;
    }
    
    sf::Sprite playAgainButton(button);
    playAgainButton.setPosition((WINDOW_WIDTH - 500) / 2, WINDOW_HEIGHT / 2 + 150);
    
    sf::RectangleShape buttonRect(sf::Vector2f(500.f, 70.f));
    buttonRect.setPosition((WINDOW_WIDTH - 500) / 2, WINDOW_HEIGHT / 2 + 150);
    buttonRect.setFillColor(background_color);
    
    sf::Text playAgainText;
    playAgainText.setFont(regularFont);
    playAgainText.setString("PLAY AGAIN");
    playAgainText.setCharacterSize(50);
    sf::FloatRect textBounds = playAgainText.getLocalBounds();
    playAgainText.setFillColor(background_color == sf::Color::Black ? sf::Color::White : sf::Color::Black);
    playAgainText.setPosition((WINDOW_WIDTH - textBounds.width) / 2, WINDOW_HEIGHT / 2 + 150);
    
    while (window.isOpen()) {
        sf::Vector2i mousePosHover = sf::Mouse::getPosition(window);
        
        if (buttonRect.getGlobalBounds().contains(mousePosHover.x, mousePosHover.y)) {
            buttonRect.setFillColor(sf::Color(200, 200, 200));
        } else {
            buttonRect.setFillColor(background_color);
        }
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
                return;
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                
                if (buttonRect.getGlobalBounds().contains(mousePos)) {
                    resetGame();
                    welcome_screen();
                    return;
                }
            }
        }
        
        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(loseText);
        window.draw(scoreText);
        window.draw(highScoreText);
        window.draw(buttonRect);
        window.draw(playAgainButton);
        window.draw(playAgainText);
        window.display();
    }
}