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
turnIndicatorText(font, "", 28),

//Setup Menu Texts
setupTitleText(font, "GAME SETUP", 48),
setupInfoText(font, "", 24),
txtStartGame(font, "START BATTLE", 24),
txtBack(font, "Back", 24),

//Main Menu Texts
titleText(font, "TRI-TACTICAL", 60),
txtPvP(font, "Player vs Player", 24),
txtPvAI(font, "Player vs AI", 24),
txtAIvAI(font, "AI vs AI", 24)
{
    LoadFont(font, "Assets/RockwellNova.ttf");
    
	initGameUI();
    initMenuUI();

    std::cout << "UIManager initialized successfully.\n";
}

void UIManager::createBtn(sf::RectangleShape& btn, sf::Text& txt, std::string str, float y, sf::Color color)
{
    // 1. Buton Ayarlarý
    btn.setSize(sf::Vector2f(300.f, 60.f));
    btn.setFillColor(color);
    btn.setOutlineThickness(2.f);
    btn.setOutlineColor(sf::Color::White);
    // Ekraný 1366 kabul ediyoruz
    btn.setPosition(sf::Vector2f((1366.f - 300.f) / 2.f, y));

    // 2. Yazý Ayarlarý
    txt.setFont(font);
    txt.setString(str);
    txt.setCharacterSize(24);
    txt.setFillColor(sf::Color::White);

    sf::FloatRect bounds = txt.getLocalBounds();

    // Origin'i yazýnýn tam ortasýna alýyoruz:
    txt.setOrigin(bounds.position + bounds.size / 2.f);

    // Yazýyý butonun tam ortasýna koyuyoruz:
    txt.setPosition(btn.getPosition() + btn.getSize() / 2.f);
}

void UIManager::initMenuUI()
{
    // Title Text Ortala
    titleText.setFont(font);
    titleText.setString("TRI-TACTICAL");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    // SFML 3.0 Origin Ayarý:
    titleText.setOrigin(titleBounds.position + titleBounds.size / 2.f);
    titleText.setPosition(sf::Vector2f(1366.f / 2.f, 100.f));

    // Butonlarý oluþtur
    createBtn(btnPvP, txtPvP, "Player vs Player", 250.f, sf::Color(50, 50, 150));
    createBtn(btnPvAI, txtPvAI, "Player vs AI", 350.f, sf::Color(150, 50, 50));
    createBtn(btnAIvAI, txtAIvAI, "AI vs AI", 450.f, sf::Color(50, 150, 50));

    // Setup Ekraný
    setupTitleText.setFont(font);
    setupTitleText.setString("GAME SETUP");
    setupTitleText.setCharacterSize(48);
    sf::FloatRect setupBounds = setupTitleText.getLocalBounds();
    setupTitleText.setOrigin(setupBounds.position + setupBounds.size / 2.f);
    setupTitleText.setPosition(sf::Vector2f(1366.f / 2.f, 50.f));

    setupInfoText.setFont(font);
    setupInfoText.setCharacterSize(24);
    // Info text uzun metin olacaðý için origin deðiþtirmeyelim, sol üst kalsýn
    setupInfoText.setPosition(sf::Vector2f(500.f, 150.f));

    createBtn(btnStartGame, txtStartGame, "START BATTLE", 500.f, sf::Color(200, 100, 0));
    createBtn(btnBack, txtBack, "Back", 600.f, sf::Color(50, 50, 50));
}

