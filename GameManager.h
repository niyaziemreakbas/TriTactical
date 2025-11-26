#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Owner.h"
#include "UIManager.h"
#include "CombatManager.h"
#include "Map.h"
#include "GameConfig.h"

class GameManager
{
public:
    GameManager(unsigned int windowWidth, unsigned int windowHeight, UIManager& uiMgr);
    void draw(sf::RenderWindow& window);
    void handleClick(int mouseX, int mouseY);

    void update(float dt);
    void executeMove(Soldier* soldier, const sf::Vector2i& targetCell);

    void endTurn();

    bool checkForCombat(Soldier* movedSoldier);
    Soldier* getSoldierAt(sf::Vector2i position) const;

    const std::vector<std::unique_ptr<Owner>>& getOwners() const { return owners; }

    const Map& getMap() const { return m_map; }
    sf::Vector2i getRandomMapCell() const;
    sf::Vector2i findRandomEmptyCell();
    void startGame(GameMode mode, int numHumans, int numBots);

    bool isGameOver() const { return m_isGameOver; }
    std::string getWinnerName() const { return m_winnerName; }

    void onWindowResize(unsigned int w, unsigned int h);

private: 
    bool m_isGameOver = false;
    std::string m_winnerName = "";
    void checkWinCondition();

    void calculateMoveableCells(Soldier* soldier);

    CombatManager combatManager;

    void processAITurn();

    Map m_map; 
    UIManager& uiManager;

    std::vector<std::unique_ptr<Owner>> owners;

    Soldier* selectedSoldier = nullptr;
    int currentPlayerIndex = 0;

    enum class GameState { PLAYER_INPUT, ANIMATING, AI_THINKING };
    GameState currentGameState = GameState::PLAYER_INPUT;

    std::vector<sf::Vector2i> moveableCells;
    std::vector<sf::Vector2i> attackableCells;

    sf::Texture texTriangle;
    sf::Texture texCircle;
    sf::Texture texSquare;
    sf::Shader outlineShader;
};