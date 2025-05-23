#include "Platform.h"

// --- Constructor ---
Platform::Platform(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName)
	: GameObject(assetRegistry, xPos, yPos, objectName)
{
	// Set the texture rect of the sprite 
	m_sprite.setTextureRect(m_textureMap.begin()->second);

	// Create collision shape for the platforms shape
	auto platformCollision = std::make_unique<CollisionComponent>();
	platformCollision->AddCollisionShape("surface", sf::FloatRect{{ xPos, yPos },{ 65.f, 14.f }}, CollisionType::Solid);
	SetCollisionComponent(std::move(platformCollision));

	// Set whether the platform has a spawner
	SetHasSpawner(objectName);
}

// --- Destructor ---
Platform::~Platform()
{
}

// --- Function that sets whether the platform has a spawner (based of off the name passed through) ---
void Platform::SetHasSpawner(const std::string& objectName)
{
	// Create temporary floats for the x and y pos of the spawner
	float localxPos = 0.f;

	if (objectName == "bottom_platform")
	{
		// Set the has spawner bool to true
		m_hasSpawner = true;

		// Set the local x position of the spawner
		localxPos = 160.f;

	}

	else if (objectName == "bottom_left_platform")
	{
		// Set the has spawner bool to true
		m_hasSpawner = true;

		// Set the local x position of the spawner
		localxPos = 54.f;
		
	}

	else if (objectName == "top_middle_platform")
	{
		// Set the has spawner bool to true
		m_hasSpawner = true;

		// Set the local x position of the spawner
		localxPos = 69.f;
	}

	else if (objectName == "middle_right_platform")
	{
		// Set the has spawner bool to true
		m_hasSpawner = true;

		// Set the local x position of the spawner
		localxPos = 72.f;
	}

	else
	{
		// Set the spawner bool to false
		m_hasSpawner = false;

		// Return out of the function as there is no spawner
		return;
	}

	// Set the global spawn position of the spawner
	float yPos = m_sprite.getGlobalBounds().position.y;
	float xPos = m_sprite.getGlobalBounds().position.x + localxPos;
	m_spawnPosition = sf::Vector2f(xPos, yPos);
}

// --- Function that returns the platforms spawn position ---
sf::Vector2f Platform::GetSpawnPosition()
{
	return m_spawnPosition;
}

// --- Function that returns whether the platform has a spawner ---
bool Platform::GetHasSpawner()
{
	return m_hasSpawner;
}
