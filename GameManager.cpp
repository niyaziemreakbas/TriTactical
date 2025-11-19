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
    // 1. Pointer'larý güvenli hale getir (Crash olmamasý için kritik)
    selectedSoldier = nullptr;

    // 2. Listeleri temizle (std::vector zaten boþ baþlar ama emin olalým)
    owners.clear();
    moveableCells.clear();
    attackableCells.clear();

    // 3. Sýra deðiþkenini sýfýrla
    currentPlayerIndex = 0;

    // 4. Rastgelelik için seed ayarý (Eðer rand() kullanýlýyorsa diye, mt19937 için gerekmez ama zararý yok)
    std::srand(static_cast<unsigned int>(time(nullptr)));
}

void GameManager::startGame(GameMode mode, int numHumans, int numBots)
{
    std::cout << "Starting game: " << numHumans << " Humans, " << numBots << " Bots.\n";

    m_isGameOver = false;
    m_winnerName = "";

    std::vector<sf::Color> colors = {
        sf::Color::Blue,
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan,
        sf::Color(255, 165, 0), // Turuncu
        sf::Color(128, 0, 128), // Mor
        sf::Color(0, 255, 127)  // Spring Green
    };

    // Renkleri karýþtýr (Shuffle)
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(colors.begin(), colors.end(), g);

    // 1. Temizlik
    owners.clear();
    selectedSoldier = nullptr;
    moveableCells.clear();
    attackableCells.clear();

    // 2. Harita Boyutunu Hesapla (Basit Algoritma)
    // Her oyuncu için yaklaþýk 5x5'lik bir alan ayýralým. Minimum 8x8 olsun.
    int totalPlayers = numHumans + numBots;
    int mapDim = 8 + (totalPlayers * 2); // Örn: 2 kiþi -> 12x12, 4 kiþi -> 16x16

    // Haritayý yeniden oluþtur (Pencere boyutunu constructor'dan saklaman gerekebilir veya sabit 1366x768 varsayabiliriz)
    m_map.regenerate(mapDim, mapDim, 1366, 768);

    int colorIndex = 0;

    for (int i = 0; i < numHumans; ++i) {
        std::string name = "Player " + std::to_string(i + 1);

        // Listeden sýradaki rengi ver
        sf::Color assignedColor = colors[colorIndex % colors.size()];
        colorIndex++;

        owners.push_back(std::make_unique<PlayerOwner>(name, assignedColor));
    }

    // --- BOTLARI OLUÞTUR ---
    for (int i = 0; i < numBots; ++i) {
        std::string name = "Bot " + std::to_string(i + 1);

        // Listeden sýradaki rengi ver
        sf::Color assignedColor = colors[colorIndex % colors.size()];
        colorIndex++;

        owners.push_back(std::make_unique<AIOwner>(name, assignedColor));
    }

    // 4. Askerleri Rastgele Daðýt
    for (auto& owner : owners)
    {
        // Her oyuncuya 1 Üçgen, 1 Daire, 1 Kare verelim
        Soldier::Type types[] = { Soldier::Type::Triangle, Soldier::Type::Circle, Soldier::Type::Square };

        for (auto type : types)
        {
            sf::Vector2i spawnPos = findRandomEmptyCell();
            owner->soldiers.emplace_back(owner.get(), type, spawnPos);
        }
    }

    // 5. Tur Sistemini Baþlat
    currentPlayerIndex = 0;

    // Ýlk tur için savaþ kontrolü ve UI güncellemesi
    for (auto& soldier : owners[currentPlayerIndex]->soldiers) {
        checkForCombat(&soldier);
    }

    // AI kontrolü
    if (dynamic_cast<AIOwner*>(owners[0].get())) {
        currentGameState = GameState::AI_THINKING;
        uiManager.setEndTurnButtonActive(false);
        processAITurn();
    }
    else {
        currentGameState = GameState::PLAYER_INPUT;
        uiManager.setEndTurnButtonActive(true);
    }

    uiManager.updateGameUI(nullptr, owners[currentPlayerIndex]->name);
}

void GameManager::checkWinCondition()
{
    int activeOwners = 0;
    std::string potentialWinner = "";

    for (const auto& owner : owners)
    {
        if (!owner->soldiers.empty())
        {
            activeOwners++;
            potentialWinner = owner->name;
        }
    }

    // Eðer sadece 1 kiþi kaldýysa (veya kimse kalmadýysa) oyun biter
    if (activeOwners <= 1)
    {
        m_isGameOver = true;
        m_winnerName = (activeOwners == 1) ? potentialWinner : "Draw / No Winner";
        std::cout << "GAME OVER! Winner: " << m_winnerName << "\n";
    }
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

sf::Vector2i GameManager::findRandomEmptyCell()
{
    // 1. Boþ hücreleri tutacak bir liste oluþtur
    std::vector<sf::Vector2i> emptyCells;

    sf::Vector2i dims = m_map.getDimensions();

    // 2. Tüm haritayý tara ve boþ olanlarý listeye ekle
    for (int x = 0; x < dims.x; ++x)
    {
        for (int y = 0; y < dims.y; ++y)
        {
            // Eðer bu koordinatta asker yoksa listeye ekle
            if (getSoldierAt({ x, y }) == nullptr)
            {
                emptyCells.push_back({ x, y });
            }
        }
    }

    // 3. Eðer hiç boþ yer yoksa güvenli çýkýþ yap
    if (emptyCells.empty())
    {
        return { 0, 0 }; // Hata durumu veya oyun sonu
    }

    // 4. Listeden rastgele bir indeks seç
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, emptyCells.size() - 1);

    return emptyCells[distrib(gen)];
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
    checkWinCondition();
    if (m_isGameOver) return;

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
		//SetPlayerTurn(false);
        currentGameState = GameState::AI_THINKING;
        uiManager.setEndTurnButtonActive(false); 
        processAITurn(); 
    }

    // Human turn
	else 
    {
        //SetPlayerTurn(true);
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