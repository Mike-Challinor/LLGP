#pragma once

#include "GameObject.h"

class Platform : public GameObject
{
private:
	// Vectors
	sf::Vector2f m_spawnPosition;

	// Bools
	bool m_hasSpawner = false;

	// Functions
	void SetHasSpawner(const std::string& objectName);

public:

	// Constructor & Destructor
	Platform(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName = "platform");
	virtual ~Platform();

	// Accessor method
	sf::Vector2f GetSpawnPosition();
	bool GetHasSpawner();
};

