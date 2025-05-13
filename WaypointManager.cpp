#include "WaypointManager.h"

// --- Function for adding a waypoint ---
void WaypointManager::AddWaypoint(std::shared_ptr<Waypoint> waypoint)
{
    m_waypoints.push_back(waypoint);
}

// --- Function that returns the next waypoint
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

// --- Function for resetting waypoints ---
void WaypointManager::ResetWaypoints()
{
    // Loop through and deactivate all waypoints
    for (auto& waypoint : m_waypoints) {
        waypoint->isActivated = false;
    }
}

// --- Function that returns whether there is an available waypoint ---
bool WaypointManager::HasWaypoints() const
{
    // Loop through and check whether there is any inactive waypoints
    for (const auto& waypoint : m_waypoints)
    {
        if (!waypoint->isActivated)
            return true;
    }
    return false;
}
