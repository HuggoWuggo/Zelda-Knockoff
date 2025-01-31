#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdint>

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
	void update(sf::Time dt, std::vector<Tile> tiles, Maps *maps);

	Player();
private:
	sf::Texture t_player;
	sf::RectangleShape fadeRect;
	sf::Clock fadeClock;

	bool can_switch = true;
	float alpha = 0;
	bool reached = false;
	bool canMove = true;
	bool start;

	void move(float speed, std::vector<Tile> tiles);
	void move_tiles(Maps *maps);
	void fade_clock();
};

#endif // DEBUG