void UIManager::initGameUI()
{
    // --- Owner Text ---
    ownerText.setCharacterSize(24);
    ownerText.setFillColor(sf::Color::White);
    ownerText.setPosition(sf::Vector2f(20.f, 80.f));

    // --- Type Text ---
    typeText.setCharacterSize(20);
    typeText.setFillColor(sf::Color::Cyan);
    typeText.setPosition(sf::Vector2f(20.f, 110.f));

    // --- Stats Text ---
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::Yellow);
    statsText.setPosition(sf::Vector2f(20.f, 140.f));

    // --- Turu Bitir Butonu ---
    endTurnButton.setSize(sf::Vector2f(150.f, 50.f));
    endTurnButton.setFillColor(sf::Color(100, 100, 250));
    // Sol üst (0,0) yerine sað alt köþe mantýðýný kullanmak istersen:
    // endTurnButton.setPosition(sf::Vector2f(1366.f - 170.f, 768.f - 70.f)); 
    // Senin kodunda (0,0) duruyor, deðiþtirmedim:
    endTurnButton.setPosition(sf::Vector2f(0, 0));

    endTurnText.setString("End Turn");
    endTurnText.setCharacterSize(24);
    endTurnText.setPosition(sf::Vector2f(endTurnButton.getPosition().x + 10,
        endTurnButton.getPosition().y + 10));

    // --- Turn Indicator ---
    turnIndicatorText.setCharacterSize(28);
    turnIndicatorText.setFillColor(sf::Color::White);
    turnIndicatorText.setPosition(sf::Vector2f(1000.f, 10.f));

    // --- Info Panel ---
    infoPanel.setSize(sf::Vector2f(250.f, 120.f));
    infoPanel.setFillColor(sf::Color(0, 0, 0, 150));
    infoPanel.setOutlineColor(sf::Color::White);
    infoPanel.setOutlineThickness(1.f);
    infoPanel.setPosition(sf::Vector2f(20.f, 1366.f - 140.f));
    // NOT: Y koordinatý için "1366 - 140" yazmýþsýn, 
    // normalde ekran yüksekliði (768) olmasý gerekebilir ama senin düzenine göre býraktým.
}

void UIManager::drawMainMenu(sf::RenderWindow& window)
{
    window.draw(titleText);
    window.draw(btnPvP); window.draw(txtPvP);
    window.draw(btnPvAI); window.draw(txtPvAI);
    window.draw(btnAIvAI); window.draw(txtAIvAI);
}

void UIManager::drawSetupMenu(sf::RenderWindow& window, GameMode selectedMode)
{
    // Mod bilgisine göre yazýyý güncelle (bunu her frame yapmak yerine deðiþimde yapmak daha iyi ama þimdilik sorun deðil)
    std::string modeStr = "Unknown";
    if (selectedMode == GameMode::PvP) modeStr = "Player vs Player";
    else if (selectedMode == GameMode::PvAI) modeStr = "Player vs AI";
    else if (selectedMode == GameMode::AIvAI) modeStr = "AI vs AI";

    setupInfoText.setString("Selected Mode: " + modeStr + "\n\nSettings area...");

    window.draw(setupTitleText);
    window.draw(setupInfoText);
    window.draw(btnStartGame); window.draw(txtStartGame);
    window.draw(btnBack); window.draw(txtBack);
}

MenuAction UIManager::handleMenuClick(int x, int y)
{
    sf::Vector2f mousePos(static_cast<float>(x), static_cast<float>(y));

    if (btnPvP.getGlobalBounds().contains(mousePos)) return MenuAction::SelectPvP;
    if (btnPvAI.getGlobalBounds().contains(mousePos)) return MenuAction::SelectPvAI;
    if (btnAIvAI.getGlobalBounds().contains(mousePos)) return MenuAction::SelectAIvAI;

    return MenuAction::None;
}

MenuAction UIManager::handleSetupClick(int x, int y)
{
    sf::Vector2f mousePos(static_cast<float>(x), static_cast<float>(y));

    if (btnStartGame.getGlobalBounds().contains(mousePos)) return MenuAction::StartGame;
    if (btnBack.getGlobalBounds().contains(mousePos)) return MenuAction::BackToMenu;

    return MenuAction::None;
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
        endTurnText.setString("Thinking...");
    }
}

void UIManager::updateGameUI(Soldier* selectedSoldier, const std::string& currentPlayerName)
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

void UIManager::drawGameUI(sf::RenderWindow& window)
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

