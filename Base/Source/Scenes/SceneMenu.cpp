#include "SceneMenu.h"
#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"

#include <iostream>

SceneMenu::SceneMenu()
{
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Init()
{
    camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
    GraphicsManager::GetInstance()->AttachCamera(&camera);

    // Load all the meshes
    MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//MenuState.tga");

    float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
    float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
    SceneMenuBackground = Create::Sprite2DObject("MENUSTATE_BKGROUND",
        Vector3(halfWindowWidth, halfWindowHeight, 0.f), Vector3(800.f, 600.f, 0.f));

    std::cout << "SceneMenu loaded\n" << std::endl;
}

void SceneMenu::Update(double dt)
{
    if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
    {
        std::cout << "Loading SceneMenu" << std::endl;
        SceneManager::GetInstance()->SetActiveScene("GameState");
    }
}

void SceneMenu::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup 3D pipeline then render 3D
    GraphicsManager::GetInstance()->SetPerspectiveProjection(45.f, 4.f / 3.f, 0.1f, 10000.f);
    GraphicsManager::GetInstance()->AttachCamera(&camera);

    // Render the required entities
    EntityManager::GetInstance()->Render();

    // Setup 2D pipeline then render 2D
    GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(),
        0, Application::GetInstance().GetWindowHeight(), -10, 10);

    GraphicsManager::GetInstance()->DetachCamera();

    EntityManager::GetInstance()->RenderUI();
}

void SceneMenu::Exit()
{
    EntityManager::GetInstance()->RemoveEntity(SceneMenuBackground);

    MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

    GraphicsManager::GetInstance()->DetachCamera();
}