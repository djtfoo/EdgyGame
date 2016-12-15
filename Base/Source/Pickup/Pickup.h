#pragma once

#include "../GenericEntity.h"
#include "Vector3.h"

class CPickup : public GenericEntity
{
public:
    CPickup(Mesh* _mesh, string name = "Pickup");
    virtual ~CPickup();
};

namespace Create
{
    CPickup* Pickup(const std::string& _meshName,
        string _name,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.f, 1.f, 1.f));
};