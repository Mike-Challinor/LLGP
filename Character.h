#pragma once

#include <chrono>

#include "GameObject.h"
#include "AnimationComponent.h"
#include "Commons.h"

class Character : public GameObject
{
public:
	// Constructor & Destructor
	Character(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos);
	virtual ~Character();

	// Mutation functions
	void SetPosition(float xPos, float yPos);
	void StopHorizontalMovement();
	void StopJumpingMovement();
	void StopFalling();

protected:
	// Vectors
	sf::Vector2f m_feetPosition;
	sf::Vector2f m_direction;

	//Bools
	bool m_isJumping = false;
	bool m_canJump = true;
	bool m_isFacingRight = true;
	bool m_isMoving = false;
	bool m_isGrounded = true;

	// Floats
	float m_movementSpeed;

	// Strings
	std::string m_objectName;

	// Components
	std::unique_ptr<LLGP::AnimationComponent> m_animationComponent;

	//Functions
	void InitVariables();
	void UpdateFeetPosition();
	virtual void InitAnimations();
	void FlipSprite();

	


};

