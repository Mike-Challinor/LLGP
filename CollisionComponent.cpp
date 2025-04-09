#include "CollisionComponent.h"

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

void CollisionComponent::AddCollisionShape(const std::string& name, const sf::FloatRect& bounds, CollisionType type)
{
    m_shapes[name] = CollisionShape{ bounds, type };
}

void CollisionComponent::RemoveCollisionShape(const std::string& name)
{
    m_shapes.erase(name);
}

const std::unordered_map<std::string, CollisionShape>& CollisionComponent::getShapes() const
{
    return m_shapes;
}
