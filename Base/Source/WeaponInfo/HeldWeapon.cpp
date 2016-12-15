#include "HeldWeapon.h"
#include "MeshBuilder.h"
#include "NeedleGun.h"
#include "LaserBlaster.h"
#include "../WeaponInfo/GrenadeThrow.h"

#include "RenderHelper.h"

HeldWeapon::HeldWeapon()
{
    weapon_type = WEAPON_NEEDLEGUN;
    wa_action = WA_NIL;
    action_change = false;

    weaponPos.SetZero();
    //weaponPos.Set(0.3f, 0.f, 0.f);
    weaponAngle = 0.f;
    recoilRotation.SetToIdentity();

    weaponTimeElapsed = 0.0;

    for (int i = 0; i < WEAPON_TOTAL; ++i)
    {
        weaponList[i] = NULL;
    }

    curr_weapon = weaponList[0];

    m_mesh = NULL;
}

HeldWeapon::~HeldWeapon()
{
    for (int i = 0; i < WEAPON_TOTAL; ++i)
    {
        if (weaponList[i] != NULL)
        {
            delete weaponList[i];
            weaponList[i] = NULL;
        }
    }
}

void HeldWeapon::Init()
{
    weaponList[0] = new CNeedleGun();
    weaponList[0]->Init();

    //weaponList[1] = new CLaserBlaster();
    //weaponList[1]->Init();

    weaponList[1] = new CGrenadeThrow();
    weaponList[1]->Init();

    SetWeaponType(WEAPON_NEEDLEGUN);
}

void HeldWeapon::Update(const double dt)
{
    for (int i = 0; i < WEAPON_TOTAL; ++i)
    {
        weaponList[i]->Update(dt);
    }

    // Update Gun
    if (action_change) {
        WeaponAction();
        action_change = false;
    }

    if (b_sprinting && wa_action == WA_NIL) {
        wa_action = WA_SPRINT;
    }

    Mtx44 rotation;
    WeaponRecoilMovement(dt);
    // ACCESS THE PLAYER CLASS.
    //rotation.SetToRotation(viewRecoil, 1, 0, 0);
    //camera.viewDirection = rotation * camera.viewDirection;
    //camera.target = camera.position + camera.viewDirection;

    switch (wa_action)
    {
    case WA_NIL:
        break;
    case WA_FIRE:
        weaponTimeElapsed += dt;

        if (curr_weapon->GetMagRound() == 0 && curr_weapon->GetTotalRound() > 0) {  // empty
            wa_action = WA_RELOAD;
            action_change = true;
            weaponTimeElapsed = 0.0;
        }
        else if (weaponTimeElapsed >= curr_weapon->GetTimeBetweenShots()) {
            wa_action = WA_NIL;
            weaponTimeElapsed = 0.0;
        }
        break;

    case WA_RELOAD:
        ReloadWeaponMovement(dt);
        break;

    case WA_CHANGEWEAPON:
        ChangeWeaponMovement(dt);
        break;

    case WA_NOAMMO:
        weaponTimeElapsed += dt;
        if (weaponTimeElapsed >= 0.3) {
            wa_action = WA_NIL;
            weaponTimeElapsed = 0.0;
        }
        break;

    case WA_SPRINT:
        WeaponSprintMovement(dt);
        break;
    }
}

// Getters
Vector3 HeldWeapon::GetPosition()
{
    return weaponPos;
}

float HeldWeapon::GetWeaponAngle()
{
    return weaponAngle;
}

HeldWeapon::WEAPON_TYPE HeldWeapon::GetWeaponType()
{
    return weapon_type;
}

HeldWeapon::WEAPON_ACTION HeldWeapon::GetWeaponAction()
{
    return wa_action;
}

Mesh* HeldWeapon::GetMesh()
{
    return m_mesh;
}

// Setters
void HeldWeapon::SetWeaponType(WEAPON_TYPE type)
{
    if (wa_action == WA_NIL)
    {
        weapon_type = type;

        // change currently held weapon
        curr_weapon = weaponList[type];

        switch (type)
        {
        case WEAPON_NEEDLEGUN:
            m_mesh = MeshBuilder::GetInstance()->GetMesh("NeedleGun");
            break;
        case WEAPON_KNIFE:
            m_mesh = MeshBuilder::GetInstance()->GetMesh("Grenade");
            break;
        }
    }
}

void HeldWeapon::SetWeaponAction(WEAPON_ACTION action)
{
    wa_action = action;
    action_change = true;
}

