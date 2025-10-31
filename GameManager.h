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
    void endTurn();

private:
    // Harita ile ilgili tüm fonksiyonlar ve deðiþkenler kaldýrýldý.
    void createInitialUnits();

    // Savaþ ve hareket mantýðý þimdilik burada kalabilir.
    void calculateMoveableCells(Soldier* soldier);
    Soldier* getSoldierAt(sf::Vector2i position) const;
    bool checkForCombat(Soldier* movedSoldier);
    bool resolveCombat(Soldier& attacker, Soldier& defender);

    // Üye deðiþkenler
    Map m_map; // Artýk bir Map nesnesine sahibiz.
    UIManager uiManager;
    std::vector<std::unique_ptr<Owner>> owners;

    Soldier* selectedSoldier = nullptr;
    int currentPlayerIndex = 0;

    std::vector<sf::Vector2i> moveableCells;
    std::vector<sf::Vector2i> attackableCells;
};