#pragma once
#include <SFML/Graphics.hpp>
#include "GameManager.h"

enum class AppState {
    MainMenu,
    GameSetup,
    Gameplay
};

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void initUI();
    void handleMenuInput(int x, int y);
    void handleSetupInput(int x, int y);

    AppState currentState = AppState::MainMenu;
    GameMode selectedMode;

    sf::RenderWindow window;
    GameManager gameManager;

    sf::Clock clock;
};