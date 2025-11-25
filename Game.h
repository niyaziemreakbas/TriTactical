#pragma once
#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include "GameConfig.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    AppState currentState = AppState::MainMenu;
    GameMode selectedMode = GameMode::PvP;
    int numHumans = 0;
    int numAI = 0;

    sf::RenderWindow window;

	UIManager uiManager;
    GameManager gameManager;

    sf::Clock clock;
};