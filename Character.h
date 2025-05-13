#pragma once

#include <chrono>
#include <set>

#include "GameObject.h"
#include "AnimationComponent.h"
#include "Commons.h"
#include "Constants.h"

// Structs
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
	void StopHorizontalMovement();
	void StopJumpingMovement();
	void StopFalling();
	virtual void Despawn();

	// Accessor functions
	bool GetIsSpawning();
	bool GetIsFalling();
	sf::Vector2f GetPosition();
	sf::Vector2f GetVelocity() const;
	bool GetCanCollide() const;
	bool GetHasRider() const;
	bool GetIsAlive() const;
	int GetPointsValue() const;
	bool GetIsFacingRight() const;

	// Modifier functions
	void SetVelocity(const sf::Vector2f newVelocity);
	void SetPosition(float xPos, float yPos);
	void FlipSprite();
	void ResetCollisionCooldown();
	virtual void AddNewForce(sf::Vector2f forceToAdd);

	// Update Functions
	virtual void Update(float deltaTime);
	void UpdateCollisionCooldown(float deltaTime);
	virtual void UpdateForceDecrement();

	// Render functuibs
	void Render(sf::RenderTarget& target) override;

protected:
	// Vectors
	sf::Vector2f m_feetPosition;
	sf::Vector2f m_velocity;
	sf::Vector2f m_spawnPosition;
	sf::Vector2f m_dynamicForce;

	// Bools
	bool m_isJumping = false;
	bool m_isFacingRight = true;
	bool m_isMoving = false;
	bool m_isGrounded = true;
	bool m_isFalling = false;
	bool m_isSpawning = false;
	bool m_isFlying = false;
	bool m_isAlive = true;
	bool m_canJump = true;
	bool m_canFall = false;
	bool m_canCollide = false;
	bool m_hasRider = true;

	// Floats
	float m_movementSpeed;
	float m_jumpForce;
	float m_riderXOffset = 6.f;
	float m_riderYOffset = 0.f;

	// Ints
	int m_riderIndex = 0;
	int m_pointValue = POINTS_VALUE_BOUNDER;

	// Strings
	std::string debugName = "DefaultName"; // For debug

	// Sprites
	sf::Sprite m_riderSprite;

	// Components
	std::unique_ptr<LLGP::AnimationComponent> m_animationComponent;

	// Maps
	std::unordered_map<std::string, sf::IntRect> m_characterSprites;
	std::unordered_map<LLGP::AnimationState, Animation> m_animations;

	// Functions
	virtual void InitVariables();
	void UpdateFeetPosition();
	virtual void InitAnimations() = 0;
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
	virtual void SetRiderPosition();

	// Physics functions
	void AddGravity();

	// Update functions
	virtual void UpdateWindowsBoundCollision();

private:

	// Floats
	float m_collisionCooldown = 0.f;
	const float m_maxCollisionCooldown = 0.015f;

	// Functions
	void CreateAnimationComponent(sf::Sprite& sprite, const std::string& name);
};

