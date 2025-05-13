#include "CollisionComponent.h"

// --- Function that returns the world bounds of the shape ---
sf::FloatRect CollisionComponent::getWorldBounds(const std::string& name, const sf::Vector2f& entityPosition) const
{
    auto it = m_shapes.find(name);
    if (it != m_shapes.end())
    {
        const auto& shape = it->second;

        return sf::FloatRect(
            entityPosition + shape.bounds.position,
            shape.bounds.size
        );

    }
    return {};
}

// --- Function for adding a collision shape to the component ---
void CollisionComponent::AddCollisionShape(const std::string& name, const sf::FloatRect& bounds, CollisionType type)
{
    m_shapes[name] = CollisionShape{ bounds, type };
}

// --- Function for removing a collision shape to the component ---
void CollisionComponent::RemoveCollisionShape(const std::string& name)
{
    m_shapes.erase(name);
}

//  --- Function for returning all of the collision shapes ---
const std::unordered_map<std::string, CollisionShape>& CollisionComponent::getShapes() const
{
    return m_shapes;
}
