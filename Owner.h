#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Soldier.h"

class Owner
{
public:
    std::string name;
    sf::Color color;
    int gold = 90; // Initial gold value is 90[cite: 50].

    std::vector<Soldier> soldiers;

    // A virtual destructor is important for base classes.
    virtual ~Owner() = default;
};

// Subclass for the human player.
class PlayerOwner : public Owner
{
public:
    PlayerOwner(std::string name, sf::Color color);
};

// Subclass for the AI.
class AIOwner : public Owner
{
public:
    AIOwner(std::string name, sf::Color color);
};