#pragma once
#include "Soldier.h"
#include "Owner.h"
#include <iostream>
#include <vector>

class CombatManager
{
public:
    bool resolveCombat(Soldier& attacker, Soldier& defender, Soldier*& selectedSoldierRef);
    void killSoldier(Soldier* soldierToKill, Soldier*& selectedSoldierRef);

private:
    void transferSoldier(Soldier* soldierToTransfer, Owner* newOwner, Soldier*& selectedSoldierRef);

    std::string soldierTypeToString(Soldier::Type type);
};