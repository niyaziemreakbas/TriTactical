#include "UIManager.h"
#include "Owner.h"
#include <string>
#include <iostream>
#include <sstream>

UIManager::UIManager() : 
font("Assets/arial.ttf"),
ownerText(font, "süleyman", 24),  
typeText(font, "", 20),    
statsText(font, "", 20),
endTurnText(font, "End Turn", 24), 
turnIndicatorText(font, "", 28),

//Setup Menu Texts
setupTitleText(font, "GAME SETUP", 48),
setupInfoText(font, "", 24),
txtStartGame(font, "START BATTLE", 24),
txtBack(font, "Back", 24),
labelHuman(font, "Humans:", 24),
txtHumanCount(font, "1", 30),
labelAI(font, "AI Bots:", 24),
txtAICount(font, "1", 30),
txtIncHuman(font, "+", 30),
txtDecHuman(font, "-", 30),
txtIncAI(font, "+", 30),
txtDecAI(font, "-", 30),

//Main Menu Texts
titleText(font, "TRI-TACTICAL", 60),
txtPvP(font, "Player vs Player", 24),
txtPvAI(font, "Player vs AI", 24),
txtAIvAI(font, "AI vs AI", 24),

//End Game Texts
txtGameOverTitle(font, "GAME OVER", 60),
txtWinnerName(font, "", 40),
txtReturnMain(font, "Main Menu", 24),
txtRestart(font, "Restart Game", 24)
{
    LoadFont(font, "Assets/RockwellNova.ttf");
    
	initGameUI();
    initMenuUI();

    onResize(1366, 768);
}

void UIManager::createBtn(sf::RectangleShape& btn, sf::Text& txt, std::string str, sf::Color color)
{
    btn.setSize(sf::Vector2f(300.f, 60.f));
    btn.setFillColor(color);
    btn.setOutlineThickness(2.f);
    btn.setOutlineColor(sf::Color::White);

    txt.setString(str);
    txt.setFillColor(sf::Color::White);

    sf::FloatRect bounds = txt.getLocalBounds();
    txt.setOrigin(bounds.position + bounds.size / 2.f);
}

void UIManager::alignButton(sf::RectangleShape& btn, sf::Text& txt, float x, float y)
{
    btn.setPosition(sf::Vector2f(x, y));

    txt.setPosition(btn.getPosition() + btn.getSize() / 2.f);
}

// Helper function to setup text elements
void UIManager::setupText(sf::Text& txt, int size, sf::Color color, std::string str)
{
    txt.setCharacterSize(size);
    txt.setFillColor(color);
    if (!str.empty()) {
        txt.setString(str);
    }

    sf::FloatRect b = txt.getLocalBounds();
    txt.setOrigin(b.position + b.size / 2.f);
}

// Helper function to setup small buttons
void UIManager::setupSmallBtn(sf::RectangleShape& btn, sf::Text& txt, std::string label)
{
    btn.setSize(sf::Vector2f(50.f, 50.f));
    btn.setFillColor(sf::Color(100, 100, 100));
    btn.setOutlineThickness(1.f);
    btn.setOutlineColor(sf::Color::White);

    txt.setString(label);
    txt.setCharacterSize(30);
    txt.setFillColor(sf::Color::White);

    sf::FloatRect b = txt.getLocalBounds();
    txt.setOrigin(b.position + b.size / 2.f);
}

