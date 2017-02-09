#pragma once

#include "Vector3.h"
#include <vector>

class CWaypoint
{
protected:
    Vector3 position;
    int m_iID;

    std::vector<CWaypoint*> relatedWaypoints;

public:
    CWaypoint();
    virtual ~CWaypoint();

    // position
    void SetPosition(const Vector3& position);
    Vector3 GetPosition(void) const;

    // ID
    void SetID(const int id);
    int GetID(void) const;

    void AddRelatedWaypoint(CWaypoint* relatedWaypoint);
    bool RemoveRelatedWaypoint(const int id);
    CWaypoint* GetNearestWaypoint(void);
    int GetNumberOfWaypoints(void) const;

    void PrintSelf(void);
};