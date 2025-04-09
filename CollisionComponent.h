#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

enum class CollisionType {
    Body,
    Feet,
    Attack,
    Custom
};

struct CollisionShape {
    sf::FloatRect bounds;       // Local offset and size
    CollisionType type;
};

class CollisionComponent {
public:
    void AddCollisionShape(const std::string& name, const sf::FloatRect& bounds, CollisionType type);
    void RemoveCollisionShape(const std::string& name);
    const std::unordered_map<std::string, CollisionShape>& getShapes() const;

    // This function returns the world-space collision rect
    sf::FloatRect getWorldBounds(const std::string& name, const sf::Vector2f& entityPosition) const;
    

private:
    std::unordered_map<std::string, CollisionShape> m_shapes;
};

