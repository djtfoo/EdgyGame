#pragma once

#include "Vector3.h"
#include "WeaponInfo.h"
#include "Mtx44.h"
#include "Mesh.h"

class HeldWeapon
{
public:
    enum WEAPON_TYPE
    {
        WEAPON_NEEDLEGUN,
        WEAPON_KNIFE,
        WEAPON_TOTAL
    };

    enum WEAPON_ACTION
    {
        WA_NIL = 0,
        WA_FIRE,
        WA_RELOAD,
        WA_CHANGEWEAPON,
        WA_NOAMMO,
        WA_SPRINT,
        WA_TOTAL
    };

    HeldWeapon();
    ~HeldWeapon();

    void Init();
    void Update(const double dt);

    // Getters
    Vector3 GetPosition();
    float GetWeaponAngle();
    WEAPON_TYPE GetWeaponType();
    WEAPON_ACTION GetWeaponAction();
    Mesh* GetMesh();

    // Setters
    void SetWeaponType(WEAPON_TYPE type);

    // Set Weapon States
    void SetToFire();
    void SetToReload();
    void SetToChangeWeapon();
    void SetToNoAmmo();
    void SetToSprint();

    // Get information from player
    void CheckSprinting(const bool& sprinting);
    void CheckPlayerPos(const Vector3& position);
    void CheckPlayerTarget(const Vector3& target);

    void Render();

private:
    CWeaponInfo* curr_weapon;
    CWeaponInfo* weaponList[WEAPON_TOTAL];

    WEAPON_TYPE weapon_type;
    WEAPON_ACTION wa_action;

    // 3D render
    Vector3 weaponPos;
    float weaponAngle;
    Mtx44 recoilRotation;
    Mesh* m_mesh;

    float recoilTime;
    
    // Weapon logic
    bool action_change;
    double weaponTimeElapsed;
    
    // Player information
    bool b_sprinting;
    Vector3 player_pos;
    Vector3 player_target;

    // Setters
    void SetWeaponAction(WEAPON_ACTION action);

    // Player Actions
    void WeaponAction();

    void ChangeWeaponMovement(const double dt);     //animation for changing weapon
    void ReloadWeaponMovement(const double dt);
    void WeaponSprintMovement(const double dt);
    void WeaponRecoilMovement(const double dt);
};