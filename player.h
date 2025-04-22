#pragma once

#include <chrono>
#include <set>

#include "Constants.h"
#include "InputManager.h"
#include "Character.h"

struct Animation
{
	int numberOfFrames;
	int startingFrame;
};

class player : public Character
{
private:
	
	// Managers
	LLGP::InputManager& m_inputManager; // Reference to an existing InputManager

	// Animations
	std::unordered_map<LLGP::AnimationState, Animation> m_animations;

	std::unordered_map<std::string, sf::IntRect> m_playerSprites;

	// Vectors
	std::vector<LLGP::Key> m_usableKeys;

	// Ints
	int m_playerID;

	// Floats
	float m_jumpForce;

	// Lists
	std::set<LLGP::Key> m_activeKeys;

	// Init functions
	void InitAnimations() override;

	// Collision check functions
	bool CheckLeftColl();
	bool CheckRightColl();
	bool CheckTopColl();
	bool CheckFeetColl();

	// Movement functions
	void Move();
	void Jump();
	void ReduceJumpForce();
	void UpdateMovementDirection();

	// Physics functions
	void AddGravity();

	// Animation functions
	void SetAnimationState();

public:

	// Constructors and Destructors
	player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos = 10.f, float yPos = 10.f, int player_id = 0);
	virtual ~player();

	// Listener functions
	void keyInputListener(LLGP::Key key);
	void OnKeyReleased(LLGP::Key key);

	// Update functions
	void UpdateInput();
	void UpdateWindowsBoundCollision();
	void Update();

	// Render functions
	void Render(sf::RenderTarget& target) override;

};

