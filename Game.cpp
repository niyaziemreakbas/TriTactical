#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "GameManager.h"

Game::Game()
    : window(sf::VideoMode({ 1366, 768 }), "SFML Strategy Game"),
    gameManager(window.getSize().x, window.getSize().y, uiManager)
{
    // Kurucu gövdesi artýk boþ.
}

void Game::run()
{
    clock.restart(); // Döngü baþlamadan saati sýfýrla.
    while (window.isOpen())
    {
        processEvents();

        // Saati yeniden baþlat ve geçen süreyi saniye olarak al.
        float dt = clock.restart().asSeconds();
        update(dt);

        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) window.close();

        if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseBtn->button == sf::Mouse::Button::Left)
            {
                int mx = mouseBtn->position.x;
                int my = mouseBtn->position.y;

                if (currentState == AppState::MainMenu)
                {
                    // UI Manager'a sor: Neye týklandý?
                    MenuAction action = uiManager.handleMenuClick(mx, my);

                    if (action == MenuAction::SelectPvP) {
                        selectedMode = GameMode::PvP;
                        currentState = AppState::GameSetup;
                    }
                    else if (action == MenuAction::SelectPvAI) {
                        selectedMode = GameMode::PvAI;
                        currentState = AppState::GameSetup;
                    }
                    else if (action == MenuAction::SelectAIvAI) {
                        selectedMode = GameMode::AIvAI;
                        currentState = AppState::GameSetup;
                    }
                }
                else if (currentState == AppState::GameSetup)
                {
                    MenuAction action = uiManager.handleSetupClick(mx, my);

                    if (action == MenuAction::StartGame) {
                        gameManager.startGame(selectedMode);
                        currentState = AppState::Gameplay;
                    }
                    else if (action == MenuAction::BackToMenu) {
                        currentState = AppState::MainMenu;
                    }
                }
                else if (currentState == AppState::Gameplay)
                {
                    gameManager.handleClick(mx, my);
                }
            }
        }

        // ESC tuþu ile geri gelme mantýðý
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                if (currentState == AppState::Gameplay) {
                    currentState = AppState::MainMenu; // Oyundan çýkýp menüye dön
                }
                else if (currentState == AppState::GameSetup) {
                    currentState = AppState::MainMenu; // Setup'tan menüye dön
                }
            }
        }
    }
}

void Game::update(float dt)
{
    gameManager.update(dt);
}

void Game::render()
{
    window.clear(sf::Color(150, 50, 150));

    if (currentState == AppState::MainMenu)
    {
        uiManager.drawMainMenu(window);
    }
    else if (currentState == AppState::GameSetup)
    {
        uiManager.drawSetupMenu(window, selectedMode);
    }
    else if (currentState == AppState::Gameplay)
    {
        window.clear(sf::Color(150, 50, 150)); 

        gameManager.draw(window);
    }

    window.display();
}