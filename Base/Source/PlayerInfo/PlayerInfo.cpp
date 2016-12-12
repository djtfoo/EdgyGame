#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/LaserBlaster.h"
#include "../WeaponInfo/GrenadeThrow.h"

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
//	, m_dAcceleration(10.0)
//	, m_bJumpUpwards(false)
//	, m_dJumpSpeed(10.0)
//	, m_dJumpAcceleration(-10.0)
//	, m_bFallDownwards(false)
//	, m_dFallSpeed(0.0)
//	, m_dFallAcceleration(-10.0)
	: attachedCamera(NULL)
    , m_dSpeed(40.0)
	, m_pTerrain(NULL)
	, primaryWeapon(NULL)
	, secondaryWeapon(NULL)
{
    m_eyeLevel = m_STAND_EYELEVEL;
    m_speed = 0.f;
    m_jumpSpeed = 0.f;
    m_gravity = -100.f;
    m_jumpHeight = 0.f;

    m_movementState = MOVEMENT_STATE_IDLE;
    m_heightState = HEIGHT_STATE_STANDING;
}

CPlayerInfo::~CPlayerInfo(void)
{
	if (secondaryWeapon)
	{
		delete secondaryWeapon;
		secondaryWeapon = NULL;
	}
	if (primaryWeapon)
	{
		delete primaryWeapon;
		primaryWeapon = NULL;
	}
	m_pTerrain = NULL;
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	position.Set(0, 0, 10);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	// Set the pistol as the primary weapon
	primaryWeapon = new CPistol();
	primaryWeapon->Init();
	// Set the laser blaster as the secondary weapon
	//secondaryWeapon = new CLaserBlaster();
	//secondaryWeapon->Init();
	secondaryWeapon = new CGrenadeThrow();
	secondaryWeapon->Init();
}

// Set position
void CPlayerInfo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void CPlayerInfo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// ` this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

    m_eyeLevel = m_STAND_EYELEVEL;
    m_speed = 0.f;
    m_jumpSpeed = 0.f;
    m_gravity = -100.f;
    m_jumpHeight = 0.f;

    m_movementState = MOVEMENT_STATE_IDLE;
    m_heightState = HEIGHT_STATE_STANDING;
}

// Get position x of the player
Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

// Get the terrain for the player info
GroundEntity* CPlayerInfo::GetTerrain(void)
{
	return m_pTerrain;
}

