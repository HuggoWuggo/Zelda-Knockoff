#include "player.hpp"
#include <cmath>  // For sqrt()

Player::Player() : t_player(), s_player(t_player, 32, 32)
{
    if (!t_player.loadFromFile("res/link_tileset.png")) {
		std::cerr << "Error: Could not load player texture." << std::endl;
		exit(1);
    }

    std::vector <sf::IntRect> idleFrames_D = {
        {{64, 0}, {32, 32}},
	};

    std::vector <sf::IntRect> idleFrames_U = {
        {{64, 32}, {32, 32}},
    };

    std::vector <sf::IntRect> idleFrames_L = {
        {{64, 64}, {32, 32}},
    };

    std::vector <sf::IntRect> idleFrames_R = {
        {{64, 96}, {32, 32}},
    };

    std::vector <sf::IntRect> walkFramesD = {
        {{0, 0}, {32, 32}},
        {{32, 0}, {32, 32}},
    };

	std::vector <sf::IntRect> walkFramesU = {
		{{0, 32}, {32, 32}},
		{{32, 32}, {32, 32}},
	};

	std::vector <sf::IntRect> walkFramesL = {
		{{0, 64}, {32, 32}},
		{{32, 64}, {32, 32}},
	};

	std::vector <sf::IntRect> walkFramesR = {
		{{0, 96}, {32, 32}},
		{{32, 96}, {32, 32}},
	};

    std::vector <sf::IntRect> fall_frames = {
        {{0, 128}, {32, 32}},
        {{32, 128}, {32, 32}},
        {{64, 128}, {32, 32}},
    };

    std::vector <sf::IntRect> jump_framesD = {
        {{0, 224}, {32, 32}},
        {{32, 224}, {32, 32}},
        {{64, 224}, {32, 32}},
    };

    std::vector <sf::IntRect> pickup_framesD = {
        {{0, 256}, {32, 32}},
    };

    std::vector <sf::IntRect> pickup_framesU = {
        {{32, 256}, {32, 32}},
    };

    std::vector <sf::IntRect> pickup_framesL = {
        {{64, 256}, {32, 32}},
    };

    std::vector <sf::IntRect> pickup_framesR = {
        {{96, 256}, {32, 32}},
    };

	s_player.addAnimation("idle_d", idleFrames_D, 100.0f);
	s_player.addAnimation("idle_u", idleFrames_U, 100.0f);
	s_player.addAnimation("idle_l", idleFrames_L, 150.0f);
	s_player.addAnimation("idle_r", idleFrames_R, 150.0f);
	s_player.addAnimation("walk_down", walkFramesD, 10.0f);
	s_player.addAnimation("walk_up", walkFramesU, 10.0f);
	s_player.addAnimation("walk_left", walkFramesL, 8.0f);
	s_player.addAnimation("walk_right", walkFramesR, 8.0f);
	s_player.addAnimation("fall", fall_frames, 10.0f);
	s_player.addAnimation("jump_d", jump_framesD, 10.0f);
    s_player.addAnimation("pickup_d", pickup_framesD, 10.0f);
    s_player.addAnimation("pickup_u", pickup_framesU, 10.0f);
    s_player.addAnimation("pickup_l", pickup_framesL, 10.0f);
    s_player.addAnimation("pickup_r", pickup_framesR, 10.0f);

	s_player.switchAnimation("idle_d");
	s_player.setScale(sf::Vector2f(2.0f, 2.0f));
    s_player.setPosition({ 480, 64 });

    m_player.setSize(sf::Vector2f(25, 50));
	m_player.setFillColor(sf::Color::Transparent);
	m_player.setOutlineThickness(1.5f);
	m_player.setOutlineColor(sf::Color::Blue);

    fadeRect.setSize(sf::Vector2f(640, 628));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    alpha = 255;

	start = true;
}

