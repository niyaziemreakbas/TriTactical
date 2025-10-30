#pragma once
#include <SFML/Graphics.hpp>
#include "Soldier.h" // Soldier pointer'ý alacaðýmýz için include ediyoruz.

class UIManager
{
public:
    UIManager(); // Kurucuda font'u yükleyip text'leri ayarlayacaðýz.

    // Seçili asker deðiþtiðinde bu fonksiyonu çaðýracaðýz.
    void update(Soldier* selectedSoldier, const std::string& currentPlayerName);

    // Her karede UI'ý çizmek için bu fonksiyonu çaðýracaðýz.
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