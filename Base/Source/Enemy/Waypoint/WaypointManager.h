#pragma once

#include "Waypoint.h"
#include "Vector3.h"
#include <vector>

class CWaypointManager
{
protected:
    static CWaypointManager *s_instance;
    CWaypointManager();

    int m_iID;
    std::vector<CWaypoint*> listOfWaypoints;

    void RemoveRelatedWaypoint(const int id);

public:
    static CWaypointManager* GetInstance()
    {
        if (!s_instance)
            s_instance = new CWaypointManager;
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

    virtual ~CWaypointManager();

    // Add waypoint
    int AddWaypoint(const Vector3& position);
    int AddWaypoint(const int id, const Vector3& position);

    // Remove waypoint
    bool RemoveWaypoint(const int id);

    // Get Waypoint
    CWaypoint* GetWaypoint(const int id);
    CWaypoint* GetNearestWaypoint(const Vector3& position);
    int GetNumberOfWaypoints(void) const;

    void PrintSelf(void);
};