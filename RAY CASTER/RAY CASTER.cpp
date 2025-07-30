// RayCaster.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include "Player.h"
#define PI 3.1415926535

// Constants for window dimensions
const int WINDOW_HEIGHT = 512;
const int WINDOW_WIDTH = 1024;
const int NUMBER_OF_RAYS_IN_ONE_DEGREE = 10;


// mapS size of each tile
int mapX = 8, mapY = 8, mapS = 64;

// map 
int mapGrid[] = {
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,1,1,1,0,0,1,
    1,0,0,0,1,0,0,1,
    1,1,1,1,1,1,1,1,
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Ray Caster");
    
    sf::Font font;
    if (!font.openFromFile("arial.ttf")) std::cout << "Error loading font\n";

    sf::Text fpsText(font);
    fpsText.setCharacterSize(25);
    fpsText.setFillColor(sf::Color::Blue);
    fpsText.setPosition(sf::Vector2f(10.f, 10.f));

    // creating player object
    Player player(300, 300,mapS,mapX,mapY);

    // converting 1,0 mapgrid into a map with shape for the display;
    std::vector<std::vector<sf::RectangleShape>> map(mapY, std::vector<sf::RectangleShape>(mapX));
    for (int y = 0; y < mapY; y++) {
        for (int x = 0; x < mapX; x++) {
            int xo = x * mapS, yo = y * mapS;
            sf::RectangleShape rect(sf::Vector2f(mapS - 1, mapS - 1));
            rect.setPosition(sf::Vector2f(xo, yo));
            if (mapGrid[y * mapX + x] == 1) {
                rect.setFillColor(sf::Color::White);
            }
            else {
                rect.setFillColor(sf::Color::Black);
            }
            map[y][x] = rect;
        }
    }

    sf::Clock deltaclock;

    float fpsTimer = 0.f;
    int frameCount = 0;
    float fps = 0.f;
    
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        float dt = deltaclock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            player.moveForwards(dt,mapGrid);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            player.moveBackwards(dt,mapGrid);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            player.rotateLeft(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            player.rotateRight(dt);
        }
        // updating the player position
        player.update();

        // clearing the screen
        window.clear(sf::Color(128, 128, 128));

        // displaying the map
        for (int y = 0; y < mapY; y++) {
            for (int x = 0; x < mapX; x++) {
                window.draw(map[y][x]);
            }
        }
        // calling the function draw in player class to draw the character in the map and 3-D view
        player.draw(window,mapGrid,NUMBER_OF_RAYS_IN_ONE_DEGREE,WINDOW_HEIGHT,WINDOW_WIDTH);

        float fpsValue = 1.f / deltaclock.restart().asSeconds();

        fpsText.setString("FPS: " + std::to_string(int(fpsValue)));

        window.draw(fpsText);

        // to display the content
        window.display();
    }
}