void UIManager::initMenuUI()
{
    // Titles
    setupText(titleText, 60, sf::Color::Yellow);
    setupText(setupTitleText, 48, sf::Color::White);

    // Setup Labels (Stringler constructor'da verildi)
    setupText(labelHuman, 24, sf::Color::White);
    setupText(txtHumanCount, 30, sf::Color::White);
    setupText(labelAI, 24, sf::Color::White);
    setupText(txtAICount, 30, sf::Color::White);

    setupInfoText.setFont(font);
    setupInfoText.setCharacterSize(24); // Origin ayarý yapmýyoruz (Sol üst kalsýn)

    // Main Buttons
    createBtn(btnPvP, txtPvP, "Player vs Player", sf::Color(50, 50, 150));
    createBtn(btnPvAI, txtPvAI, "Player vs AI", sf::Color(150, 50, 50));
    createBtn(btnAIvAI, txtAIvAI, "AI vs AI", sf::Color(50, 150, 50));

    createBtn(btnStartGame, txtStartGame, "START BATTLE", sf::Color(200, 100, 0));
    createBtn(btnBack, txtBack, "Back", sf::Color(50, 50, 50));

    // Small Buttons (+/-) Style Helper
    auto styleSmallBtn = [&](sf::RectangleShape& btn) {
        btn.setSize(sf::Vector2f(50.f, 50.f));
        btn.setFillColor(sf::Color(100, 100, 100));
        btn.setOutlineThickness(1.f);
        btn.setOutlineColor(sf::Color::White);
        };

    styleSmallBtn(btnDecHuman); styleSmallBtn(btnIncHuman);
    styleSmallBtn(btnDecAI);    styleSmallBtn(btnIncAI);

    // +/- Text Origins
    setupText(txtIncHuman, 30, sf::Color::White); setupText(txtDecHuman, 30, sf::Color::White);
    setupText(txtIncAI, 30, sf::Color::White);    setupText(txtDecAI, 30, sf::Color::White);
}

void UIManager::initGameUI()
{
    // HUD Text Styles
    setupText(ownerText, 24, sf::Color::White); ownerText.setOrigin(sf::Vector2f(0, 0));
    setupText(typeText, 20, sf::Color::Cyan);   typeText.setOrigin(sf::Vector2f(0, 0));
    setupText(statsText, 20, sf::Color::Yellow); statsText.setOrigin(sf::Vector2f(0, 0));

    // End Turn Button
    endTurnButton.setSize(sf::Vector2f(150.f, 50.f));
    endTurnButton.setFillColor(sf::Color(100, 100, 250));
    setupText(endTurnText, 24, sf::Color::White); endTurnText.setOrigin(sf::Vector2f(0, 0)); // Manuel hizalama için

    // Info Panel
    infoPanel.setSize(sf::Vector2f(250.f, 120.f));
    infoPanel.setFillColor(sf::Color(0, 0, 0, 150));
    infoPanel.setOutlineColor(sf::Color::White);
    infoPanel.setOutlineThickness(1.f);

    // Turn Indicator
    setupText(turnIndicatorText, 28, sf::Color::White);

    ownerColorBackground.setSize(sf::Vector2f(40.f, 40.f));
    ownerColorBackground.setFillColor(sf::Color::White);
    ownerColorBackground.setOutlineColor(sf::Color::Black);
    ownerColorBackground.setOutlineThickness(1.f);
    ownerColorBackground.setOrigin(sf::Vector2f(20.f, 20.f));

    ownerColorCircle.setRadius(15.f);
    ownerColorCircle.setPointCount(50);
    ownerColorCircle.setOrigin(sf::Vector2f(15.f, 15.f)); 

    // Game Over Panel
    gameOverPanel.setFillColor(sf::Color(0, 0, 0, 200));
    setupText(txtGameOverTitle, 80, sf::Color::Red);
    setupText(txtWinnerName, 40, sf::Color::Yellow);

    createBtn(btnRestart, txtRestart, "Restart Game", sf::Color(0, 100, 0));
    createBtn(btnReturnMain, txtReturnMain, "Main Menu", sf::Color(50, 50, 50));
}

//---------- Resize Functions ----------

void UIManager::onResize(unsigned int width, unsigned int height)
{
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    resizeMainMenu(w, h);
    resizeSetupMenu(w, h);
    resizeGameUI(w, h);
    resizeGameOverUI(w, h);
}

void UIManager::resizeMainMenu(float w, float h)
{
    titleText.setPosition(sf::Vector2f(w / 2.f, h * 0.15f));

    float btnX = (w - 300.f) / 2.f; // Butonlar ortada

    alignButton(btnPvP, txtPvP, btnX, h * 0.35f);
    alignButton(btnPvAI, txtPvAI, btnX, h * 0.45f);
    alignButton(btnAIvAI, txtAIvAI, btnX, h * 0.55f);
}

