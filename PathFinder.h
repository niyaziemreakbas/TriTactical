#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <queue>
#include <map>

class GameManager;
class Soldier;
class Pathfinder
{
public:
    static std::vector<sf::Vector2i> findPath(const Soldier& me, const sf::Vector2i& target, GameManager& gameManager);

private:
    static bool isValid(const sf::Vector2i& pos, int width, int height);

    static bool isPositionSafe(const sf::Vector2i& pos, const Soldier& me, GameManager& gameManager);
};