/********************************************************************************
 Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

    m_movementState = MOVEMENT_STATE_IDLE;
    velocity.SetZero(); // get the velocity every frame

    Vector3 viewVector = target - position;
    viewVector.y = 0.f;
    viewVector.Normalize();

	// Update the position if the WASD buttons were activated
	if (KeyboardController::GetInstance()->IsKeyDown('W') ||
		KeyboardController::GetInstance()->IsKeyDown('A') ||
		KeyboardController::GetInstance()->IsKeyDown('S') ||
		KeyboardController::GetInstance()->IsKeyDown('D'))
	{
        m_movementState = MOVEMENT_STATE_WALK;

		if (KeyboardController::GetInstance()->IsKeyDown('W'))
		{
            velocity += viewVector.Normalized();
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('S'))
		{
            velocity -= viewVector.Normalized();
		}
		if (KeyboardController::GetInstance()->IsKeyDown('A'))
		{
            Vector3 right = viewVector.Cross(up);
            right.y = 0;
            right.Normalize();
            velocity -= right;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('D'))
		{
            Vector3 right = viewVector.Cross(up);
            right.y = 0;
            right.Normalize();
            velocity += right;
		}
	}

    if (velocity.LengthSquared() > Math::EPSILON)
    {
        velocity.Normalize();
        m_prevVelocity = velocity;
    }
    if (KeyboardController::GetInstance()->IsKeyPressed(VK_CONTROL))
    {
        if (m_heightState == HEIGHT_STATE_STANDING)
            Crouch();
        else if (m_heightState == HEIGHT_STATE_CROUCH)
            StandUp();
    }
    if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
    {
        Jump();
    }
    if (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT) && m_heightState == HEIGHT_STATE_STANDING)
    {
        if (m_movementState != MOVEMENT_STATE_IDLE) {
            m_movementState = MOVEMENT_STATE_RUN;
        }
    }

    if (m_heightState == HEIGHT_STATE_CROUCH)
    {
        m_movementState = MOVEMENT_STATE_CROUCH;
    }

    switch (m_movementState)
    {
    case MOVEMENT_STATE_IDLE:
        //speed = 0;
        if (m_speed != 0.f)
        {
            m_speed -= 100.f * (float)(dt);
            if (m_speed < 0.f)
                m_speed = 0.f;
        }
        velocity = m_prevVelocity;
        break;

    case MOVEMENT_STATE_CROUCH:
        //speed = 5.f;
        if (m_speed > 5.f)
        {
            m_speed -= 30.f * (float)(dt);
            if (m_speed < 5.f)
                m_speed = 5.f;
        }
        else if (m_speed < 5.f)
        {
            m_speed += 30.f * (float)(dt);
            if (m_speed > 5.f)
                m_speed = 5.f;
        }
        break;

    case MOVEMENT_STATE_WALK:
        //speed = 20.f;
        if (m_speed > 20.f)
        {
            m_speed -= 40.f * (float)(dt);
            if (m_speed < 20.f)
                m_speed = 20.f;
        }
        else if (m_speed < 20.f)
        {
            m_speed += 40.f * (float)(dt);
            if (m_speed > 20.f)
                m_speed = 20.f;
        }
        break;

    case MOVEMENT_STATE_RUN:
        //speed = 60.f;
        if (m_speed > 60.f)
        {
            m_speed -= 60.f * (float)(dt);
            if (m_speed < 60.f)
                m_speed = 60.f;
        }
        else if (m_speed < 60.f)
        {
            m_speed += 60.f * (float)(dt);
            if (m_speed > 60.f)
                m_speed = 60.f;
        }
        break;
    }

    position += velocity * m_speed * (float)(dt);
    UpdatePlayerHeight(dt);

    // Constrain the position
    Constrain();
    // Update the target
    target = position + viewVector;

    //************************************
    // VIEW
    //************************************

	// Rotate the view direction
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_UP) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
	{
		Vector3 viewUV = (target - position).Normalized();
		Vector3 rightUV;
		if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		{
			float yaw = (float)m_dSpeed * (float)dt;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		{
			float yaw = (float)(-m_dSpeed * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			float pitch = (float)(m_dSpeed * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			float pitch = (float)(-m_dSpeed * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
	}

	//Update the camera direction based on mouse move
	{
		Vector3 viewUV = (target - position).Normalized();
		Vector3 rightUV;

		{
			float yaw = (float)(-m_dSpeed * camera_yaw * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		{
			float pitch = (float)(-m_dSpeed * camera_pitch * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
	}

	// Update the weapons
	if (KeyboardController::GetInstance()->IsKeyReleased('R'))
	{
		if (primaryWeapon)
		{
			primaryWeapon->Reload();
			//primaryWeapon->PrintSelf();
		}
		if (secondaryWeapon)
		{
			secondaryWeapon->Reload();
			//secondaryWeapon->PrintSelf();
		}
	}
	if (primaryWeapon)
		primaryWeapon->Update(dt);
	if (secondaryWeapon)
		secondaryWeapon->Update(dt);

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
		if (primaryWeapon)
			primaryWeapon->Discharge(position, target, this);
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		if (secondaryWeapon)
			secondaryWeapon->Discharge(position, target, this);
	}

	// If the user presses P key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		Reset();
	}

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->SetCameraPos(position);
		attachedCamera->SetCameraTarget(target);
		attachedCamera->SetCameraUp(up);
	}
}

void CPlayerInfo::UpdatePlayerHeight(const double dt)
{
    switch (m_heightState)
    {
    case HEIGHT_STATE_STANDING:
        UpdateStandUp(dt);
        break;

    case HEIGHT_STATE_CROUCH:
        UpdateCrouch(dt);
        break;

    case HEIGHT_STATE_JUMP:
        UpdateJump(dt);
        break;
    }
    position.y = m_eyeLevel + m_jumpHeight;
}


void CPlayerInfo::Crouch()
{
    if (m_heightState != HEIGHT_STATE_CROUCH && m_eyeLevel == m_STAND_EYELEVEL)
    {
        m_heightState = HEIGHT_STATE_CROUCH;
    }

    if (m_heightState == HEIGHT_STATE_CROUCH && m_eyeLevel == m_CROUCH_EYELEVEL)
    {
        m_heightState = HEIGHT_STATE_STANDING;
    }
}

void CPlayerInfo::StandUp()
{
    if (m_heightState != HEIGHT_STATE_STANDING && m_eyeLevel == m_CROUCH_EYELEVEL)
    {
        m_heightState = HEIGHT_STATE_STANDING;
    }
}

void CPlayerInfo::Jump()
{
    if (m_heightState != HEIGHT_STATE_JUMP) {
        m_heightState = HEIGHT_STATE_JUMP;

        m_jumpSpeed = 30.f;    //dt not needed
    }
}

void CPlayerInfo::UpdateStandUp(const double dt)
{
    if (m_eyeLevel < 25.f)
    {
        m_eyeLevel += (float)(20.f * dt);
        m_eyeLevel = Math::Min(m_STAND_EYELEVEL, m_eyeLevel);
    }
}

void CPlayerInfo::UpdateCrouch(const double dt)
{
    m_eyeLevel -= (float)(20.f * dt);
    m_eyeLevel = Math::Max(m_CROUCH_EYELEVEL, m_eyeLevel);
}

void CPlayerInfo::UpdateJump(const double dt)
{
    // Factor in gravity
    m_jumpSpeed += (float)(m_gravity * dt);

    // Update camera and target position

    m_jumpHeight += m_jumpSpeed * (float)dt;

    // Check if camera reached the ground
    float newHeight = position.y + m_jumpHeight;  //player would be in the ground already
    if (newHeight + 1.f <= position.y)
    {
        // Camera landing on ground
        position.y = newHeight;

        //when landing, reduce MoveVel (impact)
        //m_speed = Math::Max(0.f, m_speed - 20.f);

        // Reset values
        m_jumpSpeed = 0.f;
        m_heightState = HEIGHT_STATE_STANDING;
        m_jumpHeight = 0.f;
    }
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	//if (position.y > maxBoundary.y - 1.0f)// Returns true if the player is on ground
    bool isOnGround(void);
    bool IsJumping();
	//	position.y = maxBoundary.y - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	//if (position.y < minBoundary.y + 1.0f)
	//	position.y = minBoundary.y + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the player is not jumping nor falling, then adjust his y position
	if (m_heightState != HEIGHT_STATE_JUMP)
	{
		// if the y position is not equal to terrain height at that position, 
		// then update y position to the terrain height
		if (position.y != m_pTerrain->GetTerrainHeight(position))
			position.y = m_pTerrain->GetTerrainHeight(position) + m_eyeLevel;
	}
}

void CPlayerInfo::AttachCamera(FPSCamera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}
