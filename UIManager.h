#pragma once
#include <SFML/Graphics.hpp>
#include "Soldier.h" // Soldier pointer'� alaca��m�z i�in include ediyoruz.

class UIManager
{
public:
    UIManager(); // Kurucuda font'u y�kleyip text'leri ayarlayaca��z.

    // Se�ili asker de�i�ti�inde bu fonksiyonu �a��raca��z.
    void update(Soldier* selectedSoldier, const std::string& currentPlayerName);

    // Her karede UI'� �izmek i�in bu fonksiyonu �a��raca��z.
    void draw(sf::RenderWindow& window);

    bool isEndTurnButtonClicked(sf::Vector2i mousePos);

	void LoadFont(sf::Font& font, std::string str);

private:
    sf::Font font;
    sf::Text ownerText;
    sf::Text typeText;
    sf::Text statsText;
    sf::RectangleShape infoPanel;
    sf::RectangleShape endTurnButton;
    sf::Text endTurnText;
    sf::Text turnIndicatorText;
};