#include "Map.h"
#include <random>

Map::Map(unsigned int windowWidth, unsigned int windowHeight)
{
    // generateMap fonksiyonunun içeriði buraya taþýndý.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribWidth(5, 5);
    std::uniform_int_distribution<> distribHeight(5, 5);

    mapWidth = distribWidth(gen);
    mapHeight = distribHeight(gen);

    totalMapWidth = mapWidth * tileSize;
    totalMapHeight = mapHeight * tileSize;

    mapOffsetX = (windowWidth - totalMapWidth) / 2.0f;
    mapOffsetY = (windowHeight - totalMapHeight) / 2.0f;

    grid.resize(mapWidth);
    for (int i = 0; i < mapWidth; ++i) {
        grid[i].resize(mapHeight);
    }

    for (int x = 0; x < mapWidth; ++x) {
        for (int y = 0; y < mapHeight; ++y) {
            grid[x][y].shape.setSize(sf::Vector2f(tileSize, tileSize));
            grid[x][y].shape.setPosition(sf::Vector2f(mapOffsetX + (x * tileSize), mapOffsetY + (y * tileSize)));
            grid[x][y].shape.setFillColor(sf::Color::White);
            grid[x][y].shape.setOutlineThickness(3.0f);
            grid[x][y].shape.setOutlineColor(sf::Color(200, 200, 200));
        }
    }
}

void Map::draw(sf::RenderWindow& window)
{
    for (int x = 0; x < mapWidth; ++x) {
        for (int y = 0; y < mapHeight; ++y) {
            window.draw(grid[x][y].shape);
        }
    }
}

void Map::colorCell(sf::Vector2i position, sf::Color color)
{
    if (position.x >= 0 && position.x < mapWidth && position.y >= 0 && position.y < mapHeight) {
        grid[position.x][position.y].shape.setFillColor(color);
    }
}

void Map::resetCellColor(sf::Vector2i position)
{
    colorCell(position, sf::Color::White);
}