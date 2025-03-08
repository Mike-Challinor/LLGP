#include "player.h"

// Constructor
player::player(float x, float y) : ostrichSprite(texture)
{
	// Set players position
	ostrichSprite.setPosition(sf::Vector2f(x, y));

	// Set player texture
	if (!this->texture.loadFromFile("Resources/Sprites/joustsprites.jpg"))
	{
		throw std::runtime_error("Failed to load texture: joustsprites.jpg");
	}

	else
	{
		// Set players texture
		ostrichSprite.setTexture(texture);
		ostrichSprite.setTextureRect(sf::IntRect({ 384, 62 }, { 40 , 50 }));
	}

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
	this->movementSpeed = 150.f;
}

void player::initShape()
{
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setSize(sf::Vector2f(50.f, 50.f));
}

// Collision Checks
bool player::checkLeftColl()
{
	if (ostrichSprite.getGlobalBounds().position.x <= 0.f)
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
	if (this->shape.getGlobalBounds().position.x + ostrichSprite.getGlobalBounds().size.x >= screen_bounds.size.x)
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
	if (ostrichSprite.getGlobalBounds().position.y <= 0.f)
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
	if (ostrichSprite.getGlobalBounds().position.y + ostrichSprite.getGlobalBounds().size.y >= screen_bounds.size.y)
	{
		return true;
	}

	else
	{
		return false;
	}
}


// Update functions
void player::updateInput(float deltaTime)
{
	//Keyboard input

	// Horizontal movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
	{
		// Move left
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(-movementSpeed * (deltaTime / 1000), 0.f));
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
	{
		// Move right
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(movementSpeed * (deltaTime / 1000), 0.f));
	}


	// Vertical movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
	{
		// Move up
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(0.f, -movementSpeed * (deltaTime / 1000)));
	}
	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
	{
		// Move down
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(0.f, movementSpeed * (deltaTime / 1000)));
	}
}

void player::updateWindowsBoundCollision(sf::VideoMode screen_bounds)
{
	// Left
	if (this->checkLeftColl())
	{
		ostrichSprite.setPosition(sf::Vector2f(0.f, ostrichSprite.getGlobalBounds().position.y));
	}

	// Right
	else if (this->checkRightColl(screen_bounds))
	{
		ostrichSprite.setPosition(sf::Vector2f(screen_bounds.size.x - ostrichSprite.getGlobalBounds().size.x, ostrichSprite.getGlobalBounds().position.y));
	}

	// Top
	if (this->checkTopColl())
	{
		ostrichSprite.setPosition(sf::Vector2f(ostrichSprite.getGlobalBounds().position.x, 0.f));
	}

	// Bottom

	else if (this->checkBottomColl(screen_bounds))
	{
		ostrichSprite.setPosition(sf::Vector2f(ostrichSprite.getGlobalBounds().position.x, screen_bounds.size.y - ostrichSprite.getGlobalBounds().size.y));
	}
}

void player::update(sf::VideoMode screen_bounds, float deltaTime)
{
	// Update keyboard/movement input
	this->updateInput(deltaTime);

	// Update window bounds collision
	this->updateWindowsBoundCollision(screen_bounds);
}


// Render functions
void player::render(sf::RenderTarget& target)
{
	target.draw(this->ostrichSprite);
}

