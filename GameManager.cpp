#include "GameManager.h"
#include "Owner.h"
#include <random> // better random number generation than rand().
#include "Soldier.h"
#include <iostream>
#include <string>
#include "AIOwner.h"

GameManager::GameManager(unsigned int windowWidth, unsigned int windowHeight, UIManager& uiMgr)
    : m_map(windowWidth, windowHeight),
    uiManager(uiMgr)
{
    //createInitialUnits();
}

//void GameManager::createInitialUnits()
//{
//    // Sahipleri (Player ve AI) oluþtur
//    owners.push_back(std::make_unique<PlayerOwner>("Player", sf::Color::Blue));
//    owners.push_back(std::make_unique<AIOwner>("AI 1", sf::Color::Red));
//
//    sf::Vector2i mapDims = m_map.getDimensions();
//
//    // --- Oyuncu (Mavi) için 3 farklý asker oluþtur ---
//    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Triangle, sf::Vector2i(1, 1));
//    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Circle, sf::Vector2i(1, 2));
//    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Square, sf::Vector2i(2, 1));
//
//    // --- Yapay Zeka (Kýrmýzý) için 3 farklý asker oluþtur ---
//    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Square, sf::Vector2i(mapDims.x - 2, mapDims.y - 2));
//    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Circle, sf::Vector2i(mapDims.x - 2, mapDims.y - 3));
//    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Triangle, sf::Vector2i(mapDims.x - 3, mapDims.y - 2));
//
//    // UI'ý baþlangýç durumu için ayarla
//    uiManager.setEndTurnButtonActive(true);
//    uiManager.updateGameUI(nullptr, owners[currentPlayerIndex]->name);
//}

void GameManager::startGame(GameMode mode)
{
    std::cout << "Starting game in mode: " << static_cast<int>(mode) << "\n";

    // 1. Önceki oyundan kalan her þeyi temizle
    owners.clear();
    selectedSoldier = nullptr;
    moveableCells.clear();
    attackableCells.clear();

    // Harita üzerindeki renkleri sýfýrla
    // (Eðer Map sýnýfýnda reset fonksiyonun yoksa þimdilik sadece grid'i temizlemiþ varsayýyoruz)

    // 2. Seçilen moda göre Oyuncularý (Owners) Oluþtur
    switch (mode)
    {
    case GameMode::PvP:
        owners.push_back(std::make_unique<PlayerOwner>("Player 1", sf::Color::Blue));
        owners.push_back(std::make_unique<PlayerOwner>("Player 2", sf::Color::Red));
        break;

    case GameMode::PvAI:
        owners.push_back(std::make_unique<PlayerOwner>("Player", sf::Color::Blue));
        owners.push_back(std::make_unique<AIOwner>("AI Bot", sf::Color::Red));
        break;

    case GameMode::AIvAI:
        owners.push_back(std::make_unique<AIOwner>("AI Alpha", sf::Color::Blue));
        owners.push_back(std::make_unique<AIOwner>("AI Beta", sf::Color::Red));
        break;
    }

    // 3. Askerleri Yerleþtir
    sf::Vector2i mapDims = m_map.getDimensions();

    // -- Mavi Takým (0. indeks) --
    // Dikkat: owners[0] her zaman var, güvenle eriþebiliriz.
    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Triangle, sf::Vector2i(1, 1));
    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Circle, sf::Vector2i(1, 2));
    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Square, sf::Vector2i(2, 1));

    // -- Kýrmýzý Takým (1. indeks) --
    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Square, sf::Vector2i(mapDims.x - 2, mapDims.y - 2));
    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Circle, sf::Vector2i(mapDims.x - 2, mapDims.y - 3));
    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Triangle, sf::Vector2i(mapDims.x - 3, mapDims.y - 2));

    // 4. Tur Sistemini Baþlat
    currentPlayerIndex = 0;

    //// Tur baþý savaþ kontrolü (dönüþüm varsa hemen uygula)
    //for (auto& soldier : owners[currentPlayerIndex]->soldiers) {
    //    checkForCombat(&soldier);
    //}

    // Ýlk oyuncunun kim olduðuna göre durumu ayarla
    if (dynamic_cast<AIOwner*>(owners[0].get())) {
        currentGameState = GameState::AI_THINKING;
        uiManager.setEndTurnButtonActive(false);
        processAITurn(); // YZ ise hemen düþünsün
    }
    else {
        currentGameState = GameState::PLAYER_INPUT;
        uiManager.setEndTurnButtonActive(true);
    }

    uiManager.updateGameUI(nullptr, owners[currentPlayerIndex]->name);
}

