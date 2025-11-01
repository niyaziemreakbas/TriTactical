#pragma once
#include <SFML/Graphics.hpp>

//Soldier don't need to know all about Owner for now. We need this to prevent circular dependency.
class Owner;

class Soldier
{
public:

    // Square > Triangle > Circle > Square
    enum class Type { Square, Circle, Triangle };

    Soldier(Owner* owner, Type type, sf::Vector2i gridPosition);

    Owner* owner;
    Type type;
    sf::Vector2i gridPosition;

    void toggleSelection();

    void draw(sf::RenderWindow& window, float tileSize, float offsetX, float offsetY);

    bool getSelectState();

    void resetMovementPoints();
	int getCurrentPoints() const { return currentMovementPoints; }
	int setCurrentPoints(int points) { return currentMovementPoints += points; }

    void setOwner(Owner* owner);

    bool moveTo(const sf::Vector2i& newPosition);

	// Animation update per frame
    void update(float dt);
    void startMoveAnimation(const sf::Vector2f& startPixel, const sf::Vector2f& targetPixel);
    bool IsAnimating() const { return isAnimating; }

private:

    int cost = 30;
    int movementPoints = 3;
	int currentMovementPoints = movementPoints;

    sf::CircleShape shape; 

    bool isSelected = false;
    sf::CircleShape selectionHighlight;

	// Animation members
    bool isAnimating = false;      
    sf::Vector2f pixelPosition;  
    sf::Vector2f startPixelPos;   
	sf::Vector2f targetPixelPos;   
	float animationTime = 0.f;   
    const float animationDuration = 0.25f; 
};