void UIManager::resizeSetupMenu(float w, float h)
{
    float centerX = w / 2.f;
    float mainBtnX = (w - 300.f) / 2.f;

    setupTitleText.setPosition(sf::Vector2f(centerX, h * 0.1f));
    setupInfoText.setPosition(sf::Vector2f(w * 0.35f, h * 0.25f));

    alignButton(btnStartGame, txtStartGame, mainBtnX, h * 0.7f);
    alignButton(btnBack, txtBack, mainBtnX, h * 0.8f);

    // --- Counter Layout ---
    float TypeTextX = centerX - 170.f;
    float BtnDecX = centerX - 20.f;
    float BtnIncX = centerX + 100.f;

    // Calculate exact center between buttons for the number text
    // Minus Button End = (centerX - 20) + 50 = centerX + 30
    // Plus Button Start = centerX + 100
    // Gap = 70px -> Midpoint = centerX + 30 + 35
    float countTextX = centerX + 65.f;

    float HumanY = h * 0.3f;
    float AIY = h * 0.4f;

    // Vertical centering offset (Button Height 50 / 2)
    float btnHalfHeight = 25.f;
    float labelOffsetY = 10.f;

    // Human Row
    labelHuman.setPosition(sf::Vector2f(TypeTextX, HumanY + labelOffsetY));
    alignButton(btnDecHuman, txtDecHuman, BtnDecX, HumanY);
    alignButton(btnIncHuman, txtIncHuman, BtnIncX, HumanY);

    // Set text position to center of the gap + vertical center of button
    txtHumanCount.setPosition(sf::Vector2f(countTextX, HumanY + btnHalfHeight));

    // AI Row
    labelAI.setPosition(sf::Vector2f(TypeTextX, AIY + labelOffsetY));
    alignButton(btnDecAI, txtDecAI, BtnDecX, AIY);
    alignButton(btnIncAI, txtIncAI, BtnIncX, AIY);

    txtAICount.setPosition(sf::Vector2f(countTextX, AIY + btnHalfHeight));
}

void UIManager::resizeGameUI(float w, float h)
{
    // --- INFO PANEL ---
    float panelX = 20.f;
    float panelY = h - 150.f;
    float padding = 20.f;

    infoPanel.setPosition(sf::Vector2f(panelX, panelY));

    ownerText.setPosition(sf::Vector2f(panelX + padding, panelY + 5.f));
    typeText.setPosition(sf::Vector2f(panelX + padding, panelY + 35.f));
    statsText.setPosition(sf::Vector2f(panelX + padding, panelY + 65.f));

    endTurnButton.setPosition(sf::Vector2f(w - 170.f, h - 70.f));
    endTurnText.setPosition(endTurnButton.getPosition() + sf::Vector2f(18.f, 10.f));


    turnIndicatorText.setPosition(sf::Vector2f(w - 250.f, 30.f));

    ownerColorBackground.setPosition(sf::Vector2f(w - 250.f, 70.f));
    ownerColorCircle.setPosition(ownerColorBackground.getPosition()); 
}

void UIManager::resizeGameOverUI(float w, float h)
{
    gameOverPanel.setSize(sf::Vector2f(w, h));
    gameOverPanel.setPosition(sf::Vector2f(0, 0));

    txtGameOverTitle.setPosition(sf::Vector2f(w / 2.f, h * 0.3f));
    txtWinnerName.setPosition(sf::Vector2f(w / 2.f, h * 0.45f));

    float btnX = (w - 300.f) / 2.f;
    alignButton(btnRestart, txtRestart, btnX, h * 0.6f);
    alignButton(btnReturnMain, txtReturnMain, btnX, h * 0.75f);
}

//---------- Draw Functions ----------

void UIManager::drawGameUI(sf::RenderWindow& window)
{
    window.draw(infoPanel);
    window.draw(ownerText);
    window.draw(typeText);
    window.draw(statsText);

    window.draw(endTurnButton);
    window.draw(endTurnText);

    window.draw(turnIndicatorText);
    window.draw(ownerColorBackground);      // Beyaz Kare
    window.draw(ownerColorCircle);   // Renkli Daire
}

void UIManager::drawGameOverScreen(sf::RenderWindow& window, std::string winnerName)
{
    window.draw(gameOverPanel);
    window.draw(txtGameOverTitle);

    txtWinnerName.setString(winnerName);
    sf::FloatRect bounds = txtWinnerName.getLocalBounds();
    txtWinnerName.setOrigin(bounds.position + bounds.size / 2.f);
    txtWinnerName.setPosition(sf::Vector2f(1366.f / 2.f, 350.f));

    window.draw(txtWinnerName);

    window.draw(btnRestart);
    window.draw(txtRestart);

    window.draw(btnReturnMain);
    window.draw(txtReturnMain);
}

