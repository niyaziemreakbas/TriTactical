#include "Soldier.h"
#include "Owner.h"
#include <iostream>

Soldier::Soldier(Owner* p_owner, Type p_type, sf::Vector2i p_gridPosition)
    : owner(p_owner), type(p_type), gridPosition(p_gridPosition)
{
    // Configure the shape based on the soldier type.
    shape.setRadius(20.f); 
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius())); 
    shape.setFillColor(p_owner->color); 
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.f);

	// Configure selection highlight
    selectionHighlight.setRadius(20.f);
    selectionHighlight.setFillColor(sf::Color::Transparent);
    selectionHighlight.setOutlineColor(sf::Color::Green);
    selectionHighlight.setOutlineThickness(3.f);
    selectionHighlight.setOrigin(
        sf::Vector2f(selectionHighlight.getRadius(), selectionHighlight.getRadius())
    );

    switch (type)
    {
    case Type::Triangle:
        shape.setPointCount(3);
		selectionHighlight.setPointCount(3);
        break;
    case Type::Circle:
        shape.setPointCount(100);
		selectionHighlight.setPointCount(100);
        break;
    case Type::Square:
        shape.setPointCount(4);
		selectionHighlight.setPointCount(4);
        break;
    }
}

void Soldier::resetMovementPoints()
{
    currentMovementPoints = movementPoints; // Deðeri baþlangýç deðerine döndür.
}

void Soldier::setOwner(Owner* newOwner)
{
    if (owner == newOwner)
    {
        return;
    }
    owner = newOwner;
    shape.setFillColor(newOwner->color);
}

void Soldier::toggleSelection()
{
	std::cout << "Toggling selection for soldier at (" << gridPosition.x << ", " << gridPosition.y << ")\n";
    isSelected = !isSelected;
}

void Soldier::draw(sf::RenderWindow& window, float tileSize, float offsetX, float offsetY)
{
    // Calculate the pixel position on the screen based on grid position.
    float pixelX = offsetX + (gridPosition.x * tileSize) + (tileSize / 2);
    float pixelY = offsetY + (gridPosition.y * tileSize) + (tileSize / 2);

    shape.setPosition(sf::Vector2f(pixelX, pixelY));
    window.draw(shape);

    if (isSelected)
    {
        selectionHighlight.setPosition(sf::Vector2f(pixelX, pixelY));
        window.draw(selectionHighlight);
    }
}

bool Soldier::getSelectState()
{
    if (isSelected)
        return true;
    
    return false;
}
