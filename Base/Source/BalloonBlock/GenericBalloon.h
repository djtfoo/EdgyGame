#pragma once

#include "../GenericEntity.h"

enum BALLOON_TYPES
{
    BLUE,
    RED, 
    GREEN,
    YELLOW,
};

class GenericBalloon : public GenericEntity
{
public:
    GenericBalloon(Mesh* _modelMesh);
    virtual ~GenericBalloon();

    virtual void Init();
    virtual void Update(double _dt);

    enum BALLOON_STATE
    {
        ALIVE,
        DEFLATING,
        DEFLATED,
    };

    void SetType(const BALLOON_TYPES &type);
    BALLOON_TYPES GetType();

    void SetState(const BALLOON_STATE &state);
    BALLOON_STATE GetState();

    // Check if parent is deflating
    bool IsParentDeflate(GenericEntity* thisNode);

private:

    BALLOON_TYPES m_type;
    BALLOON_STATE m_state;

    // Functions to put in update

    // Deflating
    void Deflate(double dt);


};

namespace Create
{
    GenericBalloon* Balloon(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
}