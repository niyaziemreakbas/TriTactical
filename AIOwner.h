#pragma once
#include "Owner.h"
#include "GameManager.h"

//class Map;
//class GameManager;

// Subclass for the AI.
class AIOwner : public Owner
{
public:
    AIOwner(std::string name, sf::Color color);
    void processTurn(GameManager& gameManager);

private:
    Soldier* findClosestSoldier(Soldier& from, const std::vector<Soldier*>& targets);
    sf::Vector2i calculateMoveTowards(const Soldier& from, const sf::Vector2i& targetPos);
    sf::Vector2i calculateMoveAway(const Soldier& from, const sf::Vector2i& threatPos);
};
