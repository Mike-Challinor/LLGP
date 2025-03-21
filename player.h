#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include "Commons.h"
#include "Constants.h"
#include "InputManager.h"

class player
{
private:
	
	// Managers
	LLGP::InputManager& m_inputManager; // Reference to an existing InputManager

	// Shapes
	sf::Texture texture;
	sf::IntRect ostrich1Rect;  // x, y, width, height
	sf::Sprite ostrichSprite;

	//Ints
	int m_playerID;

	// Floats
	float m_movementSpeed;
	float m_jumpForce;

	// Bools
	bool m_isJumping = false;
	bool m_canJump = true;

	// Init functions
	void initVariables();

	// Collision checks
	bool checkLeftColl();
	bool checkRightColl(sf::VideoMode screen_bounds);
	bool checkTopColl();
	bool checkBottomColl(sf::VideoMode screen_bounds);

	void AddGravity();

public:

	// Constructors and Destructors
	player(LLGP::InputManager& inputManager, float x = 10.f, float y = 10.f, int player_id = 0);
	virtual ~player();

	// Movement functions
	void Jump();

	// Listener functions
	void keyInputListener(LLGP::Key key);

	// Update functions
	void updateInput();
	void updateWindowsBoundCollision(sf::VideoMode screen_bounds);
	void update(sf::VideoMode screen_bounds, float deltaTime);

	// Render functions
	void render(sf::RenderTarget& target);

};

