#include "player.h"

// Constructor
player::player(float x, float y)
{
	this->shape.setPosition(sf::Vector2f(x, y));
	this->initVariables();
	this->initShape();
}

// Destructor
player::~player()
{

}

// Init functions
void player::initVariables()
{
	this->movementSpeed = 5.f;
}

void player::initShape()
{
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setSize(sf::Vector2f(50.f, 50.f));
}

// Collision Checks
bool player::checkLeftColl()
{
	if (this->shape.getGlobalBounds().position.x <= 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool player::checkRightColl(sf::VideoMode screen_bounds)
{
	if (this->shape.getGlobalBounds().position.x + this->shape.getGlobalBounds().size.x >= screen_bounds.size.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool player::checkTopColl()
{
	if (this->shape.getGlobalBounds().position.y <= 0.f)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool player::checkBottomColl(sf::VideoMode screen_bounds)
{
	if (this->shape.getGlobalBounds().position.y + this->shape.getGlobalBounds().size.y >= screen_bounds.size.y)
	{
		return true;
	}

	else
	{
		return false;
	}
}


// Update functions
void player::updateInput()
{
	//Keyboard input

	// Horizontal movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
	{
		// Move Left
		shape.move(sf::Vector2f(-movementSpeed, 0.f));
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
	{
		// Move Right
		shape.move(sf::Vector2f(movementSpeed, 0.f));
	}


	// Vertical movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
	{
		// Move Up
		shape.move(sf::Vector2f(0.f, -movementSpeed));
	}
	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
	{
		// Move Down
		shape.move(sf::Vector2f(0.f, movementSpeed));
	}
}

void player::updateWindowsBoundCollision(sf::VideoMode screen_bounds)
{
	//Left
	if (this->checkLeftColl())
	{
		this->shape.setPosition(sf::Vector2f(0.f, this->shape.getGlobalBounds().position.y));
	}

	//Right
	else if (this->checkRightColl(screen_bounds))
	{
		this->shape.setPosition(sf::Vector2f(screen_bounds.size.x - this->shape.getGlobalBounds().size.x, this->shape.getGlobalBounds().position.y));
	}

	//Top
	if (this->checkTopColl())
	{
		this->shape.setPosition(sf::Vector2f(this->shape.getGlobalBounds().position.x, 0.f));
	}

	//Bottom

	else if (this->checkBottomColl(screen_bounds))
	{
		this->shape.setPosition(sf::Vector2f(this->shape.getGlobalBounds().position.x, screen_bounds.size.y - this->shape.getGlobalBounds().size.y));
	}
}

void player::update(sf::VideoMode screen_bounds)
{
	// Update keyboard/movement input
	this->updateInput();

	// Update window bounds collision
	this->updateWindowsBoundCollision(screen_bounds);
}


// Render functions
void player::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}

