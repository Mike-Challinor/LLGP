#include "Platform.h"

Platform::Platform(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos)
	: GameObject(assetRegistry, xPos, yPos)
{
	// Get the platform spritemap
	m_texture = assetRegistry.LoadTexture();
	m_textureMap = assetRegistry.LoadPlatformSprites();

	// Set the platforms texture and rect
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(m_textureMap.begin()->second);

	// Create collision shape for the platforms shape
	auto platformCollision = std::make_unique<CollisionComponent>();
	platformCollision->AddCollisionShape("surface", sf::FloatRect{{ xPos, yPos },{ 65.f, 14.f }}, CollisionType::Solid);
	SetCollisionComponent(std::move(platformCollision));
}

Platform::~Platform()
{

}

void Platform::Render(sf::RenderTarget& target)
{
	GameObject::Render(target);
}
