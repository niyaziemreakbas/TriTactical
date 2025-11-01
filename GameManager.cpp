#include "GameManager.h"
#include "Owner.h"
#include <random> // better random number generation than rand().
#include "Soldier.h"
#include <iostream>
#include <string>
#include "AIOwner.h"

GameManager::GameManager(unsigned int windowWidth, unsigned int windowHeight)
    : m_map(windowWidth, windowHeight),
    uiManager()
{
    createInitialUnits();
}

void GameManager::createInitialUnits()
{
    // Sahipleri (Player ve AI) oluþtur
    owners.push_back(std::make_unique<PlayerOwner>("Player", sf::Color::Blue));
    owners.push_back(std::make_unique<AIOwner>("AI 1", sf::Color::Red));

    sf::Vector2i mapDims = m_map.getDimensions();

    // --- Oyuncu (Mavi) için 3 farklý asker oluþtur ---
    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Triangle, sf::Vector2i(1, 1));
    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Circle, sf::Vector2i(1, 2));
    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Square, sf::Vector2i(2, 1));

    // --- Yapay Zeka (Kýrmýzý) için 3 farklý asker oluþtur ---
    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Square, sf::Vector2i(mapDims.x - 2, mapDims.y - 2));
    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Circle, sf::Vector2i(mapDims.x - 2, mapDims.y - 3));
    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Triangle, sf::Vector2i(mapDims.x - 3, mapDims.y - 2));

    // UI'ý baþlangýç durumu için ayarla
    uiManager.setEndTurnButtonActive(true);
    uiManager.update(nullptr, owners[currentPlayerIndex]->name);
}

