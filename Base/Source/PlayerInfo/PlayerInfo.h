#pragma once
#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "../WeaponInfo/HeldWeapon.h"

class CPlayerInfo
{
protected:
    static CPlayerInfo *s_instance;
	CPlayerInfo(void);

public:
    enum MOVEMENT_STATE     // movement speed
    {
        MOVEMENT_STATE_IDLE,
        MOVEMENT_STATE_CROUCH,
        MOVEMENT_STATE_WALK,
        MOVEMENT_STATE_RUN,
    };

    enum HEIGHT_STATE   // player's height
    {
        HEIGHT_STATE_STANDING,
        HEIGHT_STATE_CROUCH,
        HEIGHT_STATE_JUMP,
        HEIGHT_STATE_TOTAL
    };

	static CPlayerInfo *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~CPlayerInfo(void);

	// Initialise this class instance
	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

    // Check for player state
    bool isOnGround();
    bool IsJumping();
    bool IsCrouching();

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);
    // Get HeldWeapon
    HeldWeapon* GetHeldWeapon();

    // For movement & collision response
    MOVEMENT_STATE GetMovementState();
    void SetMovementState(MOVEMENT_STATE state);
    HEIGHT_STATE GetHeightState();
    void SetHeightState(HEIGHT_STATE state);

    bool b_setToJump;
    float GetJumpSpeed();
    void SetJumpSpeed(float jumpSpeed);
    
    float GetMovementSpeed();
    void SetMovementSpeed(float mSpeed);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);

	// Handling Camera
	void AttachCamera(FPSCamera* _cameraPtr);
	void DetachCamera(void);

    // Render
    void RenderWeapon();

    // Diamond pickup
    int GetDiamondsLeftToCollect();
    void SetDiamondsLeftToCollect(int num);

private:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 position, target, up;
    Vector3 velocity;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

    MOVEMENT_STATE m_movementState;
    HEIGHT_STATE m_heightState;
    
    Vector3 m_prevVelocity;

    const float m_STAND_EYELEVEL;
    const float m_CROUCH_EYELEVEL;

    float m_eyeLevel;
    float m_speed;
    float m_jumpSpeed;
    float m_gravity;
    float m_jumpHeight;
    float m_jumpAcceleration;

    void Crouch();
    void StandUp();
    void Jump();

    void UpdatePlayerHeight(const double dt);
    void UpdateStandUp(const double dt);
    void UpdateCrouch(const double dt);
    void UpdateJump(const double dt);

	FPSCamera* attachedCamera;

    double m_dSpeed;

	//CWeaponInfo* primaryWeapon;
	//CWeaponInfo* secondaryWeapon;

    // class to handle weapon 3D model
    HeldWeapon* m_heldWeapon;   // shift CWeaponInfo weapons here

    int m_diamondsLeftToCollect;

    // Keys to move the player
    char keyMoveForward;
    char keyMoveBackward;
    char keyMoveLeft;
    char keyMoveRight;
};
