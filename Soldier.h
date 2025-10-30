#pragma once
#include <SFML/Graphics.hpp>

//Soldier don't need to know all about Owner
class Owner;

class Soldier
{
public:
    // Enum to manage soldier types easily.
    enum class Type { Square, Circle, Triangle };

    Soldier(Owner* owner, Type type, sf::Vector2i gridPosition);

    Owner* owner;
    Type type;
    sf::Vector2i gridPosition;

    void toggleSelection();

    // A function to draw the soldier.
    void draw(sf::RenderWindow& window, float tileSize, float offsetX, float offsetY);

    bool getSelectState();

    void resetMovementPoints();

	int getCurrentPoints() const { return currentMovementPoints; }

	int setCurrentPoints(int points) { return currentMovementPoints = points; }

private:

    const int cost = 30;
    int movementPoints = 3;
	int currentMovementPoints = movementPoints;

    sf::CircleShape shape; 

    bool isSelected = false;
    sf::CircleShape selectionHighlight;
};