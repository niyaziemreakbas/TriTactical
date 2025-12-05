#include "Pathfinder.h"
#include "GameManager.h" // GameManager'ýn özelliklerini burada kullanacaðýz
#include <algorithm> // std::reverse için

std::vector<sf::Vector2i> Pathfinder::findPath(const sf::Vector2i& start, const sf::Vector2i& target, GameManager& gameManager)
{
    // Harita boyutlarýný al
    sf::Vector2i mapDims = gameManager.getMap().getDimensions();
    int width = mapDims.x;
    int height = mapDims.y;

    std::queue<sf::Vector2i> frontier;
    frontier.push(start);

    
    std::vector<std::vector<sf::Vector2i>> cameFrom(width, std::vector<sf::Vector2i>(height, { -1, -1 }));

    // Baþlangýç noktasýný iþaretle
    cameFrom[start.x][start.y] = start;

    bool found = false;

    while (!frontier.empty())
    {
        sf::Vector2i current = frontier.front();
        frontier.pop();

        // Hedefe ulaþtýk mý?
        if (current == target)
        {
            found = true;
            break;
        }

        sf::Vector2i directions[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        for (const auto& dir : directions)
        {
            sf::Vector2i next = current + dir;

            // 1. Harita sýnýrlarý içinde mi?
            if (!isValid(next, width, height)) continue;

            // 2. Daha önce ziyaret edildi mi? (cameFrom deðeri -1,-1 deðilse edilmiþtir)
            if (cameFrom[next.x][next.y] != sf::Vector2i(-1, -1)) continue;

            // 3. Engel kontrolü:
            bool isWalkable = (gameManager.getSoldierAt(next) == nullptr);
            bool isTarget = (next == target);

            if (isWalkable || isTarget)
            {
                frontier.push(next);
                cameFrom[next.x][next.y] = current; // "Next'e Current'tan geldim"
            }
        }
    }

    // --- (Backtracking) ---
    std::vector<sf::Vector2i> path;

    if (!found)
    {
        return path;
    }

    sf::Vector2i current = target;
    while (current != start)
    {
        path.push_back(current);
        current = cameFrom[current.x][current.y];
    }

    std::reverse(path.begin(), path.end());

    return path;
}

bool Pathfinder::isValid(const sf::Vector2i& pos, int width, int height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}