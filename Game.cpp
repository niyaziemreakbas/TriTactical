#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "GameManager.h"

Game::Game()
    : window(sf::VideoMode({ 1366, 768 }), "SFML Strategy Game"),
    gameManager(window.getSize().x, window.getSize().y, uiManager)
{
}

void Game::run()
{
    clock.restart();
    while (window.isOpen())
    {
        processEvents();

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

        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            unsigned int w = resized->size.x;
            unsigned int h = resized->size.y;

            sf::FloatRect visibleArea({ 0.f, 0.f }, { static_cast<float>(w), static_cast<float>(h) });
            window.setView(sf::View(visibleArea));

            uiManager.onResize(w, h);
            gameManager.onWindowResize(w, h);
        }

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

                        // PvP rules
                        numHumans = 2;
                        numAI = 0;

                        currentState = AppState::GameSetup;
                    }
                    else if (action == MenuAction::SelectPvAI) {
                        selectedMode = GameMode::PvAI;

						// PvAI rules
                        numHumans = 1;
                        numAI = 1;

                        currentState = AppState::GameSetup;
                    }
                    else if (action == MenuAction::SelectAIvAI) {
                        selectedMode = GameMode::AIvAI;

						// AIvAI rules
						numHumans = 0;
						numAI = 2;

                        currentState = AppState::GameSetup;
                    }
                }
                else if (currentState == AppState::GameSetup)
                {
                    MenuAction action = uiManager.handleSetupClick(mx, my);

                    if (action == MenuAction::StartGame) {
                        gameManager.startGame(selectedMode, numHumans, numAI);
                        currentState = AppState::Gameplay;
                    }
                    else if (action == MenuAction::BackToMenu) {
                        currentState = AppState::MainMenu;
                    }
                    // Increase
                    else if (action == MenuAction::IncHuman && numHumans < 8) numHumans++;
                    else if (action == MenuAction::IncAI && numAI < 8) numAI++;

                    // Decrease
                    else if (action == MenuAction::DecHuman)
                    {
                        // PvP ise 2'nin altýna inemesin
                        if (selectedMode == GameMode::PvP && numHumans > 2) numHumans--;
                        // PvAI ise 1'in altýna inemesin
                        else if (selectedMode == GameMode::PvAI && numHumans > 1) numHumans--;
                        // Diðer durumlarda (AIvAI) 0'a kadar inebilir (gerçi AIvAI'da insan butonunu gizlesen daha þýk olur ama þimdilik kalsýn)
                        else if (selectedMode == GameMode::AIvAI && numHumans > 0) numHumans--;
                    }
                    else if (action == MenuAction::DecAI)
                    {
                        // AIvAI ise 2'nin altýna inemesin
                        if (selectedMode == GameMode::AIvAI && numAI > 2) numAI--;
                        // PvAI ise 1'in altýna inemesin
                        else if (selectedMode == GameMode::PvAI && numAI > 1) numAI--;
                        // PvP ise 0'a kadar inebilir
                        else if (selectedMode == GameMode::PvP && numAI > 0) numAI--;
                    }
                }
                else if (currentState == AppState::Gameplay)
                {
                    gameManager.handleClick(mx, my);
                }
                else if (currentState == AppState::GameOver)
                {
                    MenuAction action = uiManager.handleGameOverClick(mx, my);

                    if (action == MenuAction::ReturnToMain)
                    {
                        currentState = AppState::MainMenu;
                    }
                    else if (action == MenuAction::RestartGame)
                    {
                        std::cout << "Restarting Game...\n";

                        gameManager.startGame(selectedMode, numHumans, numAI);

                        currentState = AppState::Gameplay;
                    }
                }
            }
        }

		// Return to Main Menu with Escape key
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                if (currentState == AppState::Gameplay) {
                    currentState = AppState::MainMenu;
                }
                else if (currentState == AppState::GameSetup) {
                    currentState = AppState::MainMenu;
                }
            }
        }
    }
}

void Game::update(float dt)
{
    if (currentState == AppState::Gameplay)
    {
        gameManager.update(dt);

        if (gameManager.isGameOver())
        {
            currentState = AppState::GameOver;
        }
    }
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
        uiManager.drawSetupMenu(window, selectedMode, numHumans, numAI);
    }
    else if (currentState == AppState::Gameplay)
    {
        window.clear(sf::Color(150, 50, 150)); 

        gameManager.draw(window);
    }
    else if (currentState == AppState::GameOver)
    {
        window.clear(sf::Color(150, 50, 150));
        gameManager.draw(window);

        uiManager.drawGameOverScreen(window, gameManager.getWinnerName());
    }

    window.display();
}
