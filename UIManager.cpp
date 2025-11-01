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
    LoadFont(font, "Assets/RockwellNova.ttf");

    ownerText.setFont(font);
    typeText.setFont(font);
    statsText.setFont(font);
    endTurnText.setFont(font);
    turnIndicatorText.setFont(font);

    // Owner Text
    ownerText.setCharacterSize(24); // Boyutunu ayarla
    ownerText.setFillColor(sf::Color::White);
    ownerText.setPosition(sf::Vector2f(20.f, 80.f));

    // Type Text
    typeText.setCharacterSize(20);
    typeText.setFillColor(sf::Color::Cyan);
    typeText.setPosition(sf::Vector2f(20.f, 110.f));

    // Stats Text
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::Yellow);
    statsText.setPosition(sf::Vector2f(20.f, 140.f));

    // YENÝ KOD: "Turu Bitir" butonunu ayarla.
    endTurnButton.setSize(sf::Vector2f(150.f, 50.f));
    endTurnButton.setFillColor(sf::Color(100, 100, 250)); // Mavi bir renk
    endTurnButton.setPosition(sf::Vector2f(0,0)); // Pencere boyutuna göre sað alta yerleþtir.

    endTurnText.setString("End Turn");
    endTurnText.setCharacterSize(24);
    endTurnText.setPosition(sf::Vector2f(endTurnButton.getPosition().x + 10,
        endTurnButton.getPosition().y + 10));

    turnIndicatorText.setCharacterSize(28);
    turnIndicatorText.setFillColor(sf::Color::White);
    turnIndicatorText.setPosition(sf::Vector2f(1000, 10)); // Ortaya yakýn

    infoPanel.setSize(sf::Vector2f(250.f, 120.f));
    infoPanel.setFillColor(sf::Color(0, 0, 0, 150)); // Yarý þeffaf siyah
    infoPanel.setOutlineColor(sf::Color::White);
    infoPanel.setOutlineThickness(1.f);
    infoPanel.setPosition(sf::Vector2f(20.f, 1366 - 140.f));

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
        std::cout << "Font Loaded : " << font.getInfo().family << std::endl;
    }
}

void UIManager::setEndTurnButtonActive(bool isActive)
{
    if (isActive)
    {
        endTurnButton.setFillColor(sf::Color(100, 100, 250)); // Normal mavi renk
        endTurnText.setString("End Turn");
    }
    else
    {
        endTurnButton.setFillColor(sf::Color(80, 80, 80)); // Pasif gri renk
        endTurnText.setString("AI is Thinking...");
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
    window.draw(infoPanel);

    window.draw(ownerText);
    window.draw(typeText);
    window.draw(statsText);

    // Yeni UI elemanlarýný çiz.
    window.draw(endTurnButton);
    window.draw(endTurnText);
    window.draw(turnIndicatorText);
}

