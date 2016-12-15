#pragma once

#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../SceneGraph/SceneGraph.h"
#include "../BalloonBlock/GenericBalloon.h"

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

    vector<Vector3> m_WaypointList;
    int m_CurrWaypointIdx;
    int m_MaxWaypoints;

public:
    CEnemy();
    virtual ~CEnemy();

    void Init();
    void SetWaypoints(vector<Vector3> waypoints);
    void Reset();

    void Update(double dt = 0.0333f);
    void Constrain();   // constrain position within the borders
    void Render();

    void UpdateBodyParts(double dt);

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