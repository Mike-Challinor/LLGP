#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory> 

// Structs
struct Waypoint {
    sf::Vector2f position;
    bool isActivated;

    Waypoint(float x, float y) : position(x, y), isActivated(false) {}
};

class WaypointManager {
public:
    // Functions
    void AddWaypoint(std::shared_ptr<Waypoint> waypoint);
    void ResetWaypoints();

    // Accessor functions
    Waypoint* GetNextWaypoint();
    bool HasWaypoints() const;

private:
    // Vectors
    std::vector<std::shared_ptr<Waypoint>> m_waypoints;
};
