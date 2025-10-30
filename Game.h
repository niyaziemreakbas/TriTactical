#pragma once
#include <SFML/Graphics.hpp>
#include "GameManager.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
    GameManager gameManager;
};