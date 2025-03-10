#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdint>
#include <optional>

#include "useful.hpp"
#include "bar.hpp"

#define DEBUG
#ifdef DEBUG

class Player {
public:
	sf::RectangleShape m_player;
	AnimatableSprite s_player;
	Bar bar;

	void render(sf::RenderWindow *window);
	void update(sf::Time dt, std::vector<Tile> tiles, Maps* maps);

	Player();
private:
	sf::Texture t_player;
	sf::RectangleShape fadeRect;
	sf::Clock fadeClock;

	int fall_count = 0;

	float alpha = 0;

	bool can_switch = true;
	bool reached = false;
	bool canMove = true;
	bool start;
	bool falling = false;
	bool isJumping = false;

	// Store the current animation state
	std::string currentAnimation; // Default animation

	void move(float speed, std::vector<Tile> tiles, Maps* maps);
	void move_tiles(Maps *maps);
	void fade_clock();
	void isFalling();
	void pickup();
};

#endif // DEBUG
