#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <set>

#include "Commons.h"
#include "Constants.h"
#include "InputManager.h"
#include "AssetRegistry.h"
#include "AnimationComponent.h"

struct Animation
{
	int numberOfFrames;
	int startingFrame;
};

class player
{
private:
	
	// Managers
	LLGP::InputManager& m_inputManager; // Reference to an existing InputManager
	LLGP::AssetRegistry& m_assetRegistry; // Reference to an existing AssetRegistry

	// Animations
	std::unordered_map<LLGP::AnimationState, Animation> m_animations;

	// Shapes
	sf::Texture m_texture;
	sf::Sprite m_mountSprite;

	std::unordered_map<std::string, sf::IntRect> m_playerSprites;

	// Vectors
	sf::Vector2f m_direction;

	//Ints
	int m_playerID;

	// Floats
	float m_movementSpeed;
	float m_jumpForce;

	// Bools
	bool m_isJumping = false;
	bool m_canJump = true;
	bool isFacingRight = true;
	bool m_isMoving = true;
	bool m_isGrounded = false;

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
	bool checkLeftColl();
	bool checkRightColl();
	bool checkTopColl();
	bool checkBottomColl();

	// Sprite Functions
	void FlipSprite();

	// Movement functions
	void Move();
	void Jump();
	void ReduceJumpForce();
	void UpdateMovementDirection();

	// Physics functions
	void AddGravity();

public:

	// Constructors and Destructors
	player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float x = 10.f, float y = 10.f, int player_id = 0);
	virtual ~player();

	sf::IntRect GetSpriteRectByName(const std::string& name) const;

	// Listener functions
	void keyInputListener(LLGP::Key key);
	void OnKeyReleased(LLGP::Key key);

	// Update functions
	void updateInput();
	void updateWindowsBoundCollision();
	void update();

	// Render functions
	void render(sf::RenderTarget& target);

};

