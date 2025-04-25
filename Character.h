#pragma once

#include <chrono>
#include <set>

#include "GameObject.h"
#include "AnimationComponent.h"
#include "Commons.h"
#include "Constants.h"

struct Animation
{
	int numberOfFrames;
	int startingFrame;
};

class Character : public GameObject
{
public:
	// Constructor & Destructor
	Character(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName);
	virtual ~Character();

	// Functions
	void Spawn();

	// Mutation functions
	void SetPosition(float xPos, float yPos);
	void StopHorizontalMovement();
	void StopJumpingMovement();
	void StopFalling();

	// Accessor functions
	bool GetIsSpawning();
	bool GetIsFalling();
	sf::Vector2f GetPosition();

	virtual void Update(float deltaTime);

protected:
	// Vectors
	sf::Vector2f m_feetPosition;
	sf::Vector2f m_velocity;
	sf::Vector2f m_spawnPosition;

	//Bools
	bool m_isJumping = false;
	bool m_canJump = true;
	bool m_canFall = false;
	bool m_isFacingRight = true;
	bool m_isMoving = false;
	bool m_isGrounded = true;
	bool m_isFalling = false;
	bool m_isSpawning = false;
	bool m_isFlying = false;

	// Floats
	float m_movementSpeed;
	float m_jumpForce;

	// Components
	std::unique_ptr<LLGP::AnimationComponent> m_animationComponent;

	// Maps
	std::unordered_map<std::string, sf::IntRect> m_characterSprites;
	std::unordered_map<LLGP::AnimationState, Animation> m_animations;

	// Functions
	virtual void InitVariables();
	void UpdateFeetPosition();
	virtual void InitAnimations() = 0;
	void FlipSprite();
	void SetAnimationState();
	void StopSpawning();

	// Collision check functions
	bool CheckLeftColl();
	bool CheckRightColl();
	bool CheckTopColl();
	bool CheckFeetColl();

	// Movement functions
	void Move();
	void Jump();
	void ReduceJumpForce();
	virtual void UpdateMovementDirection();
	void MoveToSpawnPosition();

	// Physics functions
	void AddGravity();

	// Update functions
	void UpdateWindowsBoundCollision();

private:

	void CreateAnimationComponent(sf::Sprite& sprite, const std::string& name);
};

