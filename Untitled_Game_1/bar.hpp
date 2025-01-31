#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#define DEBUG
#ifdef DEBUG

class Bar
{
public:
	Bar();
	sf::RectangleShape s_bar;
private:
	sf::Texture t_bar;
};

#endif