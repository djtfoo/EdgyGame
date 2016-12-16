#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile/Laser.h"
#include "SceneGraph\SceneGraph.h"

#include "BalloonBlock\GenericBalloon.h"

#include "Application.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt, CPlayerInfo* player)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();

	// Check for Collision amongst entities with collider properties
	CheckForCollision(player);

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
        if (!(*it)->GetIsInSceneGraph())
		    (*it)->Render();
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	// Render the Spatial Partition
	//if (theSpatialPartition)
	//	theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition)
{
	entityList.push_back(_newEntity);

	// Add to the Spatial Partition
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		// Remove from SceneNode too
		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity)==false)
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			// Add to the Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;	
	}
	// Return false if not found
	return false;
}

// Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	// Return false if not found
	return false;
}

// Set a handle to the Spatial Partition to this class
void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

CSpatialPartition* EntityManager::GetSpartialPartition()
{
    return theSpatialPartition;
}

// Constructor
EntityManager::EntityManager()
	: theSpatialPartition(NULL)
{
}

// Destructor
EntityManager::~EntityManager()
{
	// Drop the Spatial Partition instance
	CSpatialPartition::GetInstance()->DropInstance();

	// Clear out the Scene Graph
	CSceneGraph::GetInstance()->Destroy();
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
    return (thisMaxAABB.x > thatMinAABB.x &&
        thisMinAABB.x < thatMaxAABB.x &&
        thisMaxAABB.y > thatMinAABB.y &&
        thisMinAABB.y < thatMaxAABB.y &&
        thisMaxAABB.z > thatMinAABB.z &&
        thisMinAABB.z < thatMaxAABB.z);

	//// Check if this object is overlapping that object
	///*
	//if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	//(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	//(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	//||
	//((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	//(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	//(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	//*/
	//if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
	//	||
	//	((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	//{
	//	return true;
	//}
    //
	//// Check if that object is overlapping this object
	///*
	//if (((thisMinAABB.x >= thatMinAABB.x) && (thisMinAABB.x <= thatMaxAABB.x) &&
	//(thisMinAABB.y >= thatMinAABB.y) && (thisMinAABB.y <= thatMaxAABB.y) &&
	//(thisMinAABB.z >= thatMinAABB.z) && (thisMinAABB.z <= thatMaxAABB.z))
	//||
	//((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	//(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	//(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	//*/
	//if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
	//	||
	//	((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	//{
	//	return true;
	//}
    //
	//// Check if this object is within that object
	///*
	//if (((thisMinAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	//(thisMinAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	//(thisMinAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z))
	//&&
	//((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	//(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	//(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	//*/
	//if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
	//	&&
	//	((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	//	return true;
    //
	//// Check if that object is within this object
	///*
	//if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	//(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	//(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	//&&
	//((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	//(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	//(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	//*/
	//if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
	//	&&
	//	((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	//	return true;
    //
	//return false;
}

