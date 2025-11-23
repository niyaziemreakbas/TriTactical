#include "Pathfinder.h"
#include "GameManager.h" // GameManager'ýn özelliklerini burada kullanacaðýz
#include <algorithm> // std::reverse için

std::vector<sf::Vector2i> Pathfinder::findPath(const sf::Vector2i& start, const sf::Vector2i& target, GameManager& gameManager)
{
    // Harita boyutlarýný al
    sf::Vector2i mapDims = gameManager.getMap().getDimensions();
    int width = mapDims.x;
    int height = mapDims.y;

    // BFS için gerekli yapýlar
    std::queue<sf::Vector2i> frontier; // Gideceðimiz kareler kuyruðu
    frontier.push(start);

    // Nereden geldik? (Yolu geri çizmek için)
    // cameFrom[x][y] = {x, y} (bir önceki kare)
    // Bu matrisi -1, -1 ile doldurarak "ziyaret edilmedi" olarak baþlatýyoruz.
    std::vector<std::vector<sf::Vector2i>> cameFrom(width, std::vector<sf::Vector2i>(height, { -1, -1 }));

    // Baþlangýç noktasýný iþaretle
    cameFrom[start.x][start.y] = start;

    bool found = false;

    // --- BFS ALGORÝTMASI ---
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

        // 4 Yönlü Komþular (Sað, Sol, Aþaðý, Yukarý)
        sf::Vector2i directions[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        for (const auto& dir : directions)
        {
            sf::Vector2i next = current + dir;

            // 1. Harita sýnýrlarý içinde mi?
            if (!isValid(next, width, height)) continue;

            // 2. Daha önce ziyaret edildi mi? (cameFrom deðeri -1,-1 deðilse edilmiþtir)
            if (cameFrom[next.x][next.y] != sf::Vector2i(-1, -1)) continue;

            // 3. Engel kontrolü:
            // Kural: Hücre boþ olmalý VEYA Hücre bizim hedefimiz olmalý (Düþmana saldýrmak için)
            bool isWalkable = (gameManager.getSoldierAt(next) == nullptr);
            bool isTarget = (next == target);

            if (isWalkable || isTarget)
            {
                frontier.push(next);
                cameFrom[next.x][next.y] = current; // "Next'e Current'tan geldim"
            }
        }
    }
    // -----------------------

    // --- YOLU GERÝ OLUÞTURMA (Backtracking) ---
    std::vector<sf::Vector2i> path;

    if (!found)
    {
        return path; // Yol bulunamadý, boþ dön.
    }

    sf::Vector2i current = target;
    while (current != start)
    {
        path.push_back(current);
        current = cameFrom[current.x][current.y];
    }

    // Yol þu an [Hedef, ..., Adým 1] þeklinde ters. Düzeltelim:
    std::reverse(path.begin(), path.end());

    return path; // [Adým 1, Adým 2, ..., Hedef]
}

bool Pathfinder::isValid(const sf::Vector2i& pos, int width, int height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}