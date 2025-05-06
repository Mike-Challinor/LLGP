#pragma once

#include "Character.h"

class Enemy : public Character
{
public:
	// Constructors and Destructors
	Enemy(LLGP::AssetRegistry& assetRegistry, float xPos = 100.f, float yPos = 100.f, const std::string& objectName = "enemy");
	virtual ~Enemy();

	// Update functions
	virtual void Update(float deltaTime) override;

	// Functions
	virtual void DecideNextMove();
	void ResetTarget();

private:
	// Vectors
	sf::Vector2f m_targetPosition;

	// Init functions
	void InitAnimations() override;
	void InitVariables() override;


protected:
	// Floats
	float m_verticalMovementSpeed;

	// Bools
	bool m_hasCollided = false;

	// Mutator Functions
	void SetTarget(sf::Vector2f targetPosition);
	void MoveToTarget();

	// Functions
	virtual void FindTarget();

};

