#include "GameManager.h"
#include "Owner.h"
#include <random> // better random number generation than rand().
#include "Soldier.h"
#include <iostream>
#include <string>

GameManager::GameManager(unsigned int windowWidth, unsigned int windowHeight)
{
    generateMap(windowWidth, windowHeight);
	createInitialUnits();
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

void GameManager::generateMap(unsigned int windowWidth, unsigned int windowHeight)
{
    //Randomly Selecting Map Size
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribWidth(5, 15);
    std::uniform_int_distribution<> distribHeight(5, 15);

    mapWidth = distribWidth(gen);
    mapHeight = distribHeight(gen);

	// Centering Map Logic
    totalMapWidth = mapWidth * tileSize;
    totalMapHeight = mapHeight * tileSize;

    mapOffsetX = (windowWidth - totalMapWidth) / 2.0f;
    mapOffsetY = (windowHeight - totalMapHeight) / 2.0f;

    map.resize(mapWidth);
    for (int i = 0; i < mapWidth; ++i)
    {
        map[i].resize(mapHeight);
    }

    for (int x = 0; x < mapWidth; ++x)
    {
        for (int y = 0; y < mapHeight; ++y)
        {
            map[x][y].shape.setSize(sf::Vector2f(tileSize, tileSize));

            // 3. Apply the offset when setting the position of each tile.
            map[x][y].shape.setPosition(sf::Vector2f(mapOffsetX + (x * tileSize), mapOffsetY + (y * tileSize)));

            map[x][y].shape.setFillColor(sf::Color::White);
            map[x][y].shape.setOutlineThickness(3.0f);
            map[x][y].shape.setOutlineColor(sf::Color(200, 200, 200));
        }
    }
}

void GameManager::createInitialUnits()
{
    owners.push_back(std::make_unique<PlayerOwner>("Player", sf::Color::Blue));
    owners.push_back(std::make_unique<AIOwner>("AI 1", sf::Color::Red));

    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Square, sf::Vector2i(1, 1));

    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Triangle, sf::Vector2i(mapWidth - 2, mapHeight - 2));

    uiManager.update(nullptr, owners[currentPlayerIndex]->name);
}

void GameManager::handleClick(int mouseX, int mouseY)
{
    if (uiManager.isEndTurnButtonClicked({ mouseX, mouseY }))
    {
        endTurn();
        return; // Butona týklandýysa baþka bir iþlem yapma.
    }

    int gridX = static_cast<int>((mouseX - mapOffsetX) / tileSize);
    int gridY = static_cast<int>((mouseY - mapOffsetY) / tileSize);

    Soldier* clickedSoldier = getSoldierAt({ gridX, gridY });

    // Durum A: Boþ bir hücreye týklandý.
    if (clickedSoldier == nullptr)
    {
        // A.1: Eðer bir asker seçiliyse, bu bir hareket emri olabilir mi?
        if (selectedSoldier != nullptr)
        {
            sf::Vector2i clickedCell(gridX, gridY);

            // Týklanan hücre, hareket edilebilir hücreler listesinde var mý diye kontrol et.
            auto it = std::find(moveableCells.begin(), moveableCells.end(), clickedCell);

            // Eðer listede bulunduysa...
            if (it != moveableCells.end())
            {
                // HAREKET ET!
                sf::Vector2i startPos = selectedSoldier->gridPosition;
                int distance = abs(clickedCell.x - startPos.x) + abs(clickedCell.y - startPos.y);

                selectedSoldier->setCurrentPoints(-distance);

                // Askerin pozisyonunu güncelle.
                selectedSoldier->gridPosition = clickedCell;

                checkForCombat(selectedSoldier);

                // Hareket sonrasý seçimi ve ESKÝ hareket alanýný temizle.
                selectedSoldier->toggleSelection();
                moveableCells.clear();

                // YENÝ hareket alanýný askerin KALAN puanlarýna göre tekrar hesapla.
                calculateMoveableCells(selectedSoldier);

                // Askere tekrar týklandýðýnda seçilebilmesi için seçimi tekrar aç ve kapat.
                selectedSoldier->toggleSelection();
            }
            // A.2: Geçerli bir hareket deðilse, sadece seçimi kaldýr.
            else
            {
                selectedSoldier->toggleSelection();
                selectedSoldier = nullptr;
                calculateMoveableCells(nullptr);
            }
        }
        // A.3: Zaten seçili bir asker yoksa hiçbir þey yapma.
    }

    // Durum B: Bir askerin olduðu hücreye týklandý. (Bu standart seçim mantýðý)
    else
    {
        if (clickedSoldier->owner == owners[currentPlayerIndex].get()) {
            // Eðer seçili olan askerden farklý bir askere týklandýysa, eskisinin seçimini kaldýr.
            if (selectedSoldier != nullptr && selectedSoldier != clickedSoldier)
            {
                selectedSoldier->toggleSelection();
            }

            // Týklanan askerin seçimini aç/kapat.
            clickedSoldier->toggleSelection();

            // Yeni seçili askeri güncelle.
            if (clickedSoldier->getSelectState()) // Eðer seçili hale geldiyse
            {
                selectedSoldier = clickedSoldier;
            }
            else // Eðer seçimi kaldýrýldýysa
            {
                selectedSoldier = nullptr;
            }

            // Yeni duruma göre hareket alanýný hesapla.
            calculateMoveableCells(selectedSoldier);
            uiManager.update(selectedSoldier, owners[currentPlayerIndex]->name);

        }
    }
}

