#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory> 

struct Waypoint {
    sf::Vector2f position;
    bool isActivated;

    Waypoint(float x, float y) : position(x, y), isActivated(false) {}
};

class WaypointManager {
public:
    void AddWaypoint(std::shared_ptr<Waypoint> waypoint);
    Waypoint* GetNextWaypoint();
    void ResetWaypoints();
    bool HasWaypoints() const;

private:
    std::vector<std::shared_ptr<Waypoint>> m_waypoints;
};