void Player::move(float speed, std::vector<Tile> tiles, Maps* maps)
{
    // Create a vector to accumulate direction inputs
    sf::Vector2f direction(0.f, 0.f);

    if (canMove == false)
        direction = { 0.f, 0.f };

    // Check each direction (WASD or arrow keys)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        direction.y -= 1.f; // Move up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        direction.y += 1.f; // Move down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        direction.x -= 1.f; // Move left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        direction.x += 1.f; // Move right

    // Normalize the direction vector to avoid diagonal speed boost
    if (direction.x != 0.f || direction.y != 0.f) {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;  // Normalize the vector
    }

    // Handle animation changes based on movement
    if (canMove == true)
    {
        if (direction.x > 0) {
            if (currentAnimation != "walk_right") {
                s_player.switchAnimation("walk_right");
                currentAnimation = "walk_right";
            }
        }
        else if (direction.x < 0) {
            if (currentAnimation != "walk_left") {
                s_player.switchAnimation("walk_left");
                currentAnimation = "walk_left";
            }
        }
        else if (direction.y > 0) {
            if (currentAnimation != "walk_down") {
                s_player.switchAnimation("walk_down");
                currentAnimation = "walk_down";
            }
        }
        else if (direction.y < 0) {
            if (currentAnimation != "walk_up") {
                s_player.switchAnimation("walk_up");
                currentAnimation = "walk_up";
            }
        }
        else {
            if (currentAnimation != "idle") {
                if (currentAnimation == "walk_down")
                    s_player.switchAnimation("idle_d");
                else if (currentAnimation == "walk_up")
                    s_player.switchAnimation("idle_u");
                else if (currentAnimation == "walk_left")
                    s_player.switchAnimation("idle_l");
                else if (currentAnimation == "walk_right")
                    s_player.switchAnimation("idle_r");
                currentAnimation = "idle";
            }
        }
    }

    // Now, check for collisions in the intended direction
    for (auto& tile : tiles) {
        sf::FloatRect nextPosition = m_player.getGlobalBounds();

        if (tile.collidable && tile.type_c != '_') {
            // Calculate the player's next position based on the movement direction
            if (direction.x > 0)
                nextPosition.position.x += speed;  // Moving right
            if (direction.x < 0)
                nextPosition.position.x -= speed;  // Moving left
            if (direction.y > 0)
                nextPosition.position.y += speed;   // Moving down
            if (direction.y < 0)
                nextPosition.position.y -= speed;   // Moving up

            // Check for collisions in the intended direction
            if (nextPosition.findIntersection(tile.shape.getGlobalBounds())) {
                // Block movement in the direction of the collision
                if (tile.collidable && tile.type_c == 'f') {
					falling = true;
                }
                else if (tile.collidable && tile.type_c == 'p' && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q)) {
                    canMove = false;

                    if (currentAnimation == "walk_down") {
                        s_player.switchAnimation("pickup_d");
                    }
                    else if (currentAnimation == "walk_up") {
                        s_player.switchAnimation("pickup_u");
                    }
                    else if (currentAnimation == "walk_left") {
                        s_player.switchAnimation("pickup_l");
                    }
                    else if (currentAnimation == "walk_right") {
                        s_player.switchAnimation("pickup_r");
                    }

                    modify(maps, tile.index, 52);
                }
                else {
                    if (direction.x > 0)
                        direction.x = 0.f;  // Block right movement
                    if (direction.x < 0)
                        direction.x = 0.f;  // Block left movement
                    if (direction.y > 0)
                        direction.y = 0.f;  // Block down movement
                    if (direction.y < 0)
                        direction.y = 0.f;  // Block up movement
                }
            }
        }
    }

    // Move the player (if direction is not zero)
    if (direction != sf::Vector2f(0.f, 0.f) && canMove == true) {
        s_player.move(direction * speed);
    }
}

