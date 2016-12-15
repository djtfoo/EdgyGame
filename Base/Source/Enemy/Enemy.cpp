#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "../SceneGraph/SceneGraph.h"

CEnemy::CEnemy()
: GenericEntity(NULL)
, defaultPosition(0.f, 0.f, 0.f), defaultTarget(0.f, 0.f, 0.f), defaultUp(0.f, 0.f, 0.f)
, target(0.f, 0.f, 0.f), up(0.f, 0.f, 0.f)
, maxBoundary(0.f, 0.f, 0.f), minBoundary(0.f, 0.f, 0.f)
, m_pTerrain(NULL)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Init()
{
    // Set the default values
    defaultPosition.Set(0.f, 0.f, 10.f);
    defaultTarget.SetZero();
    defaultUp.Set(0.f, 1.f, 0.f);

    // Set the current values
    position.Set(10.f, 0.f, 0.f);
    target.Set(10.f, 0.f, 450.f);
    up.Set(0.f, 1.f, 0.f);

    // Set the boundary
    maxBoundary.Set(1.f, 1.f, 1.f);
    minBoundary.Set(-1.f, -1.f, -1.f);

    // Set speed
    m_dSpeed = 1.0;

	// ----------- Set up Low Res Model ----------- //
	// Add to Scene Graph, and set self to low res texture
	CSceneNode* lowResNode = CSceneGraph::GetInstance()->AddNode(this);
	lowResNode->SetLowResRender(true);

    // Initialise LOD meshes
    InitLOD("", "", "cube");

    // Initialise collider
    this->SetCollider(true);
    this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

    // Add to EntityManager
    EntityManager::GetInstance()->AddEntity(this, true);

	// --------- Set up individual Models --------- // 


}

void CEnemy::Reset()
{
    // Set current values to default values
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

void CEnemy::SetPos(const Vector3& pos)
{
    this->position = pos;
}

void CEnemy::SetTarget(const Vector3& target)
{
    this->target = target;
}

void CEnemy::SetUp(const Vector3& up)
{
    this->up = up;
}

void CEnemy::SetBoundary(const Vector3& max, const Vector3& min)
{
    this->maxBoundary = max;
    this->minBoundary = min;
}

void CEnemy::SetTerrain(GroundEntity* terrain)
{
    m_pTerrain = terrain;
    SetBoundary(m_pTerrain->GetMaxBoundary(), m_pTerrain->GetMinBoundary());
}

Vector3 CEnemy::GetPos() const
{
    return position;
}

Vector3 CEnemy::GetTarget() const
{
    return target;
}

Vector3 CEnemy::GetUp() const
{
    return up;
}

GroundEntity* CEnemy::GetTerrain()
{
    return m_pTerrain;
}

void CEnemy::Update(double dt)
{
    Vector3 view = (target - position).Normalized();
    position += view * (float)(m_dSpeed) * (float)(dt);

    Constrain();

    if (position.z > 400.f || position.z < -400.f)
        target.z = position.z * -1;
}

void CEnemy::Constrain()
{
    // Constrain player within the boundary
    if (position.x > maxBoundary.x - 1.f)
        position.x = maxBoundary.x - 1.f;
    else if (position.x < minBoundary.x + 1.f)
        position.x = minBoundary.x + 1.f;

    if (position.z > maxBoundary.z - 1.f)
        position.z = maxBoundary.z - 1.f;
    else if (position.z < minBoundary.z + 1.f)
        position.z + minBoundary.z + 1.f;

    // update y position to the terrain height at that position
    if (position.y != m_pTerrain->GetTerrainHeight(position))
        position.y = m_pTerrain->GetTerrainHeight(position);
}

void CEnemy::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Scale(scale.x, scale.y, scale.z);
    if (GetLODStatus() == true)
    {
        if (theDetailLevel != NO_DETAILS)
        {
            RenderHelper::RenderMesh(GetLODMesh());
        }
    }
    modelStack.PopMatrix();
}