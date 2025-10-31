#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Owner.h"
#include "UIManager.h"
#include "Map.h" // Yeni harita s�n�f�m�z� dahil et

class GameManager
{
public:
    GameManager(unsigned int windowWidth, unsigned int windowHeight);
    void draw(sf::RenderWindow& window);
    void handleClick(int mouseX, int mouseY);
    void endTurn();

private:
    // Harita ile ilgili t�m fonksiyonlar ve de�i�kenler kald�r�ld�.
    void createInitialUnits();

    // Sava� ve hareket mant��� �imdilik burada kalabilir.
    void calculateMoveableCells(Soldier* soldier);
    Soldier* getSoldierAt(sf::Vector2i position) const;
    bool checkForCombat(Soldier* movedSoldier);
    bool resolveCombat(Soldier& attacker, Soldier& defender);

    // �ye de�i�kenler
    Map m_map; // Art�k bir Map nesnesine sahibiz.
    UIManager uiManager;
    std::vector<std::unique_ptr<Owner>> owners;

    Soldier* selectedSoldier = nullptr;
    int currentPlayerIndex = 0;

    std::vector<sf::Vector2i> moveableCells;
    std::vector<sf::Vector2i> attackableCells;
};