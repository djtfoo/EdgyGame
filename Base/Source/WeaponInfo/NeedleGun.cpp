#include "NeedleGun.h"
#include "../Projectile/Laser.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"

CNeedleGun::CNeedleGun()
{
}


CNeedleGun::~CNeedleGun()
{
}

// Initialise this instance to default values
void CNeedleGun::Init(void)
{
    // Call the parent's Init method
    CWeaponInfo::Init();

    // The number of ammunition in a magazine for this weapon
    magRounds = 1;
    // The maximum number of ammunition for this magazine for this weapon
    maxMagRounds = 1;
    // The current total number of rounds currently carried by this player
    totalRounds = 10;
    // The max total number of rounds currently carried by this player
    maxTotalRounds = 10;

    // The time between shots
    timeBetweenShots = 0.5;
    // The elapsed time (between shots)
    elapsedTime = 0.0;
    // Boolean flag to indicate if weapon can fire now
    bFire = true;
}

// Discharge this weapon
void CNeedleGun::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
    if (bFire)
    {
        // If there is still ammo in the magazine, then fire
        if (magRounds > 0)
        {
            Vector3 _direction = (target - position).Normalized();
            // Create a laser with a laser mesh. The length is 10.0f, mesh is also set at 10.0f
            CLaser* aLaser = Create::Laser("laser",
                position,
                _direction,
                10.0f,
                2.0f,
                100.0f,
                _source);
            aLaser->SetIsLaser(true);
            aLaser->SetCollider(false);
            //aLaser->SetCollider(true);
            //aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
            bFire = false;
            magRounds--;
        }
    }
}
