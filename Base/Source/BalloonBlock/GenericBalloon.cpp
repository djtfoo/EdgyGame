#include "GenericBalloon.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "../SceneGraph/SceneGraph.h"

GenericBalloon::GenericBalloon(Mesh* _modelMesh)
    : GenericEntity(_modelMesh, "GenericBalloon")
{

}

GenericBalloon::~GenericBalloon()
{

}

void GenericBalloon::Init()
{
    m_state = ALIVE;
}

void GenericBalloon::Update(double dt)
{
    switch (m_state)
    {
    case GenericBalloon::ALIVE:
        // Normal Stuff (Gravity?)

        if (this->GetIsInSceneGraph())
        {
            if (IsParentDeflate(this))
            {
                m_state = DEFLATING;
            }
        }
        break;
    case GenericBalloon::DEFLATING:
        Deflate(dt);
        break;
    case GenericBalloon::DEFLATED:
        this->SetIsDone(true);

        // Remove from Scene Graph
        if (CSceneGraph::GetInstance()->DeleteNode(this) == true)
        {
        	cout << "*** That Entity removed ***" << endl;
        }

        break;
    default:
        break;
    }
}

void GenericBalloon::Deflate(double dt)
{
    this->scale.x -= (float)dt;
    this->scale.y -= (float)dt;
    this->scale.z -= (float)dt;

    if (scale.x < 0.01f || scale.y < 0.01f || scale.z < 0.01f)
        m_state = DEFLATED;

    // Move the balloon object down, as deflate
    this->position.y -= (float)dt;
    this->position.x += Math::RandFloatMinMax(-1, 1) * (float)dt;
    this->position.z += Math::RandFloatMinMax(-1, 1) * (float)dt;
}

void GenericBalloon::SetType(const BALLOON_TYPES &type)
{
    m_type = type;
}

BALLOON_TYPES GenericBalloon::GetType()
{
    return m_type;
}

void GenericBalloon::SetState(const BALLOON_STATE &state)
{
    m_state = state;
}

GenericBalloon::BALLOON_STATE GenericBalloon::GetState()
{
    return m_state;
}

GenericBalloon* Create::Balloon(const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _scale)
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    GenericBalloon* result = new GenericBalloon(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetCollider(false);
    result->Init();
    EntityManager::GetInstance()->AddEntity(result, true);
    //EntityManager::GetInstance()->GetSpartialPartition()->Add(result);
    return result;
}

bool GenericBalloon::IsParentDeflate(GenericEntity* thisNode)
{
    GenericBalloon* check = dynamic_cast<GenericBalloon*>(CSceneGraph::GetInstance()->GetNode(thisNode)->GetParent()->GetEntity());
    if (check != NULL)
    {
        check = dynamic_cast<GenericBalloon*>(CSceneGraph::GetInstance()->GetNode(thisNode)->GetParent()->GetEntity());
        if (check->GetState() == DEFLATING)
        {
            return true;
        }
    }

    return false;
}
