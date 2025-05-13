#pragma once

#include "Character.h"
#include "WaypointManager.h"

class Enemy : public Character
{
public:
	// Constructors and Destructors
	Enemy(LLGP::AssetRegistry& assetRegistry, WaypointManager& waypointManager, float xPos = 100.f, float yPos = 100.f, const std::string& objectName = "enemy");
	virtual ~Enemy();

	// Update functions
	virtual void Update(float deltaTime) override;

	// Functions
	virtual void DecideNextMove(float deltaTime);
	void ResetTarget();

	// Modifier functions
	void RemoveRider();

private:
	// Vectors
	sf::Vector2f m_targetPosition;

	// Init functions
	void InitAnimations() override;
	void InitVariables() override;

	// Update functions
	void UpdateWindowsBoundCollision() override;


protected:
	// Floats
	float m_verticalMovementSpeed;
	float m_waitTime;

	// Bools
	bool m_hasCollided = false;
	bool m_hasTarget = false;

	// Classes
	WaypointManager m_waypointManager;
	Waypoint* m_currentWaypoint = nullptr;

	// Mutator Functions
	void SetTarget(sf::Vector2f targetPosition);
	void MoveTowardsTarget(const sf::Vector2f& target, float deltaTime);
	void MoveTowardsWaypoint(float deltaTime);

	// Functions
	virtual void FindTarget();
	void SetRiderPosition() override;


};