void GameManager::update(float dt)
{
    // Önce, oyundaki TÜM askerlerin animasyonunu her zaman ilerlet.
    for (auto& owner : owners) {
        for (auto& soldier : owner->soldiers) {
            soldier.update(dt);
        }
    }

    // Þimdi, oyunun durumuna göre karar ver.
    switch (currentGameState)
    {
    case GameState::PLAYER_INPUT:
        // Bu durumda hiçbir þey yapma, oyuncunun týklamasýný bekle.
        break;

    case GameState::AI_THINKING:
    {
        // YZ'nin düþünme zamaný!
        AIOwner* aiOwner = dynamic_cast<AIOwner*>(owners[currentPlayerIndex].get());
        if (aiOwner) {
            aiOwner->processTurn(*this); // YZ tüm kararlarýný anýnda verir.
        }
        // YZ düþündükten sonra, animasyonlarýn oynamasý için oyunu ANIMATING moduna al.
        //currentGameState = GameState::ANIMATING;
        break;
    }

    case GameState::ANIMATING:
    {
        // Animasyonlarýn bitip bitmediðini kontrol et.
        bool isAnyAnimationRunning = false;
        for (const auto& owner : owners) {
            for (const auto& soldier : owner->soldiers) {
                if (soldier.IsAnimating()) {
                    isAnyAnimationRunning = true;
                    break;
                }
            }
            if (isAnyAnimationRunning) break;
        }

        // Eðer tüm animasyonlar bittiyse...
        if (!isAnyAnimationRunning) {
            std::cout << "All animations finished.\n";
            // Turu bitirerek sýrayý bir sonraki oyuncuya devret.
            //endTurn();
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
                // (Seçimi kaldýrmak, renkleri temizlemek vb.)
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
    uiManager.update(selectedSoldier, owners[currentPlayerIndex]->name);
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

    uiManager.draw(window);
}

std::string soldierTypeToString(Soldier::Type type)
{
    switch (type)
    {
    case Soldier::Type::Square:   return "Square";
    case Soldier::Type::Circle:   return "Circle";
    case Soldier::Type::Triangle: return "Triangle";
    default:                      return "Unknown";
    }
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

    // 1. Piksel pozisyonlarýný hesapla.
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

    // 2. MANTIKSAL hareketi anýnda yap.
    // (Bu kýsmý Soldier::moveTo'ya zaten taþýmýþtýk, onu kullanalým!)
    if (soldier->moveTo(targetCell))
    {
        // 3. GÖRSEL animasyonu baþlat.
        soldier->startMoveAnimation(startPixel, targetPixel);

        // 4. Oyunu animasyon moduna kilitle.
        //currentGameState = GameState::ANIMATING;
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
    // 1. Mevcut seçimleri temizle.
    if (selectedSoldier != nullptr)
    {
        selectedSoldier->toggleSelection();
        selectedSoldier = nullptr;
    }
    moveableCells.clear();

    // 2. Sýradaki oyuncuya geç. Modulo (%) operatörü, listenin sonuna gelince baþa dönmemizi saðlar.
    currentPlayerIndex = (currentPlayerIndex + 1) % owners.size();

    // 3. YENÝ oyuncunun TÜM askerlerinin hareket puanlarýný yenile.
    Owner* newPlayer = owners[currentPlayerIndex].get();
    for (auto& soldier : newPlayer->soldiers)
    {
        soldier.resetMovementPoints();
    }
    uiManager.update(selectedSoldier, owners[currentPlayerIndex]->name);

    // (Opsiyonel) Konsola kimin sýrasý geldiðini yazdýr.
    std::cout << "--- New Turn: " << newPlayer->name << " ---\n";

    if (dynamic_cast<AIOwner*>(newPlayer) != nullptr)
    {
		SetPlayerTurn(false);
        currentGameState = GameState::AI_THINKING;
        uiManager.setEndTurnButtonActive(false); // Butonu pasif yap ve yazýyý deðiþtir.
        processAITurn(); 
    }
    else // Eðer yeni oyuncu insansa
    {
        SetPlayerTurn(true);
        currentGameState = GameState::PLAYER_INPUT;
        uiManager.setEndTurnButtonActive(true); // Butonu tekrar aktif yap.
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

        // Eðer komþuda bir asker varsa VE bu asker bir düþmansa...
        if (neighborSoldier != nullptr && neighborSoldier->owner != movedSoldier->owner)
        {
            std::cout << "COMBAT! " << movedSoldier->owner->name << " vs " << neighborSoldier->owner->name << "\n";
            return  resolveCombat(*movedSoldier, *neighborSoldier);
        }
    }
    return false;
}

bool GameManager::resolveCombat(Soldier& attacker, Soldier& defender)
{
    Soldier::Type attType = attacker.type;
    Soldier::Type defType = defender.type;

    Soldier* winner = nullptr;
    Soldier* loser = nullptr;

    bool attackResult;

    // Triangle > Circle > Square > Triangle
    if ((attType == Soldier::Type::Triangle && defType == Soldier::Type::Circle) ||
        (attType == Soldier::Type::Circle && defType == Soldier::Type::Square) ||
        (attType == Soldier::Type::Square && defType == Soldier::Type::Triangle))
    {
        winner = &attacker;
        loser = &defender;
        attackResult = true;
    }
    else if ((defType == Soldier::Type::Triangle && attType == Soldier::Type::Circle) ||
        (defType == Soldier::Type::Circle && attType == Soldier::Type::Square) ||
        (defType == Soldier::Type::Square && attType == Soldier::Type::Triangle))
    {
        winner = &defender;
        loser = &attacker;
        attackResult = false;
    }
    else 
    {
        std::cout << "Combat is a draw. " << soldierTypeToString(attType) << " " << soldierTypeToString(defType) << " No change.\n";
        return false;
    }

    std::cout << winner->owner->name << " wins the battle!\n";

    //Owner* oldOwner = loser->owner;
    Owner* newOwner = winner->owner;

    // 1. Kaybeden askerin sahibini ve rengini deðiþtir.
    loser->setOwner(newOwner);

	loser->setCurrentPoints(-loser->getCurrentPoints());
    winner->setCurrentPoints(-winner->getCurrentPoints());

    return attackResult;
}

// Ve iþte her þeyi birleþtiren ana YZ fonksiyonu!
void GameManager::processAITurn()
{
    std::cout << "--- AI is thinking... ---\n";
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Sýrasý gelen YZ'yi bul.
    AIOwner* aiOwner = dynamic_cast<AIOwner*>(owners[currentPlayerIndex].get());
    if (aiOwner)
    {
        // YZ'ye dünyayý göster ve oynamasýný söyle.
        aiOwner->processTurn(*this);
    }

    std::cout << "AI finished its turn.\n";
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    endTurn();
}