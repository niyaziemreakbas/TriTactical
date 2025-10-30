#pragma once
#include <SFML/Graphics.hpp>

struct GridCell
{
    // The visual representation of the cell.
    sf::RectangleShape shape;

    // We can add more properties later, like:
    // bool isOccupied = false;
    // int ownerId = -1;
};