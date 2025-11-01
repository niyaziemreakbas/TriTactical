#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Owner.h"
#include "UIManager.h"
#include "Map.h" // Yeni harita sýnýfýmýzý dahil et

class GameManager
{
public:
    GameManager(unsigned int windowWidth, unsigned int windowHeight);
    void draw(sf::RenderWindow& window);
    void handleClick(int mouseX, int mouseY);

    void update(float dt);
    void executeMove(Soldier* soldier, const sf::Vector2i& targetCell);

    void endTurn();
	bool IsPlayerTurn() const { return isPlayerTurn; }
	bool SetPlayerTurn(bool turn) { return isPlayerTurn = turn; }

    bool checkForCombat(Soldier* movedSoldier);
    Soldier* getSoldierAt(sf::Vector2i position) const;

    const std::vector<std::unique_ptr<Owner>>& getOwners() const { return owners; }

    const Map& getMap() const { return m_map; }
    sf::Vector2i getRandomMapCell() const;

private:
    void createInitialUnits();

    // War Functions can be separated to another script
    void calculateMoveableCells(Soldier* soldier);
    bool resolveCombat(Soldier& attacker, Soldier& defender);

    void processAITurn();

    Map m_map; 
    UIManager uiManager;
    std::vector<std::unique_ptr<Owner>> owners;

    Soldier* selectedSoldier = nullptr;
    int currentPlayerIndex = 0;

    enum class GameState { PLAYER_INPUT, ANIMATING, AI_THINKING };
    GameState currentGameState = GameState::PLAYER_INPUT;

	// Can be use in Game State but its okay for now
    bool isPlayerTurn;

    std::vector<sf::Vector2i> moveableCells;
    std::vector<sf::Vector2i> attackableCells;
};