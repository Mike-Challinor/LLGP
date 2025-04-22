#pragma once

#include <SFML/Graphics.hpp>

#include "AssetRegistry.h"
#include "CollisionComponent.h"

class GameObject
{
public:

	// Constructor & Destructor
	GameObject(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos);
	virtual ~GameObject();

	// Accessors and Mutators
	CollisionComponent* GetCollisionComponent() const;

	// Render functions
	virtual void Render(sf::RenderTarget& target);

	// Accessor methods
	sf::FloatRect GetCollisionBounds();

protected:
	// Classes
	LLGP::AssetRegistry& m_assetRegistry; // Reference to an existing AssetRegistry

	// Shapes
	sf::Sprite m_sprite;
	sf::Texture m_texture;

	// Maps
	std::unordered_map<std::string, sf::IntRect> m_textureMap;

	// Pointers
	std::unique_ptr<CollisionComponent> m_collisionComponent;

	// Mutation methods
	void SetCollisionComponent(std::unique_ptr<CollisionComponent> component);

};

