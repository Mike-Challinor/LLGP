#pragma once
#include "Enemy.h"

class Bounder : public Enemy
{
public:

	// Constructor
	Bounder(LLGP::AssetRegistry& assetRegistry, float xPos = 100.f, float yPos = 100.f, const std::string& objectName = "bounder");

	// Update functions
	void Update(float deltaTime) override;

private:

	// Functions
	void DecideNextMove() override;
};

