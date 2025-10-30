#include "UIManager.h"
#include "Owner.h"
#include <string>
#include <iostream>
#include <sstream>

UIManager::UIManager() : 
font("Assets/arial.ttf"),
ownerText(font, "süleyman", 24),     // ownerText'i boþ string, Sýnýfýn 'font' üyesi ve 24 punto ile baþlat
typeText(font, "", 20),      // typeText'i baþlat
statsText(font, "", 20),     // statsText'i baþlat
endTurnText(font, "End Turn", 24), // endTurnText'i baþlat
turnIndicatorText(font, "", 28)
{
    //LoadFont(font, "Assets/RockwellNova.ttf");

    std::cout << "Font Type :: " << font.getInfo().family;

    ownerText.setFont(font);
    typeText.setFont(font);
    statsText.setFont(font);
    endTurnText.setFont(font);
    turnIndicatorText.setFont(font);

    // Owner Text
    ownerText.setCharacterSize(24); // Boyutunu ayarla
    ownerText.setFillColor(sf::Color::White);
    ownerText.setPosition(sf::Vector2f(20.f, 20.f));

    // Type Text
    typeText.setCharacterSize(20);
    typeText.setFillColor(sf::Color::Cyan);
    typeText.setPosition(sf::Vector2f(20.f, 50.f));

    // Stats Text
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::Yellow);
    statsText.setPosition(sf::Vector2f(20.f, 80.f));

    // YENÝ KOD: "Turu Bitir" butonunu ayarla.
    endTurnButton.setSize(sf::Vector2f(150.f, 50.f));
    endTurnButton.setFillColor(sf::Color(100, 100, 250)); // Mavi bir renk
    endTurnButton.setPosition(sf::Vector2f(0,0)); // Pencere boyutuna göre sað alta yerleþtir.

    endTurnText.setString("End Turn");
    endTurnText.setCharacterSize(24);
    endTurnText.setPosition(sf::Vector2f(30, 30));

    turnIndicatorText.setCharacterSize(28);
    turnIndicatorText.setFillColor(sf::Color::White);
    turnIndicatorText.setPosition(sf::Vector2f(10, 10)); // Ortaya yakýn

    std::cout << "UIManager initialized successfully.\n";
}

bool UIManager::isEndTurnButtonClicked(sf::Vector2i mousePos)
{
    return endTurnButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void UIManager::LoadFont(sf::Font& font, std::string str)
{
    if (!font.openFromFile(str)) {

        std::cerr << "ERROR::COULD NOT LOAD FILE::" << str << "!!!" << std::endl;
    }
    else {
        std::cout << "Thats Okay ::" << font.getInfo().family;
    }
}

void UIManager::update(Soldier* selectedSoldier, const std::string& currentPlayerName)
{
    if (selectedSoldier != nullptr)
    {
        ownerText.setString("Owner: " + selectedSoldier->owner->name);

        std::string typeStr;
        switch (selectedSoldier->type)
        {
        case Soldier::Type::Square: typeStr = "Type: Square"; break;
        case Soldier::Type::Circle: typeStr = "Type: Circle"; break;
        case Soldier::Type::Triangle: typeStr = "Type: Triangle"; break;
        }
        typeText.setString(typeStr);

        statsText.setString("Move Points: " + std::to_string(selectedSoldier->getCurrentPoints()));

		std::cout << "UI updated for selected soldier.\n";
    }
    else
    {
        ownerText.setString("");
        typeText.setString("");
        statsText.setString("");
    }

    turnIndicatorText.setString("Current Turn: " + currentPlayerName);
}

void UIManager::draw(sf::RenderWindow& window)
{
    window.draw(ownerText);
    window.draw(typeText);
    window.draw(statsText);

    // Yeni UI elemanlarýný çiz.
    window.draw(endTurnButton);
    window.draw(endTurnText);
    window.draw(turnIndicatorText);
}