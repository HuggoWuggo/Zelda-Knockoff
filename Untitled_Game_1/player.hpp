#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <array>
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
	bool isPickingUp = false;

	// Store the current animation state
	std::string currentAnimation; // Default animation
	std::vector<std::array<int, 2>> changed;

	void move(float speed, std::vector<Tile> tiles, Maps* maps);
	void move_tiles(Maps *maps);
	void fade_clock();
	void isFalling();
	void modify(Maps* maps, int index, int n_tile);
};

#endif // DEBUG
