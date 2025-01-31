#include "bar.hpp"

Bar::Bar()
{
	s_bar = sf::RectangleShape(sf::Vector2f(640, 128));

	if (!t_bar.loadFromFile("res/tilebar.png"))
		std::cerr << "Error: Could not load bar texture." << std::endl;

	//s_bar.setFillColor(sf::Color::White);
	s_bar.setPosition({ 0, 500 });
	s_bar.setTexture(&t_bar);
}
