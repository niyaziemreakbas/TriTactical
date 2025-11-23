#include "CombatManager.h"

bool CombatManager::resolveCombat(Soldier& attacker, Soldier& defender, Soldier*& selectedSoldierRef)
{
    Soldier::Type attType = attacker.type;
    Soldier::Type defType = defender.type;

    Soldier* winner = nullptr;
    Soldier* loser = nullptr;
    bool attackResult; // Saldýran kazandýysa true

    // Taþ-Kaðýt-Makas Mantýðý
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
        std::cout << "Combat is a draw. No change.\n";
        return false;
    }

    std::cout << winner->owner->name << " wins! (" << soldierTypeToString(winner->type)
        << " vs " << soldierTypeToString(loser->type) << ")\n";

    winner->setCurrentPoints(-abs(winner->getCurrentPoints()));

    loser->takeDamage(1);
    std::cout << "Loser HP: " << loser->getHp() << "\n";

    if (loser->isDead())
    {
        killSoldier(loser, selectedSoldierRef);
    }
    else
    {
        transferSoldier(loser, winner->owner, selectedSoldierRef);
    }

    return attackResult;
}

void CombatManager::transferSoldier(Soldier* soldierToTransfer, Owner* newOwner, Soldier*& selectedSoldierRef)
{
    Owner* oldOwner = soldierToTransfer->owner;

    // 1. Yeni listeye ekle
    newOwner->soldiers.push_back(*soldierToTransfer);

    // 2. Yeni listedeki askeri yapýlandýr
    Soldier& newSoldier = newOwner->soldiers.back();
    newSoldier.setOwner(newOwner);
    newSoldier.setCurrentPoints(-abs(newSoldier.getCurrentPoints())); // Hareketi sýfýrla

    // 3. Eðer oyunun seçili tuttuðu asker transfer edildiyse, pointer boþa düþmesin diye güncelle
    if (selectedSoldierRef == soldierToTransfer) {
        selectedSoldierRef = &newSoldier;
    }

    // 4. Eski listeden sil
    for (size_t i = 0; i < oldOwner->soldiers.size(); ++i)
    {
        if (&oldOwner->soldiers[i] == soldierToTransfer)
        {
            oldOwner->soldiers.erase(oldOwner->soldiers.begin() + i);
            break;
        }
    }

    std::cout << "Transfer complete.\n";
}

std::string CombatManager::soldierTypeToString(Soldier::Type type)
{
    switch (type) {
    case Soldier::Type::Square: return "Square";
    case Soldier::Type::Circle: return "Circle";
    case Soldier::Type::Triangle: return "Triangle";
    default: return "Unknown";
    }
}

void CombatManager::killSoldier(Soldier* soldierToKill, Soldier*& selectedSoldierRef)
{
    Owner* owner = soldierToKill->owner;

    std::cout << "Soldier died! Removing from " << owner->name << "\n";

    if (selectedSoldierRef == soldierToKill) {
        selectedSoldierRef = nullptr;
    }

    for (size_t i = 0; i < owner->soldiers.size(); ++i)
    {
        if (&owner->soldiers[i] == soldierToKill)
        {
            owner->soldiers.erase(owner->soldiers.begin() + i);
            break;
        }
    }
}