#include "GameManager.h"
#include "Owner.h"
#include <random> // better random number generation than rand().
#include "Soldier.h"
#include <iostream>
#include <string>

GameManager::GameManager(unsigned int windowWidth, unsigned int windowHeight)
    : m_map(windowWidth, windowHeight),
    uiManager()
{
    createInitialUnits();
}

void GameManager::createInitialUnits()
{
    owners.push_back(std::make_unique<PlayerOwner>("Player", sf::Color::Blue));
    owners.push_back(std::make_unique<AIOwner>("AI 1", sf::Color::Red));

    sf::Vector2i mapDims = m_map.getDimensions();

    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Square, sf::Vector2i(1, 1));
    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Triangle, sf::Vector2i(mapDims.x - 2, mapDims.y - 2));

    uiManager.update(nullptr, owners[currentPlayerIndex]->name);
}

void GameManager::handleClick(int mouseX, int mouseY)
{
    if (uiManager.isEndTurnButtonClicked({ mouseX, mouseY }))
    {
        endTurn();
        return;
    }

    const sf::Vector2f mapOffset = m_map.getMapOffset();
    const float tileSize = m_map.getTileSize();
    const sf::Vector2i mapDims = m_map.getDimensions();

    // �imdi bu do�ru bilgilerle koordinatlar� hesapla.
    int gridX = static_cast<int>((mouseX - mapOffset.x) / tileSize);
    int gridY = static_cast<int>((mouseY - mapOffset.y) / tileSize);

    Soldier* clickedSoldier = getSoldierAt({ gridX, gridY });

    // Durum A: Bo� bir h�creye t�kland�.
    if (clickedSoldier == nullptr)
    {
        if (selectedSoldier != nullptr)
        {
            sf::Vector2i clickedCell(gridX, gridY);

            bool canMove = std::find(moveableCells.begin(), moveableCells.end(), clickedCell) != moveableCells.end();
            bool canAttackMove = std::find(attackableCells.begin(), attackableCells.end(), clickedCell) != attackableCells.end();

            if (canMove || canAttackMove)
            {
                bool didAttackerSurvive = false; // �nce tan�mla

                // Hareketi yap
                int distance = abs(clickedCell.x - selectedSoldier->gridPosition.x) + abs(clickedCell.y - selectedSoldier->gridPosition.y);
                
				selectedSoldier->gridPosition = clickedCell;

                selectedSoldier->setCurrentPoints(-distance);

                didAttackerSurvive = checkForCombat(selectedSoldier);

                selectedSoldier->toggleSelection();

                if (!didAttackerSurvive)
                {
                    selectedSoldier = nullptr;
                    calculateMoveableCells(nullptr);
                }
                else
                {
                    calculateMoveableCells(selectedSoldier);
                }
            }
            else
            {
                selectedSoldier->toggleSelection();

                selectedSoldier = nullptr;
                calculateMoveableCells(nullptr);
            }
        }
    }
    // Durum B: Bir askere t�kland�.
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
    // Renklendirme i�ini Map s�n�f�na devret
    for (const auto& cellPos : moveableCells) {
        m_map.colorCell(cellPos, sf::Color(144, 238, 144));
    }
    for (const auto& cellPos : attackableCells) {
        m_map.colorCell(cellPos, sf::Color(250, 128, 114));
    }

    // Haritay� �iz
    m_map.draw(window);

    // Renkleri s�f�rla
    for (const auto& cellPos : moveableCells) {
        m_map.resetCellColor(cellPos);
    }
    for (const auto& cellPos : attackableCells) {
        m_map.resetCellColor(cellPos);
    }

    // Askerleri �iz (offset ve tileSize'� haritadan al)
    sf::Vector2f mapOffset = m_map.getMapOffset();
    float tileSize = m_map.getTileSize();
    for (auto& owner : owners) {
        for (auto& soldier : owner->soldiers) {
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

void GameManager::calculateMoveableCells(Soldier* soldier)
{
    moveableCells.clear();
    attackableCells.clear();

    if (soldier == nullptr || soldier->getCurrentPoints() <= 0) return;

    // DE����KL�K: Harita boyutlar�n� en ba�ta bir kere alal�m.
    const sf::Vector2i mapDims = m_map.getDimensions();

    sf::Vector2i startPos = soldier->gridPosition;
    int movePoints = soldier->getCurrentPoints();

    std::vector<sf::Vector2i> reachableEmptyCells;

    // 1. ADIM: Ula��labilir T�M bo� h�creleri bul ve listeye ekle.
    for (int x = -movePoints; x <= movePoints; ++x)
    {
        for (int y = -movePoints; y <= movePoints; ++y)
        {
            if (abs(x) + abs(y) <= movePoints && (x != 0 || y != 0))
            {
                sf::Vector2i targetPos = { startPos.x + x, startPos.y + y };

                // DE����KL�K: Harita s�n�rlar�n� kontrol ederken mapDims'i kullan.
                if (targetPos.x >= 0 && targetPos.x < mapDims.x &&
                    targetPos.y >= 0 && targetPos.y < mapDims.y &&
                    getSoldierAt(targetPos) == nullptr)
                {
                    reachableEmptyCells.push_back(targetPos);
                }
            }
        }
    }

    // 2. ADIM: Bu bo� h�creleri ye�il (hareket) ve k�rm�z� (sald�r�) olarak ay�r.

    for (const auto& cell : reachableEmptyCells)
    {
        bool isAttackCell = false;

        // H�crenin 4 kom�usunu (�st, alt, sa�, sol) kontrol et.
        const sf::Vector2i neighbors[] = {

            {cell.x, cell.y + 1}, {cell.x, cell.y - 1},

            {cell.x + 1, cell.y}, {cell.x - 1, cell.y}
        };

        for (const auto& neighborPos : neighbors)
        {
            Soldier* neighborSoldier = getSoldierAt(neighborPos);

            // E�er o kom�uda bir asker varsa VE o asker bir d��mansa...

            if (neighborSoldier != nullptr && neighborSoldier->owner != soldier->owner)
            {
                isAttackCell = true;

                break; // Bir tane d��man bulmak, o h�creyi k�rm�z� yapmak i�in yeterli.
            }
        }

        if (isAttackCell)
        {
            attackableCells.push_back(cell); // Bu bir sald�r� h�cresi (k�rm�z�).
        }

        else
        {
            moveableCells.push_back(cell); // Bu normal bir hareket h�cresi (ye�il).
        }
    }
}

Soldier* GameManager::getSoldierAt(sf::Vector2i position) const
{
    // DE����KL�K: Harita boyutlar�n� do�rudan Map nesnesinden al.
    const sf::Vector2i mapDims = m_map.getDimensions();

    // Harita s�n�rlar� i�inde mi diye kontrol et.
    if (position.x < 0 || position.x >= mapDims.x || position.y < 0 || position.y >= mapDims.y)
    {
        return nullptr;
    }

    // ... (geri kalan kod ayn�, o zaten do�ru �al���yor)
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
    // 1. Mevcut se�imleri temizle.
    if (selectedSoldier != nullptr)
    {
        //selectedSoldier->toggleSelection();
        selectedSoldier = nullptr;
    }
    moveableCells.clear();

    // 2. S�radaki oyuncuya ge�. Modulo (%) operat�r�, listenin sonuna gelince ba�a d�nmemizi sa�lar.
    currentPlayerIndex = (currentPlayerIndex + 1) % owners.size();

    // 3. YEN� oyuncunun T�M askerlerinin hareket puanlar�n� yenile.
    Owner* newPlayer = owners[currentPlayerIndex].get();
    for (auto& soldier : newPlayer->soldiers)
    {
        soldier.resetMovementPoints();
    }
    uiManager.update(selectedSoldier, owners[currentPlayerIndex]->name);

    // (Opsiyonel) Konsola kimin s�ras� geldi�ini yazd�r.
    std::cout << "--- New Turn: " << newPlayer->name << " ---\n";
}

bool GameManager::checkForCombat(Soldier* movedSoldier)
{
    if (movedSoldier == nullptr) return false;

    sf::Vector2i pos = movedSoldier->gridPosition;

    // Hareket edilen yerin 4 kom�usunu kontrol et.
    const sf::Vector2i neighbors[] = {
        {pos.x, pos.y - 1}, // �st
        {pos.x, pos.y + 1}, // Alt
        {pos.x - 1, pos.y}, // Sol
        {pos.x + 1, pos.y}  // Sa�
    };

    for (const auto& neighborPos : neighbors)
    {
        Soldier* neighborSoldier = getSoldierAt(neighborPos);

        // E�er kom�uda bir asker varsa VE bu asker bir d��mansa...
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

    // 1. Kaybeden askerin sahibini ve rengini de�i�tir.
    loser->setOwner(newOwner);

	loser->setCurrentPoints(-loser->getCurrentPoints());
    winner->setCurrentPoints(-winner->getCurrentPoints());

    return attackResult;
}
