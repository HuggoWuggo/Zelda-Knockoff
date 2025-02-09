#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

#define DEBUG

#ifdef DEBUG
sf::Time dt(sf::Clock clock);

class TileMap;

struct Tile {
	int index;
	int x;
	int y;
	sf::RectangleShape shape;
	bool collidable;
	char type;
};

struct Maps {
	int x;
	int y;
	TileMap *map;
};

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	TileMap(sf::RenderWindow *win);

	bool load(const std::filesystem::path& tileset, sf::Vector2u tileSize, int* tiles, char* tiles_c, unsigned int width, unsigned int height);
	int* processFileAndReturnInts(const std::string& filename, size_t& size, Maps *maps);
	char* processFileAndReturnChars(const std::string& filename, size_t& size, Maps *maps);

	std::vector<Tile> cells;
private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	int tile_v[110];
	char tile_c_v[110];

	sf::RectangleShape cell;
	sf::RenderWindow *window;
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};

class AnimatableSprite : public sf::Sprite
{
public:
	struct Animation {
		std::vector<sf::IntRect> frames; // List of frames for this animation
		float frameRate;                 // Frames per second
		sf::Time elapsedTime;            // Time passed since the last frame change
		int currentFrame = 0;            // Current frame in the animation
		int frameCounter = 0;            // Frame counter to control frame updates
	};

	AnimatableSprite(const sf::Texture& texture, int frameWidth, int frameHeight);

	void addAnimation(const std::string& name, const std::vector<sf::IntRect>& frames, float frameRate);
	void switchAnimation(const std::string& name);
	void update(sf::Time deltaTime);
private:
	int m_frameWidth, m_frameHeight;
	std::unordered_map<std::string, Animation> m_animations; // Store animations by name
	std::string m_currentAnimation; // Currently active animation
};
#endif