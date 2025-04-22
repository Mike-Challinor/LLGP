#include "Character.h"

Character::Character(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos) : GameObject(assetRegistry, xPos, yPos)
{
	UpdateFeetPosition();
	InitVariables();
}

Character::~Character()
{
}

void Character::InitVariables()
{
	m_movementSpeed = MOVEMENTSPEED;
}

void Character::UpdateFeetPosition()
{
	// Update the feet position (the bottom center of the player)
	m_feetPosition = sf::Vector2f(m_sprite.getPosition().x + m_sprite.getGlobalBounds().size.x / 2.f,
		m_sprite.getPosition().y + m_sprite.getGlobalBounds().size.y);
}

void Character::InitAnimations()
{
	// Add animations in here
}

void Character::FlipSprite()
{
	// Flip sprite left
	if (m_isFacingRight)
	{
		// Set the origin to the sprite's center (or appropriate pivot point)
		m_sprite.setOrigin(sf::Vector2f(m_sprite.getLocalBounds().size.x, 0.f));
		// Set the scale to flip
		m_sprite.setScale(sf::Vector2f(-1.f, 1.f));
	}

	// Flip sprite right
	else
	{
		// Set the origin to the sprite's center (or appropriate pivot point)
		m_sprite.setOrigin(sf::Vector2f(0.f, 0.f));

		// Set the scale to flip
		m_sprite.setScale(sf::Vector2f(1.f, 1.f));
	}

	m_isFacingRight = !m_isFacingRight;
}

void Character::SetPosition(float xPos, float yPos)
{
	m_sprite.setPosition(sf::Vector2f(xPos, yPos));
}

void Character::StopHorizontalMovement()
{
	m_direction.x = 0.f;
	m_isJumping = false;
	m_canJump = true;
}

void Character::StopJumpingMovement()
{
	m_isJumping = false;
	m_canJump = true;
}

void Character::StopFalling()
{
	m_direction.y = 0.f;
	m_isGrounded = true;
	m_isJumping = false;
	m_canJump = true;
}