void GameManager::update(float dt)
{
    // Animasyonlarý her zaman güncelle
    for (auto& owner : owners) {
        for (auto& soldier : owner->soldiers) {
            soldier.update(dt);
        }
    }

    switch (currentGameState)
    {
    case GameState::PLAYER_INPUT:
        // Oyuncu týklamasý bekleniyor (handleClick içinde iþleniyor)
        break;

    case GameState::AI_THINKING:
    {
        AIOwner* aiOwner = dynamic_cast<AIOwner*>(owners[currentPlayerIndex].get());
        if (aiOwner) {
            // YZ'ye "Sadece BÝR adým at" diyoruz.
            bool didMove = aiOwner->processTurn(*this);

            if (didMove) {
                // Eðer hamle yaptýysa, animasyon moduna geç.
                currentGameState = GameState::ANIMATING;
            }
            else {
                // Eðer yapacak hamlesi kalmadýysa turu bitir.
                std::cout << "AI finished thinking. Ending turn.\n";
                endTurn();
            }
        }
        break;
    }

    case GameState::ANIMATING:
    {
        // Animasyonlarýn bitip bitmediðini kontrol et.
        bool isAnyAnimationRunning = false;
        for (const auto& owner : owners) {
            for (const auto& soldier : owner->soldiers) {
                // Soldier sýnýfýna public bir isAnimating deðiþkeni veya getter eklediðinden emin ol
                // Senin kodunda 'isAnimating' private olabilir, getter lazým olabilir.
                // Þimdilik senin animation logic'ine güveniyoruz.
                if (soldier.IsAnimating()) { // Soldier.h'de bunu public yap veya getter kullan
                    isAnyAnimationRunning = true;
                    break;
                }
            }
            if (isAnyAnimationRunning) break;
        }

        // Eðer animasyonlar bittiyse...
        if (!isAnyAnimationRunning) {
            // Tekrar sýranýn kimde olduðuna bak.
            // Eðer YZ sýrasýysa, YZ düþünmeye devam etmeli (belki baþka hamlesi vardýr).
            if (dynamic_cast<AIOwner*>(owners[currentPlayerIndex].get())) {
                currentGameState = GameState::AI_THINKING;
            }
            else {
                // Oyuncu ise (oyuncu animasyonu bittiyse) input'a dön
                currentGameState = GameState::PLAYER_INPUT;
            }
        }
        break;
    }
    }
}

void GameManager::handleClick(int mouseX, int mouseY)
{
    if (currentGameState == GameState::ANIMATING)
    {
        return;
    }

    if (uiManager.isEndTurnButtonClicked({ mouseX, mouseY }))
    {
        endTurn();
        return;
    }

    const sf::Vector2f mapOffset = m_map.getMapOffset();
    const float tileSize = m_map.getTileSize();
    const sf::Vector2i mapDims = m_map.getDimensions();

    // Þimdi bu doðru bilgilerle koordinatlarý hesapla.
    int gridX = static_cast<int>((mouseX - mapOffset.x) / tileSize);
    int gridY = static_cast<int>((mouseY - mapOffset.y) / tileSize);

    Soldier* clickedSoldier = getSoldierAt({ gridX, gridY });

    // Durum A: Boþ bir hücreye týklandý.
    if (clickedSoldier == nullptr)
    {
        if (selectedSoldier != nullptr)
        {
            sf::Vector2i clickedCell(gridX, gridY);

            bool canMove = std::find(moveableCells.begin(), moveableCells.end(), clickedCell) != moveableCells.end();
            bool canAttackMove = std::find(attackableCells.begin(), attackableCells.end(), clickedCell) != attackableCells.end();

            if (canMove || canAttackMove)
            {
                Soldier* soldierToMove = selectedSoldier;

                // 1. Merkezi hareket komutunu çaðýr.
                executeMove(soldierToMove, clickedCell);

                // 2. Hareketi baþlattýktan sonra, oyuncuya özel iþlemleri yap.
                soldierToMove->toggleSelection();
                selectedSoldier = nullptr;
                calculateMoveableCells(nullptr);
            }
            else
            {
                selectedSoldier->toggleSelection();

                selectedSoldier = nullptr;
                calculateMoveableCells(nullptr);
            }
        }
    }
    // Durum B: Bir askere týklandý.
    else
    {
        if (clickedSoldier->owner == owners[currentPlayerIndex].get()) {
            if (selectedSoldier != nullptr && selectedSoldier != clickedSoldier)
            {
                selectedSoldier->toggleSelection();
            }

            clickedSoldier->toggleSelection();

            if (clickedSoldier->getSelectState())
            {
                selectedSoldier = clickedSoldier;
            }
            else
            {
                selectedSoldier = nullptr;
            }
            calculateMoveableCells(selectedSoldier);
        }
    }
    uiManager.updateGameUI(selectedSoldier, owners[currentPlayerIndex]->name);
}

