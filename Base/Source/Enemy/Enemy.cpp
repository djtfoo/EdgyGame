#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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
    position.Set(10.f, 20.f, 0.f);
    target.Set(10.f, 0.f, 450.f);
    up.Set(0.f, 1.f, 0.f);

    // Set the boundary
    SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());

    // Set speed
    m_dSpeed = 1.0;

	// ----------- Set up Low Res Model ----------- //
	// Add to Scene Graph, and set self to low res texture
	CSceneNode* lowResNode = CSceneGraph::GetInstance()->AddNode(this);
    this->SetLowResRender(true);

    // Initialise LOD meshes
    InitLOD("", "", "low_res_enemy_overall");

    // Initialise collider
    this->SetCollider(false);
    this->SetScale(Vector3(3.5, 3.5, 3.5));
    this->SetAABB((scale * 0.5), -(scale * 0.5));

    // Add to EntityManager
    EntityManager::GetInstance()->AddEntity(this, true);

	// --------- Set up individual Models --------- // 

    // Body
    m_Body = Create::Balloon("high_res_enemy_body", Vector3(0, 0, 0));
    m_Body->SetCollider(true);
    m_Body->SetScale(Vector3(1, 1, 1));
    m_Body->SetAABB((m_Body->GetScale() * 0.5), -(m_Body->GetScale() * 0.5));
    m_Body->InitLOD("high_res_enemy_body", "low_res_enemy_body", "");
    m_Body->SetPosition(this->position);
    CSceneNode* EnemyBodyNode = lowResNode->AddChild(m_Body);

    // Head
    m_Head = Create::Balloon("high_res_enemy_head", Vector3(0, 0, 0));
    m_Head->SetCollider(true);
    m_Head->SetScale(Vector3(1, 1, 1));
    m_Head->SetAABB((m_Head->GetScale() * 0.5), -(m_Head->GetScale() * 0.5));
    m_Head->InitLOD("high_res_enemy_head", "low_res_enemy_head", "");
    m_Head->SetPosition(this->position);
    CSceneNode* EnemyHeadNode = EnemyBodyNode->AddChild(m_Head);
    EnemyHeadNode->ApplyTranslate(0, 2.5f, 0);

    // L-Arm
    m_LeftArm = Create::Balloon("high_res_enemy_arm", Vector3(0, 0, 0));
    m_LeftArm->SetCollider(true);
    m_LeftArm->SetScale(Vector3(1, 0.8, 1));
    m_LeftArm->SetAABB((m_LeftArm->GetScale() * 0.5), -(m_LeftArm->GetScale() * 0.5));
    m_LeftArm->InitLOD("high_res_enemy_arm", "low_res_enemy_arm", "");
    m_LeftArm->SetPosition(this->position);
    CSceneNode* EnemyarmNode = EnemyBodyNode->AddChild(m_LeftArm);
    EnemyarmNode->ApplyTranslate(-1.5f, 0.3f, 0);

    // R-Arm
    m_RightArm = Create::Balloon("high_res_enemy_arm", Vector3(0, 0, 0));
    m_RightArm->SetCollider(true);
    m_RightArm->SetScale(Vector3(1, 0.8, 1));
    m_RightArm->SetAABB((m_RightArm->GetScale() * 0.5), -(m_RightArm->GetScale() * 0.5));
    m_RightArm->InitLOD("high_res_enemy_arm", "low_res_enemy_arm", "");
    m_RightArm->SetPosition(this->position);
    EnemyarmNode = EnemyBodyNode->AddChild(m_RightArm);
    EnemyarmNode->ApplyTranslate(1.5f, 0.3f, 0);

    // L-Leg
    m_LeftLeg = Create::Balloon("high_res_enemy_leg", Vector3(0, 0, 0));
    m_LeftLeg->SetCollider(true);
    m_LeftLeg->SetScale(Vector3(0.5, 1, 1));
    m_LeftLeg->SetAABB((m_LeftLeg->GetScale() * 0.5), -(m_LeftLeg->GetScale() * 0.5));
    m_LeftLeg->InitLOD("high_res_enemy_leg", "low_res_enemy_leg", "");
    m_LeftLeg->SetPosition(this->position);
    CSceneNode* EnemylegNode = EnemyBodyNode->AddChild(m_LeftLeg);
    EnemylegNode->ApplyTranslate(0.8f, -2.0f, 0);

    // R-Leg
    m_RightLeg = Create::Balloon("high_res_enemy_leg", Vector3(0, 0, 0));
    m_RightLeg->SetCollider(true);
    m_RightLeg->SetScale(Vector3(0.5, 1, 1));
    m_RightLeg->SetAABB((m_RightLeg->GetScale() * 0.5), -(m_RightLeg->GetScale() * 0.5));
    m_RightLeg->InitLOD("high_res_enemy_leg", "low_res_enemy_leg", "");
    m_RightLeg->SetPosition(this->position);
    EnemylegNode = EnemyBodyNode->AddChild(m_RightLeg);
    EnemylegNode->ApplyTranslate(-0.8f, -2.0f, 0);
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

    UpdateBodyParts();

    //if (position.z > 400.f || position.z < -400.f)
    //    target.z = position.z * -1;
}

void CEnemy::UpdateBodyParts()
{
    if (!m_Head->IsDone())
        m_Head->SetPosition(this->position);

    if (!m_Body->IsDone())
        m_Body->SetPosition(this->position);

    if (!m_LeftArm->IsDone())
        m_LeftArm->SetPosition(this->position);

    if (!m_RightArm->IsDone())
        m_RightArm->SetPosition(this->position);

    if (!m_LeftLeg->IsDone())
        m_LeftLeg->SetPosition(this->position);

    if (!m_RightLeg->IsDone())
        m_RightLeg->SetPosition(this->position);
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
    //if (position.y != m_pTerrain->GetTerrainHeight(position))
    //    position.y = m_pTerrain->GetTerrainHeight(position);
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
            if (theDetailLevel == LOW_DETAILS)
                RenderHelper::RenderMesh(GetLODMesh());
            else
            {
                //RenderHelper::RenderMesh(m_Head->GetLODMesh());
                //RenderHelper::RenderMesh(m_Body->GetLODMesh());
                //RenderHelper::RenderMesh(m_LeftArm->GetLODMesh());
                //RenderHelper::RenderMesh(m_RightArm->GetLODMesh());
                //RenderHelper::RenderMesh(m_LeftLeg->GetLODMesh());
                //RenderHelper::RenderMesh(m_RightLeg->GetLODMesh());
            }
        
        }
    }
    modelStack.PopMatrix();
}