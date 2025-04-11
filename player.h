#pragma once

#include <chrono>
#include <set>

#include "Commons.h"
#include "Constants.h"
#include "InputManager.h"
#include "AnimationComponent.h"
#include "GameObject.h"

struct Animation
{
	int numberOfFrames;
	int startingFrame;
};

class player : public GameObject
{
private:
	
	// Managers
	LLGP::InputManager& m_inputManager; // Reference to an existing InputManager

	// Animations
	std::unordered_map<LLGP::AnimationState, Animation> m_animations;

	std::unordered_map<std::string, sf::IntRect> m_playerSprites;

	// Vectors
	sf::Vector2f m_direction;
	sf::Vector2f m_feetPosition;

	// Ints
	int m_playerID;

	// Floats
	float m_movementSpeed;
	float m_jumpForce;

	// Bools
	bool m_isJumping = false;
	bool m_canJump = true;
	bool isFacingRight = true;
	bool m_isMoving = false;
	bool m_isGrounded = true;

	// Strings
	std::string m_mountName;

	// Components
	unique_ptr<LLGP::AnimationComponent> m_animationComponent;

	// Lists
	std::set<LLGP::Key> m_activeKeys; // Tracks current active keys

	// Init functions
	void InitVariables();
	void InitAnimations();

	// Collision check functions
	bool CheckLeftColl();
	bool CheckRightColl();
	bool CheckTopColl();
	bool CheckFeetColl();

	// Sprite Functions
	void FlipSprite();

	// Movement functions
	void Move();
	void Jump();
	void ReduceJumpForce();
	void UpdateMovementDirection();
	void UpdateFeetPosition();

	// Physics functions
	void AddGravity();

	// Animation functions
	void SetAnimationState();

public:

	// Constructors and Destructors
	player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos = 10.f, float yPos = 10.f, int player_id = 0);
	virtual ~player();

	sf::IntRect GetSpriteRectByName(const std::string& name) const;

	// Listener functions
	void keyInputListener(LLGP::Key key);
	void OnKeyReleased(LLGP::Key key);

	// Accessor functions

	// Mutation functions
	void SetPosition(float xPos, float yPos);
	void StopHorizontalMovement();
	void StopJumpingMovement();
	void StopFalling();

	// Update functions
	void UpdateInput();
	void UpdateWindowsBoundCollision();
	void Update();

	// Render functions
	void Render(sf::RenderTarget& target) override;

};

