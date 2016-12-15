#pragma once

#include "../GenericEntity.h"
#include "../GroundEntity.h"

class CEnemy : public GenericEntity
{
protected:
    Vector3 defaultPosition, defaultTarget, defaultUp;
    Vector3 target, up;
    Vector3 maxBoundary, minBoundary;
    GroundEntity* m_pTerrain;

    double m_dSpeed;
    double m_dAcceleration;

	GenericEntity* m_Head;
	GenericEntity* m_Body;
	GenericEntity* m_LeftArm;
	GenericEntity* m_RightArm;
	GenericEntity* m_LeftLeg;
	GenericEntity* m_RightLeg;

public:
    CEnemy();
    virtual ~CEnemy();

    void Init();
    void Reset();

    void Update(double dt = 0.0333f);
    void Constrain();   // constrain position within the borders
    void Render();

    // Setters
    void SetPos(const Vector3& pos);
    void SetTarget(const Vector3& target);
    void SetUp(const Vector3& up);
    void SetBoundary(const Vector3& max, const Vector3& min);
    void SetTerrain(GroundEntity* terrain);

    // Getters
    Vector3 GetPos() const;
    Vector3 GetTarget() const;
    Vector3 GetUp() const;
    GroundEntity* GetTerrain();
};