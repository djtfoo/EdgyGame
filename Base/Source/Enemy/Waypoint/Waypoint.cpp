#include "Waypoint.h"

CWaypoint::CWaypoint()
: position(Vector3(0.f, 0.f, 0.f))
, m_iID(-1)
{
    relatedWaypoints.clear();
}

CWaypoint::~CWaypoint()
{
    relatedWaypoints.clear();
}

// position
void CWaypoint::SetPosition(const Vector3& position)
{
    this->position = position;
}

Vector3 CWaypoint::GetPosition(void) const
{
    return position;
}

// ID
void CWaypoint::SetID(const int id)
{
    this->m_iID = id;
}

int CWaypoint::GetID(void) const
{
    return m_iID;
}

void CWaypoint::AddRelatedWaypoint(CWaypoint* relatedWaypoint)
{
    relatedWaypoints.push_back(relatedWaypoint);
}

bool CWaypoint::RemoveRelatedWaypoint(const int id)
{
    if (relatedWaypoints.size() != 0)
    {
        std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
        while (it != relatedWaypoints.end())
        {
            if ((*it)->GetID() == m_iID)
            {
                it = relatedWaypoints.erase(it);
                return true;
            }
            ++it;
        }
    }

    return false;
}

CWaypoint* CWaypoint::GetNearestWaypoint(void)
{
    CWaypoint* nearestWaypoint = NULL;
    float m_fDistance = std::numeric_limits<float>::max();

    if (relatedWaypoints.size() != 0)
    {
        std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
        while (it != relatedWaypoints.end())
        {
            Vector3 waypoint = (*it)->GetPosition();

            float xDist = position.x - waypoint.x;
            float zDist = position.z - waypoint.z;
            float distSquared = (float)(xDist * xDist + zDist * zDist);
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

int CWaypoint::GetNumberOfWaypoints(void) const
{
    return relatedWaypoints.size();
}

void CWaypoint::PrintSelf(void)
{
    std::cout << "Waypoint ID    : " << m_iID << std::endl;
    std::cout << " position      : " << position << std::endl;
    std::cout << " related waypoints : <";

    if (relatedWaypoints.size() != 0)
    {
        std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
        while (it != relatedWaypoints.end())
        {
            std::cout << (*it)->GetID() << std::endl;
            ++it;
        }
    }
    else
        std::cout << "NIL" << std::endl;
}