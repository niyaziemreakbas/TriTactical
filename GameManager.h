#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "GridCell.h" 
#include <memory>
#include "UIManager.h"
#include "Owner.h"

class GameManager
{
public:
    GameManager(unsigned int windowWidth, unsigned int windowHeight);

    void startGame();
    void createInitialUnits();

    void handleClick(int mouseX, int mouseY);

    // handle drawing the map.
    void draw(sf::RenderWindow& window);

    void endTurn();

    Soldier* getSoldierAt(sf::Vector2i position) const;

    float totalMapWidth;
    float totalMapHeight;

private:
    // Add a function to create the map.
    void generateMap(unsigned int windowWidth, unsigned int windowHeight);

    void calculateMoveableCells(Soldier* soldier);

    void checkForCombat(Soldier* movedSoldier);

    void resolveCombat(Soldier& attacker, Soldier& defender);
    
    std::vector<sf::Vector2i> attackableCells;
    std::vector<sf::Vector2i> moveableCells;

    //Using for centering, soldier class needs
    float mapOffsetX = 0.f; 
    float mapOffsetY = 0.f;

    int mapWidth, mapHeight;
    float tileSize = 50.0f;

    std::vector<std::unique_ptr<Owner>> owners;

    std::vector<std::vector<GridCell>> map;

    Soldier* selectedSoldier = nullptr;

    UIManager uiManager;

    int currentPlayerIndex = 0;
};