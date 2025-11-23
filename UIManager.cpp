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

    // Human Numbers
    labelHuman.setFont(font);
    labelHuman.setString("Humans:");
    labelHuman.setPosition(sf::Vector2f(400, 250));

    //createBtn(btnDecHuman, txtBack /*geçici*/, "-", 250, sf::Color::Red); // createBtn fonksiyonunu özelleþtirmen gerekebilir, manuel yapalým:
    btnDecHuman.setSize(sf::Vector2f(50, 50));
    btnDecHuman.setPosition(sf::Vector2f(600, 250));
    btnDecHuman.setFillColor(sf::Color(100, 100, 100));

    txtHumanCount.setFont(font);
    txtHumanCount.setCharacterSize(30);
    txtHumanCount.setPosition(sf::Vector2f(670, 255));

    btnIncHuman.setSize(sf::Vector2f(50, 50));
    btnIncHuman.setPosition(sf::Vector2f(720, 250));
    btnIncHuman.setFillColor(sf::Color(100, 100, 100));

    // AI Numbers
    labelAI.setFont(font);
    labelAI.setString("AI Bots:");
    labelAI.setPosition(sf::Vector2f(400, 350));

    btnDecAI.setSize(sf::Vector2f(50, 50));
    btnDecAI.setPosition(sf::Vector2f(600, 350));
    btnDecAI.setFillColor(sf::Color(100, 100, 100));

    txtAICount.setFont(font);
    txtAICount.setCharacterSize(30);
    txtAICount.setPosition(sf::Vector2f(670, 355));

    btnIncAI.setSize(sf::Vector2f(50, 50));
    btnIncAI.setPosition(sf::Vector2f(720, 350));
    btnIncAI.setFillColor(sf::Color(100, 100, 100));

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

    // --- GAME OVER PANEL ---
    gameOverPanel.setSize(sf::Vector2f(1366.f, 768.f));
    gameOverPanel.setFillColor(sf::Color(0, 0, 0, 200)); 

    txtGameOverTitle.setFont(font);
    txtGameOverTitle.setString("GAME OVER");
    txtGameOverTitle.setCharacterSize(80);
    txtGameOverTitle.setFillColor(sf::Color::Red);
    sf::FloatRect titleBounds = txtGameOverTitle.getLocalBounds();
    txtGameOverTitle.setOrigin(titleBounds.position + titleBounds.size / 2.f);
    txtGameOverTitle.setPosition(sf::Vector2f(1366.f / 2.f, 250.f));

    txtWinnerName.setFont(font);
    txtWinnerName.setCharacterSize(40);
    txtWinnerName.setFillColor(sf::Color::Yellow);
    // Pozisyonu draw fonksiyonunda winner ismine göre ortalayacaðýz, þimdilik boþ kalsýn.

    createBtn(btnRestart, txtRestart, "Restart Game", 450.f, sf::Color(0, 100, 0)); // Koyu Yeþil
    createBtn(btnReturnMain, txtReturnMain, "Main Menu", 550.f, sf::Color(50, 50, 50));
}

void UIManager::drawGameOverScreen(sf::RenderWindow& window, std::string winnerName)
{
    // Önce paneli çiz
    window.draw(gameOverPanel);
    window.draw(txtGameOverTitle);

    // Kazanan yazýsýný güncelle ve ortala
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

MenuAction UIManager::handleGameOverClick(int x, int y)
{
    sf::Vector2f mPos(static_cast<float>(x), static_cast<float>(y));

    if (btnReturnMain.getGlobalBounds().contains(mPos)) return MenuAction::ReturnToMain;

    if (btnRestart.getGlobalBounds().contains(mPos)) return MenuAction::RestartGame;

    return MenuAction::None;
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

    // Stringleri güncelle
    txtHumanCount.setString(std::to_string(humanCount));
    txtAICount.setString(std::to_string(aiCount));

    // Mod'a göre butonlarý çiz
    bool showHuman = (selectedMode == GameMode::PvP || selectedMode == GameMode::PvAI);
    bool showAI = (selectedMode == GameMode::AIvAI || selectedMode == GameMode::PvAI);

    if (showHuman) {
        window.draw(labelHuman);
        window.draw(btnDecHuman);
        window.draw(txtHumanCount);
        window.draw(btnIncHuman);
    }
    if (showAI) {
        window.draw(labelAI);
        window.draw(btnDecAI);
        window.draw(txtAICount);
        window.draw(btnIncAI);
    }
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

        std::string stats = "Move: " + std::to_string(selectedSoldier->getCurrentPoints());
        stats += "\nHP: " + std::to_string(selectedSoldier->getHp()) + " / 3"; // <-- YENÝ

        statsText.setString(stats);
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