void GameManager::draw(sf::RenderWindow& window)
{
    // Renklendirme iþini Map sýnýfýna devret
    for (const auto& cellPos : moveableCells) {
        m_map.colorCell(cellPos, sf::Color(144, 238, 144));
    }
    for (const auto& cellPos : attackableCells) {
        m_map.colorCell(cellPos, sf::Color(250, 128, 114));
    }

    // Haritayý çiz
    m_map.draw(window);

    // Renkleri sýfýrla
    for (const auto& cellPos : moveableCells) {
        m_map.resetCellColor(cellPos);
    }
    for (const auto& cellPos : attackableCells) {
        m_map.resetCellColor(cellPos);
    }

    // Askerleri çiz (offset ve tileSize'ý haritadan al)
    const sf::Vector2f mapOffset = m_map.getMapOffset();
    const float tileSize = m_map.getTileSize();
    for (auto& owner : owners) {
        for (auto& soldier : owner->soldiers) {
            // DEÐÝÞÝKLÝK: Eski, parametreli haliyle çaðýr.
            soldier.draw(window, tileSize, mapOffset.x, mapOffset.y);
        }
    }

    uiManager.drawGameUI(window);
}

sf::Vector2i GameManager::getRandomMapCell() const
{
    const sf::Vector2i mapDims = m_map.getDimensions();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribX(0, mapDims.x - 1);
    std::uniform_int_distribution<> distribY(0, mapDims.y - 1);

    return { distribX(gen), distribY(gen) };
}

void GameManager::executeMove(Soldier* soldier, const sf::Vector2i& targetCell)
{
    if (soldier == nullptr) return;

    const sf::Vector2f mapOffset = m_map.getMapOffset();
    const float tileSize = m_map.getTileSize();
    sf::Vector2f startPixel = {
        mapOffset.x + (soldier->gridPosition.x * tileSize) + (tileSize / 2),
        mapOffset.y + (soldier->gridPosition.y * tileSize) + (tileSize / 2)
    };
    sf::Vector2f targetPixel = {
        mapOffset.x + (targetCell.x * tileSize) + (tileSize / 2),
        mapOffset.y + (targetCell.y * tileSize) + (tileSize / 2)
    };

    if (soldier->moveTo(targetCell))
    {
        soldier->startMoveAnimation(startPixel, targetPixel);

        checkForCombat(soldier);
    }
}