void GameManager::draw(sf::RenderWindow& window)
{
    for (const auto& cellPos : moveableCells)
    {
        map[cellPos.x][cellPos.y].shape.setFillColor(sf::Color(144, 238, 144)); // Açýk yeþil
    }

    // Loop through the entire map and draw each cell's shape.
    for (int x = 0; x < mapWidth; ++x)
    {
        for (int y = 0; y < mapHeight; ++y)
        {
            window.draw(map[x][y].shape);
        }
    }

    for (const auto& cellPos : moveableCells)
    {
        map[cellPos.x][cellPos.y].shape.setFillColor(sf::Color::White);
    }

    for (auto& owner : owners)
    {
        // Sonra o sahibin askerlerinin (soldier) listesini dolaþ ve çiz.
        for (auto& soldier : owner->soldiers)
        {
            // Number 3 fixes the problem which is coming from the outline 
            soldier.draw(window, tileSize, mapOffsetX - 1.5, mapOffsetY - 1.5);
        }
    }
    uiManager.draw(window);
}

void GameManager::calculateMoveableCells(Soldier* soldier)
{
    // Önceki hesaplamadan kalanlarý temizle.
    moveableCells.clear();

    if (soldier == nullptr) return;

    sf::Vector2i startPos = soldier->gridPosition;
    int movePoints = soldier->getCurrentPoints();

    // Bu basit bir "4 yöne yayýlma" algoritmasýdýr. Þimdilik engelleri yok sayýyoruz.
    // Daha karmaþýk algoritmalar (A*, Dijkstra) daha sonra eklenebilir.
    for (int x = -movePoints; x <= movePoints; ++x)
    {
        for (int y = -movePoints; y <= movePoints; ++y)
        {
			//&& abs(x) + abs(y) != 0 if you want to exclude the starting cell
            if (abs(x) + abs(y) <= movePoints)
            {
                int targetX = startPos.x + x;
                int targetY = startPos.y + y;

                if (targetX >= 0 && targetX < mapWidth && targetY >= 0 && targetY < mapHeight)
                {
                    moveableCells.push_back(sf::Vector2i(targetX, targetY));
                }
            }
        }
    }
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
    //uiManager.update(nullptr);

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
}

Soldier* GameManager::getSoldierAt(sf::Vector2i position) const
{
    // Harita sýnýrlarý içinde mi diye kontrol et.
    if (position.x < 0 || position.x >= mapWidth || position.y < 0 || position.y >= mapHeight)
    {
        return nullptr;
    }

    // Tüm askerleri tara.
    for (const auto& owner : owners) // 'const' fonksiyonda olduðumuz için const referans kullanmak iyidir.
    {
        for (auto& soldier : owner->soldiers) // Burasý const olamaz çünkü Soldier* döndürüyoruz.
        {
            if (soldier.gridPosition == position)
            {
                return &soldier; // Askeri bulduk, adresini döndür.
            }
        }
    }

    return nullptr;
}

void GameManager::checkForCombat(Soldier* movedSoldier)
{
    if (movedSoldier == nullptr) return;

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
            resolveCombat(*movedSoldier, *neighborSoldier);
            // Þimdilik ilk bulduðu düþmanla savaþýr. Daha sonra çoklu savaþ düþünülebilir.
            return;
        }
    }
}

void GameManager::resolveCombat(Soldier& attacker, Soldier& defender)
{
    Soldier::Type attType = attacker.type;
    Soldier::Type defType = defender.type;

    Soldier* winner = nullptr;
    Soldier* loser = nullptr;

    // Savaþ Kuralý: Triangle > Circle > Square > Triangle
    if ((attType == Soldier::Type::Triangle && defType == Soldier::Type::Circle) ||
        (attType == Soldier::Type::Circle && defType == Soldier::Type::Square) ||
        (attType == Soldier::Type::Square && defType == Soldier::Type::Triangle))
    {
        winner = &attacker;
        loser = &defender;
    }
    else // Beraberlik durumu veya kural dýþý bir durum
    {

        // Þimdilik beraberlikte hiçbir þey olmasýn.
        std::cout << "Combat is a draw. " << soldierTypeToString(attType) << " " << soldierTypeToString(defType) << " No change.\n";
        return;
    }

    std::cout << winner->owner->name << " wins the battle!\n";

    Owner* oldOwner = loser->owner;
    Owner* newOwner = winner->owner;

    // 1. Kaybeden askerin sahibini ve rengini deðiþtir.
    //loser->setOwner(newOwner);

    // 1. Taþýnacak askeri eski sahibinin listesinde bul.
    auto& oldSoldierList = oldOwner->soldiers;
    auto it = std::find_if(oldSoldierList.begin(), oldSoldierList.end(),
        [loser](const Soldier& s) { return &s == loser; });

    if (it != oldSoldierList.end())
    {
        // 2. Askeri yeni sahibin listesine taþý. Bu, bir kopya oluþturur.
        auto& newSoldierList = newOwner->soldiers;
        newSoldierList.push_back(*it);

        // 3. Eski listeden sil.
        oldSoldierList.erase(it);

        // 4. ÖNEMLÝ: Artýk YENÝ LÝSTEDEKÝ kopyanýn sahibini güncelle.
        // newSoldierList.back() listenin son elemanýný verir.
        newSoldierList.back().setOwner(newOwner);
    }
}

void GameManager::startGame()
{
    // I will use this later.
}