bool EntityManager::CheckPointToAABB(const Vector3& point, const Vector3& minAABB, const Vector3& maxAABB)
{
    // Check if point is within the AABB bounding box
    //if ((point >= minAABB) && (point <= maxAABB))
    //{
    //    return true;
    //}

    if (point.x >= minAABB.x && point.x <= maxAABB.x &&
        point.y >= minAABB.y && point.y <= maxAABB.y &&
        point.z >= minAABB.z && point.z <= maxAABB.z)
        return true;
    
    return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check where a line segment between two positions intersects a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f) 
		return false;
	if (fDst1 == fDst2) 
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check two positions are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(	Vector3 line_start, Vector3 line_end, 
											Vector3 minAABB, Vector3 maxAABB,
											Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

// Check for Player Collision - Advanced version
/*
bool EntityManager::CheckPlayerCollision(const double dt, const Vector3& point, CPlayerInfo* player)
{
    std::vector<EntityBase*> objectsInGrid = theSpatialPartition->GetObjects(point, 1.f);

    std::vector<EntityBase*>::iterator colliderThis, colliderThisEnd;
    colliderThisEnd = objectsInGrid.end();

    for (colliderThis = objectsInGrid.begin(); colliderThis != colliderThisEnd; ++colliderThis)
    {
        // Check if this entity is a Balloon type
        if (((*colliderThis)->GetIsInSceneGraph() || (*colliderThis)->GetIsLowResRender()) && (*colliderThis)->HasCollider())
        {
            CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);

            Vector3 minAABB, maxAABB;

            if ((*colliderThis)->GetIsInSceneGraph()) {
                Mtx44 transformMtx;
                transformMtx.SetToIdentity();
                CSceneNode* node = CSceneGraph::GetInstance()->GetNode(*colliderThis);
                if (node->GetParent() != NULL) {
                    transformMtx = node->GetParent()->GetTransform() * node->GetTransform();
                }
                else {
                    transformMtx = node->GetTransform();
                }

                minAABB = transformMtx * ((*colliderThis)->GetPosition() + thisCollider->GetMinAABB());
                maxAABB = transformMtx * ((*colliderThis)->GetPosition() + thisCollider->GetMaxAABB());
            }
            else {
                minAABB = (*colliderThis)->GetPosition() + thisCollider->GetMinAABB();
                maxAABB = (*colliderThis)->GetPosition() + thisCollider->GetMaxAABB();
            }

            // Create AABB for player
            //Vector3 playerMinAABB, playerMaxAABB;
            //playerMinAABB = point + Vector3(-3, -1, -3);
            //playerMaxAABB = point + Vector3(3, 2, 3);

            //if (CheckOverlap(playerMinAABB, playerMaxAABB, minAABB, maxAABB)) {
            //    return true;
            //}
            if (CheckPointToAABB(point, minAABB, maxAABB)) {

                if ((*colliderThis)->GetName() == "GenericBalloon")
                {
                    CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(*colliderThis);
                    std::vector<CSceneNode*> children = theNode->GetChildrenList();
                    for (std::vector<CSceneNode*>::iterator it = children.begin(); it < children.end(); ++it)
                    {
                        CSceneNode* childNode = *it;
                        EntityBase* entity = childNode->GetEntity();

                        CCollider *collider = dynamic_cast<CCollider*>(entity);

                        Vector3 minAABB2, maxAABB2;

                        if (entity->GetIsInSceneGraph()) {
                            Mtx44 transformMtx;
                            transformMtx.SetToIdentity();
                            if (childNode->GetParent() != NULL) {
                                transformMtx = childNode->GetParent()->GetTransform() * childNode->GetTransform();
                            }
                            else {
                                transformMtx = childNode->GetTransform();
                            }

                            minAABB2 = transformMtx * (entity->GetPosition() + collider->GetMinAABB());
                            maxAABB2 = transformMtx * (entity->GetPosition() + collider->GetMaxAABB());
                        }
                        else {
                            minAABB2 = entity->GetPosition() + collider->GetMinAABB();
                            maxAABB2 = entity->GetPosition() + collider->GetMaxAABB();
                        }

                        if (CheckPointToAABB(point, minAABB2, maxAABB2)) {
                            // INSERT COLLISION RESPONSE HERE

                            std::cout << "HEY";

                            if (player->GetHeightState() == CPlayerInfo::HEIGHT_STATE_JUMP)
                            {
                                float VelocityLoss = Math::Max(1.5f, player->GetJumpSpeed() / 2.f);

                                player->SetJumpSpeed(-player->GetJumpSpeed() - VelocityLoss);
                                if (player->GetJumpSpeed() > 0.f)
                                    return false;   // don't obstruct player movement
                                else
                                    return true;    // obstruct player movement
                            }
                            //else if (player->GetMovementState() == CPlayerInfo::MOVEMENT_STATE_RUN && point.y <= Math::EPSILON)
                            //{
                            //    float force_pushback = 5000.f;
                            //    player->SetMovementSpeed(player->GetMovementSpeed() + force_pushback * (float)(dt));
                            //    std::cout << player->GetMovementSpeed();
                            //    return false;
                            //}
                        }
                    }

                }
                else if ((*colliderThis)->GetName() == "Diamond") {

                    // COLLISION RESPONSE HERE
                    (*colliderThis)->SetIsDone(true);
                    player->SetDiamondsLeftToCollect(player->GetDiamondsLeftToCollect() - 1);

                    // Remove from Spatial Partitioning
                    if (theSpatialPartition->Remove(*colliderThis) == true)
                    {
                        cout << "*** REMOVED from Spatial Partitioning ***" << endl;
                    }

                    std::cout << "COLLISION" << std::endl;

                    return false;   // don't obstruct player movement
                }

                return true;
            }
        }
    }

    return false;
}
*/

// Check for Player Collision
bool EntityManager::CheckPlayerCollision(const double dt, const Vector3& point, CPlayerInfo* player)
{
    std::vector<EntityBase*> objectsInGrid = theSpatialPartition->GetObjects(point, 1.f);

    std::vector<EntityBase*>::iterator colliderThis, colliderThisEnd;
    colliderThisEnd = objectsInGrid.end();

    for (colliderThis = objectsInGrid.begin(); colliderThis != colliderThisEnd; ++colliderThis)
    {
        // Check if this entity is a Balloon type
        if ((*colliderThis)->HasCollider())
        {
            CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);

            Vector3 minAABB, maxAABB;

            if ((*colliderThis)->GetIsInSceneGraph()) {
                Mtx44 transformMtx;
                transformMtx.SetToIdentity();
                CSceneNode* node = CSceneGraph::GetInstance()->GetNode(*colliderThis);
                if (node->GetParent() != NULL) {
                    transformMtx = node->GetParent()->GetTransform() * node->GetTransform();
                }
                else {
                    transformMtx = node->GetTransform();
                }

                minAABB = transformMtx * ((*colliderThis)->GetPosition() + thisCollider->GetMinAABB());
                maxAABB = transformMtx * ((*colliderThis)->GetPosition() + thisCollider->GetMaxAABB());
            }
            else {
                minAABB = (*colliderThis)->GetPosition() + thisCollider->GetMinAABB();
                maxAABB = (*colliderThis)->GetPosition() + thisCollider->GetMaxAABB();
            }

            // Create AABB for player
            //Vector3 playerMinAABB, playerMaxAABB;
            //playerMinAABB = point + Vector3(-3, -1, -3);
            //playerMaxAABB = point + Vector3(3, 2, 3);

            //if (CheckOverlap(playerMinAABB, playerMaxAABB, minAABB, maxAABB)) {
            //    return true;
            //}
            if (CheckPointToAABB(point, minAABB, maxAABB)) {

                if ((*colliderThis)->GetName() == "GenericBalloon")
                {
                    // INSERT COLLISION RESPONSE HERE
                    if (player->GetHeightState() == CPlayerInfo::HEIGHT_STATE_JUMP)
                    {
                        float VelocityLoss = Math::Max(1.5f, player->GetJumpSpeed() / 2.f);

                        player->SetJumpSpeed(-player->GetJumpSpeed() - VelocityLoss);
                        if (player->GetJumpSpeed() > 0.f)
                            return false;   // don't obstruct player movement
                        else
                            return true;    // obstruct player movement
                    }
                    //else if (player->GetMovementState() == CPlayerInfo::MOVEMENT_STATE_RUN && point.y <= Math::EPSILON)
                    //{
                    //    float force_pushback = 5000.f;
                    //    player->SetMovementSpeed(player->GetMovementSpeed() + force_pushback * (float)(dt));
                    //    std::cout << player->GetMovementSpeed();
                    //    return false;
                    //}

                }
                else if ((*colliderThis)->GetName() == "Diamond") {
                    
                    // COLLISION RESPONSE HERE
                    (*colliderThis)->SetIsDone(true);
                    player->SetDiamondsLeftToCollect(player->GetDiamondsLeftToCollect() - 1);

                    Application::GetInstance().m_soundEngine->play2D("Sound//SE_diamond pickup.wav");

                    // Remove from Spatial Partitioning
                    if (theSpatialPartition->Remove(*colliderThis) == true)
                    {
                        cout << "*** REMOVED from Spatial Partitioning ***" << endl;
                    }

                    std::cout << "COLLISION" << std::endl;

                    return false;   // don't obstruct player movement
                }
                else if ((*colliderThis)->GetName() == "Ammo") {

                    // COLLISION RESPONSE HERE
                    if (player->GetHeldWeapon()->GetCurrentWeapon()->GetMaxTotalRound() > (player->GetHeldWeapon()->GetCurrentWeapon()->GetTotalRound() + player->GetHeldWeapon()->GetCurrentWeapon()->GetMagRound()))
                    {
                        player->GetHeldWeapon()->GetCurrentWeapon()->SetTotalRound(player->GetHeldWeapon()->GetCurrentWeapon()->GetMaxTotalRound() - 1);
                        player->GetHeldWeapon()->GetCurrentWeapon()->SetMagRound(1);

                        (*colliderThis)->SetIsDone(true);

                        Application::GetInstance().m_soundEngine->play2D("Sound//SE_ammo pickup.wav");

                        // Remove from Spatial Partitioning
                        if (theSpatialPartition->Remove(*colliderThis) == true)
                        {
                            cout << "*** REMOVED from Spatial Partitioning ***" << endl;
                        }

                        std::cout << "COLLISION" << std::endl;
                    }

                    return false;   // don't obstruct player movement regardless
                }

                return true;
            }
        }
    }

    return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(CPlayerInfo* player)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	//std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
            std::vector<EntityBase*> objectsInGrid = theSpatialPartition->GetObjects((*colliderThis)->GetPosition(), 1.f);

            std::vector<EntityBase*>::iterator colliderThat, colliderThatEnd;
            colliderThatEnd = objectsInGrid.end();

            for (colliderThat = objectsInGrid.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
                if ((*colliderThat)->GetName() == "GenericBalloon" && (*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0, 0, 0);

					// Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);

                    //Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
                    //Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();
                    Vector3 thatMinAABB, thatMaxAABB;

                    if ((*colliderThat)->GetIsInSceneGraph()) {
                        Mtx44 transformMtx;
                        transformMtx.SetToIdentity();
                        CSceneNode* node = CSceneGraph::GetInstance()->GetNode(*colliderThat);
                        if (node->GetParent() != NULL) {
                            transformMtx = node->GetParent()->GetTransform() * node->GetTransform();
                        }
                        else {
                            transformMtx = node->GetTransform();
                        }
                        
                        thatMinAABB = transformMtx * ((*colliderThat)->GetPosition() + thatCollider->GetMinAABB());
                        thatMaxAABB = transformMtx * ((*colliderThat)->GetPosition() + thatCollider->GetMaxAABB());
                    }
                    else {
                        thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
                        thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();
                    }

                    //std::cout << thatMinAABB.x << " | " << thatMinAABB.y << " | " << thatMinAABB.z << " | " << thatMaxAABB.x << " | " << thatMaxAABB.y << " | " << thatMaxAABB.z << std::endl;

					if (CheckLineSegmentPlane(	thisEntity->GetPosition(), 
												thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
												thatMinAABB, thatMaxAABB,
												hitPosition) == true)
					{
                        (*colliderThis)->SetIsDone(true);
                        GenericBalloon* balloon = dynamic_cast<GenericBalloon*>(*colliderThat);

                        balloon->SetState(GenericBalloon::DEFLATING);
                        // Remove from Spatial Partitioning
                        if (theSpatialPartition->Remove(*colliderThat) == true)
                        {
                            cout << "*** REMOVED from Spatial Partitioning ***" << endl;
                        }
                        Application::PlayBalloonDeflateSE(player->GetPos(), player->GetTarget(), (*colliderThat)->GetPosition());

						//(*colliderThis)->SetIsDone(true);
						//(*colliderThat)->SetIsDone(true);
                        //
                        //std::cout << "COLLISION" << std::endl;
                        //
						//// Remove from Scene Graph
						//if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
						//{
						//	cout << "*** This Entity removed ***" << endl;
						//}
						//// Remove from Scene Graph
						//if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
						//{
						//	cout << "*** That Entity removed ***" << endl;
						//}


                        break;
					}
				}
			}
		}
		//else if ((*colliderThis)->HasCollider())
		//{
		//	// This object was derived from a CCollider class, then it will have Collision Detection methods
		//	//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
		//	EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);
        //
		//	// Check for collision with another collider class
		//	colliderThatEnd = entityList.end();
		//	int counter = 0;
		//	for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
		//	{
		//		if (colliderThat == colliderThis)
		//			continue;
        //
		//		if ((*colliderThat)->HasCollider())
		//		{
		//			EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
		//			if (CheckSphereCollision(thisEntity, thatEntity))
		//			{
		//				if (CheckAABBCollision(thisEntity, thatEntity))
		//				{
		//					//thisEntity->SetIsDone(true);
		//					//thatEntity->SetIsDone(true);
        //
       ////                     // Remove from Scene Graph
       ////                     if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
       ////                     {
       ////                         cout << "*** This Entity removed ***" << endl;
       ////                     }
       ////                     // Remove from Scene Graph
       ////                     if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
       ////                     {
       ////                         cout << "*** That Entity removed ***" << endl;
       ////                     }
		//				}
		//			}
		//		}
		//	}
		//}
	}
	return false;
}
