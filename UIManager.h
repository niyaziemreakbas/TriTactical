#pragma once
#include <SFML/Graphics.hpp>
#include "Soldier.h"
#include "GameConfig.h" 

class UIManager
{
public:
    UIManager();

    void drawMainMenu(sf::RenderWindow& window);
    void drawSetupMenu(sf::RenderWindow& window, GameMode selectedMode, int humanCount, int aiCount);

    MenuAction handleMenuClick(int x, int y);
    MenuAction handleSetupClick(int x, int y);

    //In Game UI functions
    void updateGameUI(Soldier* selectedSoldier, Owner* currentOwner);
    void drawGameUI(sf::RenderWindow& window);

    bool isEndTurnButtonClicked(sf::Vector2i mousePos);

    void LoadFont(sf::Font& font, std::string str);

    void setEndTurnButtonActive(bool isActive);

    void drawGameOverScreen(sf::RenderWindow& window, std::string winnerName);
    MenuAction handleGameOverClick(int x, int y);

    void onResize(unsigned int width, unsigned int height);

    void resizeMainMenu(float width, float height);
    void resizeSetupMenu(float width, float height);
    void resizeGameUI(float width, float height);
    void resizeGameOverUI(float width, float height);

    void setupText(sf::Text& txt, int size, sf::Color color, std::string str = "");
    void setupSmallBtn(sf::RectangleShape& btn, sf::Text& txt, std::string label);

private:
    sf::Font font;

    // Main Menu UI
    sf::Text titleText;
    sf::RectangleShape btnPvP, btnPvAI, btnAIvAI;
    sf::Text txtPvP, txtPvAI, txtAIvAI;

    // Setup Menu UI
    sf::Text setupTitleText;
    sf::Text setupInfoText;
    sf::RectangleShape btnStartGame, btnBack;
    sf::Text txtStartGame, txtBack;
    sf::RectangleShape btnIncHuman, btnDecHuman;
    sf::RectangleShape btnIncAI, btnDecAI;
    sf::Text txtHumanCount, txtAICount;
    sf::Text labelHuman, labelAI;
    sf::Text txtIncHuman, txtDecHuman;
    sf::Text txtIncAI, txtDecAI;

    // In Game UI
    sf::Text ownerText;
    sf::Text typeText;
    sf::Text statsText;
    sf::RectangleShape infoPanel;
    sf::RectangleShape endTurnButton;
    sf::Text endTurnText;
    sf::Text turnIndicatorText;
    sf::RectangleShape ownerColorBackground;   // Beyaz Kare
    sf::CircleShape ownerColorCircle;   // Renkli Daire

    // --- Game Over UI ---
    sf::RectangleShape gameOverPanel;
    sf::Text txtGameOverTitle;
    sf::Text txtWinnerName;
    sf::RectangleShape btnReturnMain;
    sf::Text txtReturnMain;
    sf::RectangleShape btnRestart;
    sf::Text txtRestart;

    void initMenuUI();
    void initGameUI();
    void createBtn(sf::RectangleShape& btn, sf::Text& txt, std::string str, sf::Color color);
    void alignButton(sf::RectangleShape& btn, sf::Text& txt, float x, float y);
};