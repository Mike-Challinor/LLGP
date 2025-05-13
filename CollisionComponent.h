#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

// Enums
enum class CollisionType {
    Solid,
    Trigger,
    Custom
};

// Structs
struct CollisionShape {
    sf::FloatRect bounds;       // Local offset and size
    CollisionType type;
};

class CollisionComponent {
public:
    // Functions
    void AddCollisionShape(const std::string& name, const sf::FloatRect& bounds, CollisionType type);
    void RemoveCollisionShape(const std::string& name);

    // Accessor functions
    const std::unordered_map<std::string, CollisionShape>& getShapes() const;
    sf::FloatRect getWorldBounds(const std::string& name, const sf::Vector2f& entityPosition) const; // This function returns the world-space collision rect
    

private:
    // Maps
    std::unordered_map<std::string, CollisionShape> m_shapes;
};

