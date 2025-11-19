#pragma once
#include "Soldier.h"
#include "Owner.h"
#include <iostream>
#include <vector>

class CombatManager
{
public:
    // Savaþ sonucunu hesaplar ve gerekli transferi yapar.
    // selectedSoldierRef: GameManager'daki seçili askeri güncellemek için referans olarak alýnýr.
    bool resolveCombat(Soldier& attacker, Soldier& defender, Soldier*& selectedSoldierRef);

private:
    // Askeri bir sahibin listesinden diðerine taþýr.
    void transferSoldier(Soldier* soldierToTransfer, Owner* newOwner, Soldier*& selectedSoldierRef);

    // Debug için yardýmcý fonksiyon
    std::string soldierTypeToString(Soldier::Type type);
};