void UIManager::drawMainMenu(sf::RenderWindow& window)
{
    window.draw(titleText);
    window.draw(btnPvP); window.draw(txtPvP);
    window.draw(btnPvAI); window.draw(txtPvAI);
    window.draw(btnAIvAI); window.draw(txtAIvAI);
}

void UIManager::drawSetupMenu(sf::RenderWindow& window, GameMode selectedMode, int humanCount, int aiCount)
{
    window.draw(setupTitleText);
    window.draw(btnStartGame); window.draw(txtStartGame);
    window.draw(btnBack); window.draw(txtBack);

    // --- Update Strings & Re-Center Origins ---

    // Update Human Count
    txtHumanCount.setString(std::to_string(humanCount));
    sf::FloatRect bH = txtHumanCount.getLocalBounds();
    txtHumanCount.setOrigin(bH.position + bH.size / 2.f);

    // Update AI Count
    txtAICount.setString(std::to_string(aiCount));
    sf::FloatRect bA = txtAICount.getLocalBounds();
    txtAICount.setOrigin(bA.position + bA.size / 2.f);

    // --- Draw Elements based on Mode ---
    bool showHuman = (selectedMode == GameMode::PvP || selectedMode == GameMode::PvAI);
    bool showAI = (selectedMode == GameMode::AIvAI || selectedMode == GameMode::PvAI);

    if (showHuman) {
        window.draw(labelHuman);
        window.draw(btnDecHuman); window.draw(txtDecHuman);
        window.draw(txtHumanCount);
        window.draw(btnIncHuman); window.draw(txtIncHuman);
    }
    if (showAI) {
        window.draw(labelAI);
        window.draw(btnDecAI); window.draw(txtDecAI);
        window.draw(txtAICount);
        window.draw(btnIncAI); window.draw(txtIncAI);
    }
}


//---------- Buton Click Handlers ----------

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
    sf::Vector2f mPos(static_cast<float>(x), static_cast<float>(y));

    if (btnStartGame.getGlobalBounds().contains(mPos)) return MenuAction::StartGame;
    if (btnBack.getGlobalBounds().contains(mPos)) return MenuAction::BackToMenu;

    // +/- Buton kontrolleri
    if (btnIncHuman.getGlobalBounds().contains(mPos)) return MenuAction::IncHuman;
    if (btnDecHuman.getGlobalBounds().contains(mPos)) return MenuAction::DecHuman;
    if (btnIncAI.getGlobalBounds().contains(mPos)) return MenuAction::IncAI;
    if (btnDecAI.getGlobalBounds().contains(mPos)) return MenuAction::DecAI;

    return MenuAction::None;
}

bool UIManager::isEndTurnButtonClicked(sf::Vector2i mousePos)
{
    return endTurnButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

MenuAction UIManager::handleGameOverClick(int x, int y)
{
    sf::Vector2f mPos(static_cast<float>(x), static_cast<float>(y));

    if (btnReturnMain.getGlobalBounds().contains(mPos)) return MenuAction::ReturnToMain;

    if (btnRestart.getGlobalBounds().contains(mPos)) return MenuAction::RestartGame;

    return MenuAction::None;
}

// Helper to load font
void UIManager::LoadFont(sf::Font& font, std::string str)
{
    if (!font.openFromFile(str)) {

        std::cerr << "ERROR::COULD NOT LOAD FILE::" << str << "!!!" << std::endl;
    }
    else {
        std::cout << "Font Loaded Successfully : " << font.getInfo().family << std::endl;
    }
}

// In Game UI
void UIManager::updateGameUI(Soldier* selectedSoldier, Owner* currentOwner)
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

        std::string stats = "Move: " + std::to_string(selectedSoldier->getCurrentPoints());
        stats += "\nHP: " + std::to_string(selectedSoldier->getHp()) + " / 3";

        statsText.setString(stats);
    }
    else
    {
        ownerText.setString("");
        typeText.setString("");
        statsText.setString("");
    }

    if (currentOwner != nullptr)
    {
        turnIndicatorText.setString("Current Turn: " + currentOwner->name);

        ownerColorCircle.setFillColor(currentOwner->color);

        sf::FloatRect bounds = turnIndicatorText.getLocalBounds();
        turnIndicatorText.setOrigin(bounds.position + bounds.size / 2.f);
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

