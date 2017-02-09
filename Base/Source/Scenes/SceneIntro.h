#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;

class SceneIntro : public Scene
{
public:
    SceneIntro();
    virtual ~SceneIntro();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

private:
    FPSCamera camera;
    SpriteEntity* SceneIntroBackground;
};