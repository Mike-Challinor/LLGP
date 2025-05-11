#include "WaypointManager.h"

void WaypointManager::AddWaypoint(std::shared_ptr<Waypoint> waypoint)
{
    m_waypoints.push_back(waypoint);  // No need for std::move with shared_ptr
}

Waypoint* WaypointManager::GetNextWaypoint()
{
    for (auto& waypoint : m_waypoints)
    {
        if (!waypoint->isActivated)
        {
            waypoint->isActivated = true;
            return waypoint.get();  // Return raw pointer to shared object
        }
    }
    return nullptr;
}

void WaypointManager::ResetWaypoints()
{
    for (auto& waypoint : m_waypoints) {
        waypoint->isActivated = false;
    }
}

bool WaypointManager::HasWaypoints() const
{
    for (const auto& waypoint : m_waypoints)
    {
        if (!waypoint->isActivated)
            return true;
    }
    return false;
}
