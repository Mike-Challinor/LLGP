#include "GameObject.h"

// --- Constructor ---
GameObject::GameObject(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName) : 
	m_sprite(m_texture), m_assetRegistry(assetRegistry), m_objectName(objectName)
{
	// Set the base sprites initial position
	m_sprite.setPosition(sf::Vector2f(xPos, yPos));

	// Create collision shapes
	auto bodyCollision = std::make_unique<CollisionComponent>();
	bodyCollision->AddCollisionShape("body", m_sprite.getGlobalBounds(), CollisionType::Solid);
	SetCollisionComponent(std::move(bodyCollision));

	// Set the objects name
	m_objectName = objectName;

	// Set the texture
	SetTexture();
}

// --- Destructor ---
GameObject::~GameObject()
{
}

// --- Function that returns the collision component ---
CollisionComponent* GameObject::GetCollisionComponent() const
{
	return m_collisionComponent.get();
}

// --- Function that returns the collision bounds of the base sprite ---
sf::FloatRect GameObject::GetCollisionBounds()
{
	return m_sprite.getGlobalBounds();
}

// --- Function that returns the global position of the base sprite ---
sf::Vector2f GameObject::GetPosition()
{
	return m_sprite.getGlobalBounds().position;
}

// --- Function that returns the size of the main sprite ---
sf::Vector2f GameObject::GetSize()
{
	return m_sprite.getGlobalBounds().size;
}

// --- Function that sets the collision component ---
void GameObject::SetCollisionComponent(std::unique_ptr<CollisionComponent> component)
{
	m_collisionComponent = std::move(component);
}

// --- Function that returns the texture for the base sprite ---
void GameObject::SetTexture()
{
	m_texture = m_assetRegistry.LoadTexture();
	m_sprite.setTexture(m_texture);
	m_textureMap = m_assetRegistry.LoadSprites(m_objectName);
}

// --- Main function for rendering ---
void GameObject::Render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}
