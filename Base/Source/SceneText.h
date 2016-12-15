#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include "Enemy/Enemy.h"
#include "BalloonBlock\GenericBalloon.h"
#include "SceneGraph\SceneGraph.h"
#include "SpriteEntity.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[7];
    SpriteEntity* HUDObj[3];
	Light* lights[2];

	GenericEntity* theCube;
    CEnemy* theEnemy;

	static SceneText* sInstance; // The pointer to the object that gets registered

    // Fucntions to spawn buildings
    void SpawnArena(Vector3 spawnPos); 
    void SpawnTunnel(Vector3 spawnPos); 
    void SpawnJumpCourse(Vector3 spawnPos);
    void SpawnWindmill(Vector3 spawnPos);

	void SpawnCastle(Vector3 spawnPos);
	void SpawnLamp(Vector3 spawnPos, CSceneNode* baseEntity);
    void SpawnWall(Vector3 spawnPos, float rotate, CSceneNode* baseEntity = NULL);
	void SpawnTower(Vector3 spawnPos, CSceneNode* baseEntity);

    void SpawnDiamond(const Vector3& spawnPos);
    void SpawnAmmo(const Vector3& spawnPos);
};

#endif