#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

Game::Game()
    : window(sf::VideoMode({ 1366, 768 }), "SFML Strategy Game"),
    gameManager(window.getSize().x, window.getSize().y)
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
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
        {
            // Artýk 'mouseButton' pointer'ý üzerinden veriye güvenle eriþebiliriz.
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                gameManager.handleClick(mouseButton->position.x, mouseButton->position.y);
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

	gameManager.draw(window);

    window.display();
}