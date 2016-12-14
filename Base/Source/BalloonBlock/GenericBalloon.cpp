#include "GenericBalloon.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"

GenericBalloon::GenericBalloon(Mesh* _modelMesh)
    : GenericEntity(_modelMesh, "GenericBalloon")
{

}

GenericBalloon::~GenericBalloon()
{

}

void GenericBalloon::Update(double dt)
{
    switch (m_state)
    {
    case GenericBalloon::ALIVE:
        // Normal Stuff (Gravity?)
        break;
    case GenericBalloon::DEFLATING:
        Deflate(dt);
        break;
    case GenericBalloon::DEFLATED:
        this->SetIsDone(true);
        break;
    default:
        break;
    }
}

void GenericBalloon::Deflate(double dt)
{
    this->scale.y -= 10 * dt;

    if (scale.y < 0.1)
        m_state = DEFLATED;
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
    EntityManager::GetInstance()->AddEntity(result, true);
    //EntityManager::GetInstance()->GetSpartialPartition()->Add(result);
    return result;
}