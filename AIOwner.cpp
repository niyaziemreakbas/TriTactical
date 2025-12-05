#include "AIOwner.h"
#include "Pathfinder.h"
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

// Bir hedefe DOÐRU bir sonraki en iyi adýmý hesaplar.
sf::Vector2i AIOwner::calculateMoveTowards(const Soldier& from, const sf::Vector2i& targetPos)
{
    sf::Vector2i startPos = from.gridPosition;
    int dx = targetPos.x - startPos.x;
    int dy = targetPos.y - startPos.y;
    sf::Vector2i nextPos = startPos;

    if (abs(dx) > abs(dy)) {
        nextPos.x += (dx > 0) ? 1 : -1;
    }
    else if (dy != 0) { // Sadece dy sýfýr deðilse hareket et
        nextPos.y += (dy > 0) ? 1 : -1;
    }
    return nextPos;
}

bool AIOwner::processTurn(GameManager& gameManager)
{
    const auto& allOwners = gameManager.getOwners();

    // Her bir askerimiz için...
    for (auto& soldier : this->soldiers)
    {
        // Hareket puaný yoksa pas geç
        if (soldier.getCurrentPoints() <= 0) continue;

        // 1. ANALÝZ: Düþmanlarý sýnýflandýr
        std::vector<Soldier*> winnableTargets;
        std::vector<Soldier*> losingTargets;

        for (const auto& ownerPtr : allOwners) {
            if (ownerPtr.get() != this) {
                for (auto& enemy : ownerPtr->soldiers) {
                    if ((soldier.type == Soldier::Type::Triangle && enemy.type == Soldier::Type::Circle) ||
                        (soldier.type == Soldier::Type::Circle && enemy.type == Soldier::Type::Square) ||
                        (soldier.type == Soldier::Type::Square && enemy.type == Soldier::Type::Triangle)) {
                        winnableTargets.push_back(&enemy);
                    }
                    else if ((enemy.type == Soldier::Type::Triangle && soldier.type == Soldier::Type::Circle) ||
                        (enemy.type == Soldier::Type::Circle && soldier.type == Soldier::Type::Square) ||
                        (enemy.type == Soldier::Type::Square && soldier.type == Soldier::Type::Triangle)) {
                        losingTargets.push_back(&enemy);
                    }
                }
            }
        }

        // 2. HEDEF BELÝRLEME (Sadece hedefi seçiyoruz, hareketi Pathfinder yapacak)
        sf::Vector2i finalDestination = { -1, -1 };
        bool wantsToFlee = false; // Kaçma durumu özeldir

        // DURUM 1: SALDIRI (En yakýn yenebileceðimiz düþman)
        if (!winnableTargets.empty()) {
            Soldier* target = findClosestSoldier(soldier, winnableTargets);
            if (target) {
                finalDestination = target->gridPosition;
            }
        }

        // DURUM 2: GEZÝNME (Veya kaçacak yer bulamadýysa rastgele kaçýþ)
        if (finalDestination == sf::Vector2i(-1, -1))
        {
            sf::Vector2i rnd = gameManager.findRandomEmptyCell();
        }

        // 3. PATHFINDER ÝLE HAREKET ET
        if (finalDestination != sf::Vector2i(-1, -1) && finalDestination != soldier.gridPosition)
        {
            std::vector<sf::Vector2i> path = Pathfinder::findPath(soldier, finalDestination, gameManager);
            if (!path.empty())
            {
                sf::Vector2i nextStep = path[0];

                if (gameManager.getSoldierAt(nextStep) == nullptr)
                {
                    gameManager.executeMove(&soldier, nextStep);
                    return true;
                }
            }
        }
    }

    return false;
}