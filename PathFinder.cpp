#include "Pathfinder.h"
#include "GameManager.h" // GameManager'ýn özelliklerini burada kullanacaðýz
#include <algorithm> // std::reverse için

std::vector<sf::Vector2i> Pathfinder::findPath(const Soldier& me, const sf::Vector2i& target, GameManager& gameManager)
{
    sf::Vector2i start = me.gridPosition;
    sf::Vector2i mapDims = gameManager.getMap().getDimensions();
    int width = mapDims.x;
    int height = mapDims.y;

    std::queue<sf::Vector2i> frontier;
    frontier.push(start);

    // -1, -1 indicates unvisited
    std::vector<std::vector<sf::Vector2i>> cameFrom(width, std::vector<sf::Vector2i>(height, { -1, -1 }));
    cameFrom[start.x][start.y] = start;

    bool found = false;

    while (!frontier.empty())
    {
        sf::Vector2i current = frontier.front();
        frontier.pop();

        if (current == target)
        {
            found = true;
            break;
        }

        sf::Vector2i directions[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        for (const auto& dir : directions)
        {
            sf::Vector2i next = current + dir;

            if (!isValid(next, width, height)) continue;
            if (cameFrom[next.x][next.y] != sf::Vector2i(-1, -1)) continue;

            bool isWalkable = (gameManager.getSoldierAt(next) == nullptr);
            bool isTarget = (next == target);

            // CRITICAL: Treat unsafe tiles as walls, unless it is the target itself
            bool isSafe = isPositionSafe(next, me, gameManager);

            if ((isWalkable || isTarget) && isSafe)
            {
                frontier.push(next);
                cameFrom[next.x][next.y] = current;
            }
        }
    }

    // Reconstruct path
    std::vector<sf::Vector2i> path;
    if (!found) return path;

    sf::Vector2i current = target;
    while (current != start)
    {
        path.push_back(current);
        current = cameFrom[current.x][current.y];
    }
    std::reverse(path.begin(), path.end());

    return path;
}
bool isCounter(Soldier::Type attacker, Soldier::Type victim)
{
    if (attacker == Soldier::Type::Triangle && victim == Soldier::Type::Circle) return true;
    if (attacker == Soldier::Type::Circle && victim == Soldier::Type::Square) return true;
    if (attacker == Soldier::Type::Square && victim == Soldier::Type::Triangle) return true;
    return false;
}

bool Pathfinder::isPositionSafe(const sf::Vector2i& pos, const Soldier& me, GameManager& gameManager)
{
    sf::Vector2i directions[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    for (const auto& dir : directions)
    {
        sf::Vector2i neighborPos = pos + dir;
        Soldier* neighborSoldier = gameManager.getSoldierAt(neighborPos);

        if (neighborSoldier != nullptr)
        {
            // If neighbor is Enemy AND is a Counter Type -> Unsafe
            if (neighborSoldier->owner != me.owner)
            {
                if (isCounter(neighborSoldier->type, me.type))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Pathfinder::isValid(const sf::Vector2i& pos, int width, int height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}