void GameManager::calculateMoveableCells(Soldier* soldier)
{
    moveableCells.clear();
    attackableCells.clear();

    if (soldier == nullptr || soldier->getCurrentPoints() <= 0) return;

    // DEÐÝÞÝKLÝK: Harita boyutlarýný en baþta bir kere alalým.
    const sf::Vector2i mapDims = m_map.getDimensions();

    sf::Vector2i startPos = soldier->gridPosition;
    int movePoints = soldier->getCurrentPoints();

    std::vector<sf::Vector2i> reachableEmptyCells;

    // 1. ADIM: Ulaþýlabilir TÜM boþ hücreleri bul ve listeye ekle.
    for (int x = -movePoints; x <= movePoints; ++x)
    {
        for (int y = -movePoints; y <= movePoints; ++y)
        {
            if (abs(x) + abs(y) <= movePoints && (x != 0 || y != 0))
            {
                sf::Vector2i targetPos = { startPos.x + x, startPos.y + y };

                // DEÐÝÞÝKLÝK: Harita sýnýrlarýný kontrol ederken mapDims'i kullan.
                if (targetPos.x >= 0 && targetPos.x < mapDims.x &&
                    targetPos.y >= 0 && targetPos.y < mapDims.y &&
                    getSoldierAt(targetPos) == nullptr)
                {
                    reachableEmptyCells.push_back(targetPos);
                }
            }
        }
    }

    // 2. ADIM: Bu boþ hücreleri yeþil (hareket) ve kýrmýzý (saldýrý) olarak ayýr.

    for (const auto& cell : reachableEmptyCells)
    {
        bool isAttackCell = false;

        // Hücrenin 4 komþusunu (üst, alt, sað, sol) kontrol et.
        const sf::Vector2i neighbors[] = {

            {cell.x, cell.y + 1}, {cell.x, cell.y - 1},

            {cell.x + 1, cell.y}, {cell.x - 1, cell.y}
        };

        for (const auto& neighborPos : neighbors)
        {
            Soldier* neighborSoldier = getSoldierAt(neighborPos);

            // Eðer o komþuda bir asker varsa VE o asker bir düþmansa...

            if (neighborSoldier != nullptr && neighborSoldier->owner != soldier->owner)
            {
                isAttackCell = true;

                break; // Bir tane düþman bulmak, o hücreyi kýrmýzý yapmak için yeterli.
            }
        }

        if (isAttackCell)
        {
            attackableCells.push_back(cell); // Bu bir saldýrý hücresi (kýrmýzý).
        }

        else
        {
            moveableCells.push_back(cell); // Bu normal bir hareket hücresi (yeþil).
        }
    }
}

Soldier* GameManager::getSoldierAt(sf::Vector2i position) const
{
    // DEÐÝÞÝKLÝK: Harita boyutlarýný doðrudan Map nesnesinden al.
    const sf::Vector2i mapDims = m_map.getDimensions();

    // Harita sýnýrlarý içinde mi diye kontrol et.
    if (position.x < 0 || position.x >= mapDims.x || position.y < 0 || position.y >= mapDims.y)
    {
        return nullptr;
    }

    // ... (geri kalan kod ayný, o zaten doðru çalýþýyor)
    for (const auto& owner : owners)
    {
        for (auto& soldier : owner->soldiers)
        {
            if (soldier.gridPosition == position)
            {
                return &soldier;
            }
        }
    }

    return nullptr;
}

void GameManager::endTurn()
{
    if (selectedSoldier != nullptr)
    {
        selectedSoldier->toggleSelection();
        selectedSoldier = nullptr;
    }
    moveableCells.clear();

    currentPlayerIndex = (currentPlayerIndex + 1) % owners.size();

    Owner* newPlayer = owners[currentPlayerIndex].get();
    for (auto& soldier : newPlayer->soldiers)
    {
        soldier.resetMovementPoints();

        checkForCombat(&soldier);
    }
    uiManager.updateGameUI(selectedSoldier, owners[currentPlayerIndex]->name);

    std::cout << "--- New Turn: " << newPlayer->name << " ---\n";

	// AI turn
    if (dynamic_cast<AIOwner*>(newPlayer) != nullptr)
    {
		SetPlayerTurn(false);
        currentGameState = GameState::AI_THINKING;
        uiManager.setEndTurnButtonActive(false); 
        processAITurn(); 
    }

    // Human turn
	else 
    {
        SetPlayerTurn(true);
        currentGameState = GameState::PLAYER_INPUT;
        uiManager.setEndTurnButtonActive(true);
    }
}

bool GameManager::checkForCombat(Soldier* movedSoldier)
{
    if (movedSoldier == nullptr) return false;

    sf::Vector2i pos = movedSoldier->gridPosition;

    // Hareket edilen yerin 4 komþusunu kontrol et.
    const sf::Vector2i neighbors[] = {
        {pos.x, pos.y - 1}, // Üst
        {pos.x, pos.y + 1}, // Alt
        {pos.x - 1, pos.y}, // Sol
        {pos.x + 1, pos.y}  // Sað
    };

    for (const auto& neighborPos : neighbors)
    {
        Soldier* neighborSoldier = getSoldierAt(neighborPos);

        if (neighborSoldier != nullptr && neighborSoldier->owner != movedSoldier->owner)
        {
            std::cout << "COMBAT! " << movedSoldier->owner->name << " vs " << neighborSoldier->owner->name << "\n";
            return  combatManager.resolveCombat(*movedSoldier, *neighborSoldier, selectedSoldier);
        }
    }
    return false;
}

void GameManager::processAITurn()
{
    std::cout << "--- AI turn started ---\n";
    currentGameState = GameState::AI_THINKING;
}