// Player Actions
void HeldWeapon::WeaponAction()
{
    // Use up ammo
    if (wa_action == WA_FIRE)
    {
        //if (weapon->GetCurrAmmo() == 0) {
        //    wa_action = WA_NOAMMO;
        //}
        //else {
        //    weapon->SetCurrAmmo(weapon->GetCurrAmmo() - 1);
        //}

        if (curr_weapon->GetMagRound() == 0) {
            wa_action = WA_NOAMMO;
        }
        else {
            // DISCHARGE USING PLAYER'S POSITION AND TARGET
            curr_weapon->Discharge(player_pos, player_target);
        }

    }

    // Check if player is at max ammo inside gun, or ran out of ammo
    else if (wa_action == WA_RELOAD) {

        //if (curr_weapon->GetTotalAmmo() == 0) {
        //    wa_action = WA_NOAMMO;
        //}
        //else if (curr_weapon->GetCurrAmmo() == weapon->GetMaxAmmo()) {
        //    wa_action = WA_NIL;
        //}


        if (curr_weapon->GetMaxTotalRound() == 0 && curr_weapon->GetTotalRound() > 0) {
            wa_action = WA_NOAMMO;
        }
        else if (curr_weapon->GetMagRound() == curr_weapon->GetMaxMagRound()) {
            wa_action = WA_NIL;
        }

    }

    switch (wa_action)
    {
    case WA_FIRE:
        //Shoot();
        recoilRotation.SetToRotation(-5.f * curr_weapon->GetTimeBetweenShots(), 1, 0, 0);
        //camera.viewDirection = rotation * camera.viewDirection;
        //camera.target = camera.position + camera.viewDirection;

        recoilTime = curr_weapon->GetTimeBetweenShots();
        break;

    case WA_RELOAD:
        //Application::GetInstance().m_soundEngine->play2D("Sound//SE_reload.mp3");
        break;

    case WA_CHANGEWEAPON:

        weaponPos.Set(0.f, -1.f, 0.f);

        //switch (weapon_type)
        //{
        //case WEAPON_NEEDLEGUN:
        //    weapon = &Pistol;
        //    break;
        //}

        //Application::GetInstance().m_soundEngine->play2D("Sound//SE_change weapon.wav");

        break;

    case WA_NOAMMO:
        //Application::GetInstance().m_soundEngine->play2D("Sound//SE_no ammo.wav");
        break;

    case WA_NIL:
        break;
    }
}

void HeldWeapon::ChangeWeaponMovement(const double dt)
{
    weaponPos.y += 5.f * dt;
    if (weaponPos.y > 0.1f) {
        weaponPos.y = 0.f;
        wa_action = WA_NIL;
    }
}

static int animation_direction = 1;
static const float ROTSPEED = 200.f;
static const float TRANSSPEED = 2.f;

void HeldWeapon::ReloadWeaponMovement(const double dt)
{
    weaponPos.y -= TRANSSPEED * dt * animation_direction;
    weaponAngle += ROTSPEED * dt * animation_direction;

    if (animation_direction == 1) {     //rotate the weapon towards the player
        if (weaponPos.y < -0.4f) {
            weaponPos.y = -0.4f;
        }
        if (weaponAngle > 70.f) {
            weaponAngle = 70.f;
        }

        if (weaponPos.y <= -0.4f && weaponAngle >= 70.f) {
            weaponTimeElapsed += dt;
            // Start sound effect here
            if (weaponTimeElapsed >= 0.8) {
                weaponTimeElapsed = 0.0;
                animation_direction = -1;
                // reload
                curr_weapon->Reload();
                //int ammo = Math::Min(curr_weapon->GetMaxAmmo() - curr_weapon->GetCurrAmmo(), weapon->GetTotalAmmo());     //amount to reload
                //weapon->SetCurrAmmo(curr_weapon->GetCurrAmmo() + ammo);  //reload bullets to max; but if only holding on to less than max, then reload that amount
                //weapon->SetTotalAmmo(curr_weapon->GetTotalAmmo() - ammo);
            }
        }
    }
    else {      //rotate the weapon back

        if (weaponPos.y > 0.f) {
            weaponPos.y = 0.f;
        }
        if (weaponAngle < 0.f) {
            weaponAngle = 0.f;
        }

        if (weaponPos.y >= 0.f && weaponAngle <= 0.f) {
            wa_action = WA_NIL;
            animation_direction = 1;
        }
    }
}

void HeldWeapon::WeaponSprintMovement(const double dt)
{
    if (b_sprinting) {
        animation_direction = 1;
    }
    else {
        animation_direction = -1;
    }

    weaponPos.y -= TRANSSPEED * dt * animation_direction;
    weaponAngle += ROTSPEED * dt * animation_direction;

    if (b_sprinting) {     //rotate the weapon towards the player
        if (weaponPos.y < -0.4f) {
            weaponPos.y = -0.4f;
        }
        if (weaponAngle > 70.f) {
            weaponAngle = 70.f;
        }

    }
    else {      //rotate the weapon back

        if (weaponPos.y > 0.f) {
            weaponPos.y = 0.f;
        }
        if (weaponAngle < 0.f) {
            weaponAngle = 0.f;
        }

        if (weaponPos.y >= 0.f && weaponAngle <= 0.f) {
            wa_action = WA_NIL;
            animation_direction = 1;
        }
    }
}

void HeldWeapon::WeaponRecoilMovement(const double dt)
{
    recoilTime -= (float)(15.f * dt);

    if (recoilTime < 0.f) {
        recoilTime = 0.f;
    }
}

// Set Weapon States
void HeldWeapon::SetToFire()
{
    if (wa_action == WA_NIL)
    {
        SetWeaponAction(WA_FIRE);
    }
}

void HeldWeapon::SetToReload()
{
    if (wa_action == WA_NIL)
    {
        SetWeaponAction(WA_RELOAD);
    }
}

void HeldWeapon::SetToChangeWeapon()
{
    if (wa_action == WA_NIL)
    {
        SetWeaponAction(WA_CHANGEWEAPON);
    }
}

void HeldWeapon::SetToNoAmmo()
{
    SetWeaponAction(WA_NOAMMO);
}

void HeldWeapon::SetToSprint()
{
    SetWeaponAction(WA_SPRINT);
}

void HeldWeapon::CheckSprinting(const bool& sprinting)
{
    b_sprinting = sprinting;
}

void HeldWeapon::CheckPlayerPos(const Vector3& position)
{
    player_pos = position;
}

void HeldWeapon::CheckPlayerTarget(const Vector3& target)
{
    player_target = target;
}

void HeldWeapon::Render()
{
    //RenderHelper::RenderMesh(m_mesh);
    RenderHelper::RenderMeshWithLight(m_mesh);
}