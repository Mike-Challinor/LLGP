#pragma once

#include "GameObject.h"

class Platform : public GameObject
{
public:

	// Constructor & Destructor
	Platform(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos);
	virtual ~Platform();
};

