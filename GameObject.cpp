#include "GameObject.h"

GameObject::GameObject(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName) : 
	m_sprite(m_texture), m_assetRegistry(assetRegistry), m_objectName(objectName)
{
	m_sprite.setPosition(sf::Vector2f(xPos, yPos)); // Set initial position

	// Create collision shapes
	auto bodyCollision = std::make_unique<CollisionComponent>();
	bodyCollision->AddCollisionShape("body", m_sprite.getGlobalBounds(), CollisionType::Solid);
	SetCollisionComponent(std::move(bodyCollision));

	// Set the objects name
	m_objectName = objectName;

	// Load the texture map
	m_texture = assetRegistry.LoadTexture();	
	m_sprite.setTexture(m_texture);
	m_textureMap = assetRegistry.LoadSprites(m_objectName);
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
