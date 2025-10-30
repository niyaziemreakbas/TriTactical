#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

Game::Game()
    : window(sf::VideoMode({ 1366, 768 }), "SFML Strategy Game"),
    gameManager(window.getSize().x, window.getSize().y)
{
    // Kurucu g�vdesi art�k bo�.
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
        {
            // Art�k 'mouseButton' pointer'� �zerinden veriye g�venle eri�ebiliriz.
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                gameManager.handleClick(mouseButton->position.x, mouseButton->position.y);
            }
        }
    }
}

void Game::update()
{
    // �imdilik bo�
}

void Game::render()
{
    window.clear(sf::Color(150, 50, 150));

	gameManager.draw(window);

    window.display();
}