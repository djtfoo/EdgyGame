#include "WaypointManager.h"
#include <iostream>

CWaypointManager* CWaypointManager::s_instance = 0;

CWaypointManager::CWaypointManager() : m_iID(-1)
{
    listOfWaypoints.clear();
}

CWaypointManager::~CWaypointManager()
{
    if (listOfWaypoints.size() != 0)
    {
        while (!listOfWaypoints.empty())
        {
            CWaypoint* waypoint = listOfWaypoints.back();
            delete waypoint;
            listOfWaypoints.pop_back();
        }
    }
}

int CWaypointManager::AddWaypoint(const Vector3& position)
{
    ++m_iID;
    CWaypoint* waypoint = new CWaypoint();
    waypoint->SetID(m_iID);
    waypoint->SetPosition(position);
    listOfWaypoints.push_back(waypoint);

    return m_iID;
}

int CWaypointManager::AddWaypoint(const int id, const Vector3& position)
{
    if (listOfWaypoints.size() != 0)
    {
        for (unsigned i = 0; i < listOfWaypoints.size(); ++i)
        {
            CWaypoint* waypoint = listOfWaypoints[i];

            if (waypoint->GetID() == id)
            {
                ++(this->m_iID);
                CWaypoint* newWaypoint = new CWaypoint();
                newWaypoint->SetID(this->m_iID);
                newWaypoint->SetPosition(position);
                listOfWaypoints.push_back(newWaypoint);

                newWaypoint->AddRelatedWaypoint(waypoint);
                waypoint->AddRelatedWaypoint(newWaypoint);

                return this->m_iID;
            }
        }
    }

    return -1;
}

bool CWaypointManager::RemoveWaypoint(const int id)
{
    bool result = false;

    if (listOfWaypoints.size() != 0)
    {
        std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
        while (it != listOfWaypoints.end())
        {
            if ((*it)->GetID() == id)
            {
                RemoveRelatedWaypoint(id);
                delete *it;
                it = listOfWaypoints.erase(it);
                result = true;
                break;
            }
            if (it != listOfWaypoints.end())
                ++it;
        }
    }

    return result;
}

CWaypoint* CWaypointManager::GetWaypoint(const int id)
{
    if (listOfWaypoints.size() != 0)
    {
        std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
        while (it != listOfWaypoints.end())
        {
            if ((*it)->GetID() == id)
            {
                return (CWaypoint*)(*it);
            }
            ++it;
        }
    }

    return NULL;
}

CWaypoint* CWaypointManager::GetNearestWaypoint(const Vector3& position)
{
    CWaypoint* nearestWaypoint = NULL;
    float m_fDistance = std::numeric_limits<float>::max();

    if (listOfWaypoints.size() != 0)
    {
        std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
        while (it != listOfWaypoints.end())
        {
            Vector3 relatedWaypoint = (*it)->GetPosition();

            float xDist = position.x - relatedWaypoint.x;
            float zDist = position.z - relatedWaypoint.z;
            float distSquared = (float)(xDist*xDist + zDist*zDist);
            if (m_fDistance > distSquared)
            {
                m_fDistance = distSquared;
                nearestWaypoint = (CWaypoint*)(*it);
            }
            ++it;
        }
    }

    return nearestWaypoint;
}

int CWaypointManager::GetNumberOfWaypoints(void) const
{
    return listOfWaypoints.size();
}

void CWaypointManager::PrintSelf(void)
{
    std::cout << "=========================================" << std::endl;
    std::cout << "CWaypointManager::PrintSelf" << std::endl;
    
    if (listOfWaypoints.size() != 0)
    {
        std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
        while (it != listOfWaypoints.end())
        {
            (*it)->PrintSelf();
            ++it;
        }
    }

    std::cout << "=========================================" << std::endl;
}

void CWaypointManager::RemoveRelatedWaypoint(const int id)
{
    // this function is wrong lolz

    //std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
    //while (it != listOfWaypoints.end())
    //{
    //    (*it)->RemoveRelatedWaypoint(id);
    //    ++it;
    //}
}