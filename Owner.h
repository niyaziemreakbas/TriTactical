#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Soldier.h"

class Owner
{
public:
    std::string name;
    sf::Color color;
    int gold = 90;

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
