#pragma once

#include "WeaponInfo.h"

class CNeedleGun : public CWeaponInfo
{
public:
    CNeedleGun();
    virtual ~CNeedleGun();

    // Initialise this instance to default values
    void Init(void);
    // Discharge this weapon
    void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};