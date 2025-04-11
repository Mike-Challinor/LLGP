#include "GameObject.h"

GameObject::GameObject(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos) : m_sprite(m_texture), m_assetRegistry(assetRegistry)
{
	m_sprite.setPosition(sf::Vector2f(xPos, yPos)); // Set initial position
}

GameObject::~GameObject()
{
}

CollisionComponent* GameObject::GetCollisionComponent() const
{
	return m_collisionComponent.get();
}

void GameObject::SetCollisionComponent(std::unique_ptr<CollisionComponent> component)
{
	m_collisionComponent = std::move(component);
}

sf::FloatRect GameObject::GetCollisionBounds()
{
	return m_sprite.getGlobalBounds();
}

void GameObject::Render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}
