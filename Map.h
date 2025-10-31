#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "GridCell.h"

class Map
{
public:
    Map(unsigned int windowWidth, unsigned int windowHeight);

    void draw(sf::RenderWindow& window);
    void colorCell(sf::Vector2i position, sf::Color color);
    void resetCellColor(sf::Vector2i position);

    // GameManager'ýn harita bilgilerine eriþmesi için "getter" fonksiyonlarý
    sf::Vector2f getMapOffset() const { return { mapOffsetX, mapOffsetY }; }
    float getTileSize() const { return tileSize; }
    sf::Vector2i getDimensions() const { return { mapWidth, mapHeight }; }

private:
    int mapWidth = 0, mapHeight = 0;
    float tileSize = 50.0f;
    float mapOffsetX = 0.f, mapOffsetY = 0.f;
    float totalMapWidth = 0.f, totalMapHeight = 0.f;

    std::vector<std::vector<GridCell>> grid;
};