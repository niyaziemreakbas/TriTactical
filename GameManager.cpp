#include "GameManager.h"
#include "Owner.h"
#include <random> // better random number generation than rand().
#include "Soldier.h"
#include <iostream>

GameManager::GameManager(unsigned int windowWidth, unsigned int windowHeight)
{
    generateMap(windowWidth, windowHeight);
	createInitialUnits();
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
    float totalMapWidth = mapWidth * tileSize;
    float totalMapHeight = mapHeight * tileSize;

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

    // 2. Askerleri do�rudan ilgili sahibin 'soldiers' vekt�r�ne ekle.

    owners[0]->soldiers.emplace_back(owners[0].get(), Soldier::Type::Square, sf::Vector2i(1, 1));

    owners[1]->soldiers.emplace_back(owners[1].get(), Soldier::Type::Triangle, sf::Vector2i(mapWidth - 2, mapHeight - 2));

    uiManager.update(nullptr, owners[currentPlayerIndex]->name);
}

void GameManager::handleClick(int mouseX, int mouseY)
{
    if (uiManager.isEndTurnButtonClicked({ mouseX, mouseY }))
    {
        endTurn();
        return; // Butona t�kland�ysa ba�ka bir i�lem yapma.
    }

    int gridX = static_cast<int>((mouseX - mapOffsetX) / tileSize);
    int gridY = static_cast<int>((mouseY - mapOffsetY) / tileSize);

    // 1. T�klanan h�crede bir asker var m� diye bul (de�i�iklik yok).
    Soldier* clickedSoldier = nullptr;
    if (gridX >= 0 && gridX < mapWidth && gridY >= 0 && gridY < mapHeight)
    {
        for (auto& owner : owners)
        {
            for (auto& soldier : owner->soldiers)
            {
                if (soldier.gridPosition.x == gridX && soldier.gridPosition.y == gridY)
                {
                    clickedSoldier = &soldier;
                    break;
                }
            }
            if (clickedSoldier) break;
        }
    }

    // Durum A: Bo� bir h�creye t�kland�.
    if (clickedSoldier == nullptr)
    {
        // A.1: E�er bir asker se�iliyse, bu bir hareket emri olabilir mi?
        if (selectedSoldier != nullptr)
        {
            sf::Vector2i clickedCell(gridX, gridY);

            // T�klanan h�cre, hareket edilebilir h�creler listesinde var m� diye kontrol et.
            auto it = std::find(moveableCells.begin(), moveableCells.end(), clickedCell);

            // E�er listede bulunduysa...
            if (it != moveableCells.end())
            {
                // HAREKET ET!
                sf::Vector2i startPos = selectedSoldier->gridPosition;
                int distance = abs(clickedCell.x - startPos.x) + abs(clickedCell.y - startPos.y);

                selectedSoldier->setCurrentPoints(-distance);

                // Askerin pozisyonunu g�ncelle.
                selectedSoldier->gridPosition = clickedCell;

                // Hareket sonras� se�imi ve ESK� hareket alan�n� temizle.
                selectedSoldier->toggleSelection();
                moveableCells.clear();

                // YEN� hareket alan�n� askerin KALAN puanlar�na g�re tekrar hesapla.
                calculateMoveableCells(selectedSoldier);

                // Askere tekrar t�kland���nda se�ilebilmesi i�in se�imi tekrar a� ve kapat.
                selectedSoldier->toggleSelection();
            }
            // A.2: Ge�erli bir hareket de�ilse, sadece se�imi kald�r.
            else
            {
                selectedSoldier->toggleSelection();
                selectedSoldier = nullptr;
                calculateMoveableCells(nullptr);
            }
        }
        // A.3: Zaten se�ili bir asker yoksa hi�bir �ey yapma.
    }

    // Durum B: Bir askerin oldu�u h�creye t�kland�. (Bu standart se�im mant���)
    else
    {
        if (clickedSoldier->owner == owners[currentPlayerIndex].get()) {
            // E�er se�ili olan askerden farkl� bir askere t�kland�ysa, eskisinin se�imini kald�r.
            if (selectedSoldier != nullptr && selectedSoldier != clickedSoldier)
            {
                selectedSoldier->toggleSelection();
            }

            // T�klanan askerin se�imini a�/kapat.
            clickedSoldier->toggleSelection();

            // Yeni se�ili askeri g�ncelle.
            if (clickedSoldier->getSelectState()) // E�er se�ili hale geldiyse
            {
                selectedSoldier = clickedSoldier;
            }
            else // E�er se�imi kald�r�ld�ysa
            {
                selectedSoldier = nullptr;
            }

            // Yeni duruma g�re hareket alan�n� hesapla.
            calculateMoveableCells(selectedSoldier);
        }
    }

    uiManager.update(selectedSoldier, owners[currentPlayerIndex]->name);
}

void GameManager::draw(sf::RenderWindow& window)
{
    for (const auto& cellPos : moveableCells)
    {
        map[cellPos.x][cellPos.y].shape.setFillColor(sf::Color(144, 238, 144)); // A��k ye�il
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
        // Sonra o sahibin askerlerinin (soldier) listesini dola� ve �iz.
        for (auto& soldier : owner->soldiers)
        {
            // Number 3 fixes the problem which is coming from the outline 
            soldier.draw(window, tileSize, mapOffsetX - 1.5, mapOffsetY - 1.5);
        }
    }
}

void GameManager::calculateMoveableCells(Soldier* soldier)
{
    // �nceki hesaplamadan kalanlar� temizle.
    moveableCells.clear();

    if (soldier == nullptr) return;

    sf::Vector2i startPos = soldier->gridPosition;
    int movePoints = soldier->getCurrentPoints();

    // Bu basit bir "4 y�ne yay�lma" algoritmas�d�r. �imdilik engelleri yok say�yoruz.
    // Daha karma��k algoritmalar (A*, Dijkstra) daha sonra eklenebilir.
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
    // 1. Mevcut se�imleri temizle.
    if (selectedSoldier != nullptr)
    {
        selectedSoldier->toggleSelection();
        selectedSoldier = nullptr;
    }
    moveableCells.clear();
    //uiManager.update(nullptr);

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

void GameManager::startGame()
{
    // I will use this later.
}