void Player::move_tiles(Maps* maps) {
    size_t size = 0;
    size_t size_c = 0;

    int x_p = maps->x;
    int y_p = maps->y;

    if (s_player.getPosition().y <= -40.0f) {
        fadeClock.restart();

        if (alpha == 255)
        {
            s_player.setPosition({ s_player.getPosition().x, s_player.getPosition().y + 490 });
            maps->y -= 1;
        }
    }
    else if (s_player.getPosition().y >= 480.0f) {
        fadeClock.restart();

        if (alpha == 255)
        {
            s_player.setPosition({ s_player.getPosition().x, s_player.getPosition().y - 500 });
            maps->y += 1;
        }
	}
	else if (s_player.getPosition().x <= -60.0f) {
		fadeClock.restart();
		if (alpha == 255)
		{
			s_player.setPosition({ s_player.getPosition().x + 640, s_player.getPosition().y });
			maps->x -= 1;
		}
	}
	else if (s_player.getPosition().x >= 650.0f) {
		fadeClock.restart();
		if (alpha == 255)
		{
			s_player.setPosition({ s_player.getPosition().x - 640, s_player.getPosition().y });
			maps->x += 1;
		}
	}

    // We assume maps->map has methods for loading data
    // If the `processFileAndReturn*` methods use dynamic memory, ensure cleanup happens
    char* level_c = nullptr;
    int* level = nullptr;

    if ((x_p != maps->x || y_p != maps->y) || start == true)
    {
        std::string file_name = "Maps/row_" + std::to_string(maps->x) + ".txt";

        // Make sure previously loaded data is cleaned up to avoid overlap
        level = maps->map->processFileAndReturnInts(file_name, size, maps);
        level_c = maps->map->processFileAndReturnChars(file_name, size_c, maps);

        // Only reload the map if necessary
        if (!(maps->map->load("res/overworld.png", { 64, 64 }, level, level_c, 10, 11))) {
            std::cerr << "Error: Could not load tilemap." << std::endl;
        }
        start = false;
        changed.clear();
    }

    // Clean up after processing the new level
    delete[] level;
    delete[] level_c;
}

void Player::fade_clock()
{
    if (fadeClock.getElapsedTime().asSeconds() < 1.5f) {
        if (alpha < 255)
            alpha += 5.f;
        canMove = false;
    }
    else if (fadeClock.getElapsedTime().asSeconds() < 3 && fadeClock.getElapsedTime().asSeconds() > 1.5f) {
        if (alpha > 0)
            alpha -= 5.f;
        canMove = true;
    }
    else {
        fadeClock.stop();
        canMove = true;
    }
}

void Player::isFalling()
{
    if (falling == true && fall_count < 142) {
        canMove = false;
        fall_count += 5;
        s_player.move({ 0, 5 });

        if (isJumping == false) {
            s_player.switchAnimation("jump_d");
            isJumping = true;
        }
    }
    else if (fall_count >= 142) {
        canMove = true;
        falling = false;
		isJumping = false;
        fall_count = 0;
		s_player.switchAnimation("idle_d");
    }
}

void Player::render(sf::RenderWindow *window)
{
    window->draw(s_player);
	//window->draw(m_player);
    window->draw(bar.s_bar);
    window->draw(fadeRect);
}

void Player::update(sf::Time dt, std::vector<Tile> tiles, Maps *maps)
{
    m_player.setPosition({ s_player.getPosition().x + 20, s_player.getPosition().y + 10 });

    fade_clock();
    fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(alpha)));
	
    isFalling();

    move_tiles(maps);
	move(4.0f, tiles, maps);
	
    s_player.update(dt);
}

void Player::modify(Maps* maps, int index, int n_tile)
{
    size_t size = 0;
    size_t size_c = 0;
    std::string file_name = "Maps/row_" + std::to_string(maps->x) + ".txt";

    changed.push_back({index, n_tile});

    int* level = maps->map->processFileAndReturnInts(file_name, size, maps);
    char* level_c = maps->map->processFileAndReturnChars(file_name, size_c, maps);

    for (auto index : changed) {
        level[index[0]] = index[1];
        level_c[index[0]] = '_';
    }

    maps->map->load("res/overworld.png", { 64, 64 }, level, level_c, 10, 11);
}
