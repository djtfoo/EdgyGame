#include "Pickup.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"

CPickup::CPickup(Mesh* _mesh, string name) : GenericEntity(_mesh, name)
{
}

CPickup::~CPickup()
{
}

CPickup* Create::Pickup(const std::string& _meshName,
    string _name,
    const Vector3& _position,
    const Vector3& _scale)
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    CPickup* result = new CPickup(modelMesh, _name);
    result->SetCollider(true);
    result->SetPosition(_position);
    result->SetScale(_scale);
    EntityManager::GetInstance()->AddEntity(result, true);

    return result;
}