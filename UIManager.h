#pragma once
#include <SFML/Graphics.hpp>
#include "Soldier.h"
#include "GameConfig.h" 

class UIManager
{
public:
    UIManager(); 

    void drawMainMenu(sf::RenderWindow& window);
    void drawSetupMenu(sf::RenderWindow& window, GameMode selectedMode);

    MenuAction handleMenuClick(int x, int y);
    MenuAction handleSetupClick(int x, int y);

    //In Game UI functions
    void updateGameUI(Soldier* selectedSoldier, const std::string& currentPlayerName);
    void drawGameUI(sf::RenderWindow& window);

    bool isEndTurnButtonClicked(sf::Vector2i mousePos);

	void LoadFont(sf::Font& font, std::string str);

    void setEndTurnButtonActive(bool isActive);

private:
    sf::Font font;

	// Main Menu Texts
    sf::Text titleText;
    sf::RectangleShape btnPvP, btnPvAI, btnAIvAI;
    sf::Text txtPvP, txtPvAI, txtAIvAI;

	// Setup Menu Texts
    sf::Text setupTitleText;
    sf::Text setupInfoText;
    sf::RectangleShape btnStartGame, btnBack;
    sf::Text txtStartGame, txtBack;

    // In Game Texts
    sf::Text ownerText;
    sf::Text typeText;
    sf::Text statsText;
    sf::RectangleShape infoPanel;
    sf::RectangleShape endTurnButton;
    sf::Text endTurnText;
    sf::Text turnIndicatorText;

    void initMenuUI();
    void initGameUI();
    void createBtn(sf::RectangleShape& btn, sf::Text& txt, std::string str, float y, sf::Color color);
};