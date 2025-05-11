#pragma once
#include <SFML/Graphics.hpp>

struct Waypoint {
    sf::Vector2f position;
    bool isActivated;  // To know if the waypoint has been reached

    Waypoint(float x, float y) : position(x, y), isActivated(false) {}
};
