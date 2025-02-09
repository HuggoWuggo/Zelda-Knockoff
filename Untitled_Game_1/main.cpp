//#include <Windows.h>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "useful.hpp"
#include "player.hpp"

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main()
{
    // load shit
    sf::RenderWindow window(sf::VideoMode({ 640, 628 }), "Zelda_Clone", sf::Style::Titlebar | sf::Style::Close);

    Player player;

    TileMap map(&window);

    Maps maps = {
        1,
        2,
        &map,
    };

    sf::Clock clock;
    window.setFramerateLimit(60);  // Limit FPS to 30, making the movement less smooth

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // parse events
            if (event->is<sf::Event::Closed>())
                window.close();
            
        }

        // update

        player.update(dt(clock), maps.map->cells, &maps);
        
        window.clear();
        // render
        window.draw(*maps.map);
		player.render(&window);

        window.display();
    }
}