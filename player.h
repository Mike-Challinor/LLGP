#pragma once

#include <SFML/Graphics.hpp>
#include "Commons.h"
#include "Constants.h"

class player
{
private:

	// Shapes
	sf::Texture texture;
	sf::IntRect ostrich1Rect;  // x, y, width, height
	sf::Sprite ostrichSprite;

	// Floats
	float movementSpeed;

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
	player(float x = 10.f, float y = 10.f);
	virtual ~player();

	// Update functions
	void updateInput();
	void updateWindowsBoundCollision(sf::VideoMode screen_bounds);
	void update(sf::VideoMode screen_bounds, float deltaTime);

	// Render functions
	void render(sf::RenderTarget& target);

};

