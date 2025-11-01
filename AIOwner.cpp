#include "AIOwner.h"
#include <random>
#include <iostream>

AIOwner::AIOwner(std::string p_name, sf::Color p_color)
{
    name = p_name;
    color = p_color;
}

Soldier* AIOwner::findClosestSoldier(Soldier& from, const std::vector<Soldier*>& targets)
{
    if (targets.empty()) return nullptr;

    auto closest = std::min_element(targets.begin(), targets.end(),
        [&from](const Soldier* a, const Soldier* b) {
            int distA = abs(a->gridPosition.x - from.gridPosition.x) + abs(a->gridPosition.y - from.gridPosition.y);
            int distB = abs(b->gridPosition.x - from.gridPosition.x) + abs(b->gridPosition.y - from.gridPosition.y);
            return distA < distB;
        });

    return *closest;
}

// Bir hedefe DO�RU bir sonraki en iyi ad�m� hesaplar.
sf::Vector2i AIOwner::calculateMoveTowards(const Soldier& from, const sf::Vector2i& targetPos)
{
    sf::Vector2i startPos = from.gridPosition;
    int dx = targetPos.x - startPos.x;
    int dy = targetPos.y - startPos.y;
    sf::Vector2i nextPos = startPos;

    if (abs(dx) > abs(dy)) {
        nextPos.x += (dx > 0) ? 1 : -1;
    }
    else if (dy != 0) { // Sadece dy s�f�r de�ilse hareket et
        nextPos.y += (dy > 0) ? 1 : -1;
    }
    return nextPos;
}

// Bir tehditten UZAKLA�ACAK bir sonraki en iyi ad�m� hesaplar.
sf::Vector2i AIOwner::calculateMoveAway(const Soldier& from, const sf::Vector2i& threatPos)
{
    sf::Vector2i startPos = from.gridPosition;
    int dx = threatPos.x - startPos.x;
    int dy = threatPos.y - startPos.y;
    sf::Vector2i nextPos = startPos;

    // Y�n�n tam tersini uygula
    if (abs(dx) > abs(dy)) {
        nextPos.x -= (dx > 0) ? 1 : -1;
    }
    else if (dy != 0) {
        nextPos.y -= (dy > 0) ? 1 : -1;
    }
    return nextPos;
}

void AIOwner::processTurn(GameManager& gameManager)
{
    const auto& allOwners = gameManager.getOwners();
    const auto& map = gameManager.getMap();

    // Her bir askerimiz i�in...
    for (auto& soldier : this->soldiers)
    {
        // 1. ANAL�Z: D��manlar� s�n�fland�r.
        std::vector<Soldier*> winnableTargets;
        std::vector<Soldier*> losingTargets;

        for (const auto& ownerPtr : allOwners) {
            if (ownerPtr.get() != this) { // D��man oyuncular� bul
                for (auto& enemy : ownerPtr->soldiers) {
                    // Kazanma durumu
                    if ((soldier.type == Soldier::Type::Triangle && enemy.type == Soldier::Type::Circle) ||
                        (soldier.type == Soldier::Type::Circle && enemy.type == Soldier::Type::Square) ||
                        (soldier.type == Soldier::Type::Square && enemy.type == Soldier::Type::Triangle)) {
                        winnableTargets.push_back(&enemy);
                    }

                    // Kaybetme durumu
                    else if ((enemy.type == Soldier::Type::Triangle && soldier.type == Soldier::Type::Circle) ||
                        (enemy.type == Soldier::Type::Circle && soldier.type == Soldier::Type::Square) ||
                        (enemy.type == Soldier::Type::Square && soldier.type == Soldier::Type::Triangle)) {
                        losingTargets.push_back(&enemy);
                    }
                }
            }
        }

        // 2. KARAR VERME ve HAREKET ETME (T�m hareket puanlar� bitene kadar)
        while (soldier.getCurrentPoints() > 0)
        {
            sf::Vector2i moveTarget;
            bool hasTarget = false;

            // DURUM 1: SALDIRI
            if (!winnableTargets.empty()) {
                Soldier* target = findClosestSoldier(soldier, winnableTargets);
                if (target) {
                    moveTarget = calculateMoveTowards(soldier, target->gridPosition);
                    hasTarget = true;
                }
            }
            // DURUM 2: KA�I�
            else if (!losingTargets.empty()) {
                Soldier* threat = findClosestSoldier(soldier, losingTargets);
                if (threat) {
                    moveTarget = calculateMoveAway(soldier, threat->gridPosition);
                    hasTarget = true;
                }
            }
            // DURUM 3: GEZ�NME
            else
            {
                sf::Vector2i randomCell = gameManager.getRandomMapCell();
                moveTarget = calculateMoveTowards(soldier, randomCell);
                hasTarget = true;
            }

            // 3. HAREKET� UYGULA
            if (hasTarget && soldier.gridPosition != moveTarget)
            {
                if (gameManager.getSoldierAt(moveTarget) == nullptr)
                {
                    // DE����KL�K: Manuel g�ncelleme yerine merkezi fonksiyonu kullan.
                    gameManager.executeMove(&soldier, moveTarget);

                    // TODO: Sava� kontrol�, animasyon bittikten sonra yap�lmal�.
                    // gameManager.checkForCombat(&soldier);
                }
                else {
                    break; // Hedef dolu.
                }
            }
            else {
                break; // Hareket edecek yer yok.
            }
           // std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}