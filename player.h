#pragma once

#include <SFML/Graphics.hpp>

class player
{
private:

	// Shapes
	sf::RectangleShape shape;
	sf::Texture texture;
	sf::IntRect ostrich1Rect;  // x, y, width, height
	sf::Sprite ostrichSprite;

	// Floats
	float movementSpeed;

	// Init functions
	void initVariables();
	void initShape();

	// Collision checks
	bool checkLeftColl();
	bool checkRightColl(sf::VideoMode screen_bounds);
	bool checkTopColl();
	bool checkBottomColl(sf::VideoMode screen_bounds);

public:

	// Constructors and Destructors
	player(float x = 0.f, float y = 0.f);
	virtual ~player();

	// Update functions
	void updateInput(float deltaTime);
	void updateWindowsBoundCollision(sf::VideoMode screen_bounds);
	void update(sf::VideoMode screen_bounds, float deltaTime);

	// Render functions
	void render(sf::RenderTarget& target);

};

