#include "Character.h"

Character::Character(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName) : 
	GameObject(assetRegistry, xPos, yPos, objectName), m_riderSprite(m_texture)
{
	// Set the rider sprite
	m_riderSprite.setTextureRect(m_assetRegistry.LoadSprite("rider", m_riderIndex));
	SetRiderPosition();

	UpdateFeetPosition();
	InitVariables();
	
	// Create pointer to the animation component and initialise it
	CreateAnimationComponent(m_sprite, m_objectName);

}

Character::~Character()
{
}

void Character::InitVariables()
{
	m_movementSpeed = PLAYER_MAX_HORIZONTAL_MOVEMENTSPEED;
}

void Character::UpdateFeetPosition()
{
	// Update the feet position (the bottom center of the player)
	m_feetPosition = sf::Vector2f(m_sprite.getPosition().x + m_sprite.getGlobalBounds().size.x / 2.f,
		m_sprite.getPosition().y + m_sprite.getGlobalBounds().size.y);
}

void Character::FlipSprite()
{
	sf::FloatRect bounds = m_sprite.getLocalBounds();

	// Flip sprite left
	if (m_isFacingRight)
	{
		// Set the origin to the sprite's center
		m_sprite.setOrigin(sf::Vector2f(bounds.size.x, 0.f));
		m_riderSprite.setOrigin(sf::Vector2f(m_riderSprite.getLocalBounds().size.x, 0.f));

		// Set the scale to flip
		m_sprite.setScale(sf::Vector2f(-1.f, 1.f));
		m_riderSprite.setScale(sf::Vector2f(-1.f, 1.f));
	}

	// Flip sprite right
	else
	{
		// Set the origin to the sprite's center
		m_sprite.setOrigin(sf::Vector2f(0.f, 0.f));
		m_riderSprite.setOrigin(sf::Vector2f(0.f, 0.f));

		// Set the scale to flip
		m_sprite.setScale(sf::Vector2f(1.f, 1.f));
		m_riderSprite.setScale(sf::Vector2f(1.f, 1.f));
	}

	m_isFacingRight = !m_isFacingRight;
}

void Character::SetAnimationState()
{
	// Calculate the velocity magnitude (speed)
	float speed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
	float adjustedFrameTime = std::max(0.1f, 1.f / (speed + 1.f));

	if (m_isJumping || m_isFalling)
	{
		adjustedFrameTime = adjustedFrameTime * 0.1f;

		if (m_animationComponent->GetState() != LLGP::flying)
		{
			m_animationComponent->SetAnimationState(LLGP::flying, m_textureMap, m_animations[LLGP::AnimationState::flying].numberOfFrames
				, m_animations[LLGP::AnimationState::flying].startingFrame, adjustedFrameTime);
		}
	}

	else if (m_isGrounded)
	{
		if (m_isMoving)
		{
			adjustedFrameTime = adjustedFrameTime * 0.02f;

			if (m_animationComponent->GetState() != LLGP::walking)
			{
				m_animationComponent->SetAnimationState(LLGP::walking, m_textureMap, m_animations[LLGP::AnimationState::walking].numberOfFrames
					, m_animations[LLGP::AnimationState::walking].startingFrame, adjustedFrameTime);
			}
		}

		else
		{
			if (m_animationComponent->GetState() != LLGP::idle)
			{
				m_animationComponent->SetAnimationState(LLGP::idle, m_textureMap, m_animations[LLGP::AnimationState::idle].numberOfFrames
					, m_animations[LLGP::AnimationState::idle].startingFrame, adjustedFrameTime);
			}
		}
	}


}

void Character::StopSpawning()
{
	m_isSpawning = false;
	m_canFall = true;
	m_canJump = true;
	m_canCollide = true;

	// Set the colour back to white
	m_sprite.setColor(sf::Color::White);
}

bool Character::CheckLeftColl()
{
	if (m_sprite.getGlobalBounds().position.x + m_sprite.getGlobalBounds().size.x <= 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Character::CheckRightColl()
{
	if (m_sprite.getGlobalBounds().position.x >= SCREEN_WIDTH)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Character::CheckTopColl()
{
	if (m_sprite.getGlobalBounds().position.y <= 0.f)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Character::CheckFeetColl()
{
	// Check if feet are colliding with the ground (using feet position)
	if (m_feetPosition.y >= SCREEN_HEIGHT)
	{
		m_isGrounded = true;
		m_isJumping = false;
		m_canJump = true;
		m_isFalling = false;
		return true;
	}
	return false;
}

void Character::Move()
{
	if (!m_isSpawning)
	{
		// Apply movement to the characters sprite
		m_sprite.setPosition(m_sprite.getPosition() + m_velocity + m_dynamicForce);
	}
}

void Character::Jump()
{
	if (!m_isJumping && m_canJump)
	{
		m_isJumping = true;
		m_jumpForce = INITIAL_JUMP_FORCE;
		m_canJump = false;
		m_isGrounded = false;
	}

	ReduceJumpForce();
}

void Character::ReduceJumpForce()
{
	if (m_isJumping)
	{
		// Adjust position
		float yPos = m_sprite.getPosition().y;
		float newYPos = yPos -= m_jumpForce;
		m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, newYPos));

		// Reduce jump force
		m_jumpForce -= JUMP_FORCE_DECREMENT;

		UpdateFeetPosition();

		// Stop jump when force has reached 0
		if (m_jumpForce <= 0.0f)
		{
			StopJumpingMovement();
		}
	}
}

void Character::UpdateMovementDirection()
{

}

void Character::MoveToSpawnPosition()
{
	debugName;

	m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, m_sprite.getPosition().y - 1.1f));
	float yPos = m_sprite.getGlobalBounds().position.y + m_sprite.getGlobalBounds().size.y - 25.f;

	if (yPos < m_spawnPosition.y)
	{
		StopSpawning();
	}
}

void Character::SetRiderPosition()
{
	// Get the sprite’s actual visual origin (in world space)
	sf::Vector2f charPos = m_sprite.getPosition();
	sf::Vector2f origin = m_sprite.getOrigin();
	sf::Vector2f scale = m_sprite.getScale();

	float xOffset = m_riderXOffset;

	if (!m_isFacingRight)
	{
		xOffset = m_riderXOffset / 2;
	}

	// Flip-aware offset (only flip horizontally)
	sf::Vector2f offset(xOffset, m_riderYOffset);

	offset.x *= scale.x; // Flip horizontally if scale.x == -1

	// Position rider relative to the character's true visual anchor
	sf::Vector2f anchoredPos = charPos + offset;

	m_riderSprite.setPosition(anchoredPos);
}


void Character::AddGravity()
{
	if (m_canFall)
	{
		// Add gravity if in the air
		if (!CheckFeetColl())
		{
			// Player is in the air so set the y velocity to the gravity value
			m_velocity.y += GRAVITY;
			m_isFalling = true;
		}

		// If grounded
		else
		{
			// player is grounded, so set the y velocity to 0
			m_velocity.y = 0;
		}
	}
}

void Character::UpdateWindowsBoundCollision()
{
	// Left
	if (this->CheckLeftColl())
	{
		m_sprite.setPosition(sf::Vector2f(SCREEN_WIDTH, m_sprite.getGlobalBounds().position.y));
	}

	// Right
	else if (this->CheckRightColl())
	{
		m_sprite.setPosition(sf::Vector2f(0.f, m_sprite.getGlobalBounds().position.y));
	}


	// Top
	if (this->CheckTopColl())
	{
		m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, 0.f));
	}

	// Bottom

	else if (this->CheckFeetColl())
	{
		m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, SCREEN_HEIGHT - m_sprite.getGlobalBounds().size.y));
	}
}

void Character::CreateAnimationComponent(sf::Sprite& sprite, const std::string& name)
{
	// Create pointer to the animation component
	m_animationComponent = make_unique<LLGP::AnimationComponent>(m_sprite, m_objectName);
}

void Character::Spawn()
{
	m_isSpawning = true;
	m_canFall = false;
	m_canJump = false;
	m_canCollide = false;

	m_sprite.setColor(sf::Color::Cyan);

	float xPos = m_sprite.getPosition().x;
	float yPos = m_sprite.getPosition().y - m_sprite.getGlobalBounds().size.x;
	m_spawnPosition = sf::Vector2f(xPos, yPos);

}

void Character::SetPosition(float xPos, float yPos)
{
	m_sprite.setPosition(sf::Vector2f(xPos, yPos));
}

void Character::StopHorizontalMovement()
{
	m_velocity.x = 0.f;
	m_isJumping = false;
	m_canJump = true;
}

void Character::StopJumpingMovement()
{
	m_isJumping = false;
	m_canJump = true;
	m_isFalling = true;
}

void Character::StopFalling()
{
	m_velocity.y = 0.f;
	m_isGrounded = true;
	m_isJumping = false;
	m_canJump = true;
	m_isFalling = false;
}

void Character::Despawn()
{
	m_isAlive = false;
}

bool Character::GetIsSpawning()
{
	return m_isSpawning;
}

bool Character::GetIsFalling()
{
	return m_isFalling;
}

sf::Vector2f Character::GetPosition()
{
	return m_sprite.getGlobalBounds().position;
}

sf::Vector2f Character::GetVelocity() const
{
	return m_velocity;
}

void Character::SetVelocity(const sf::Vector2f newVelocity)
{
	m_velocity = newVelocity;
}

void Character::UpdateCollisionCooldown(float deltaTime)
{
	if (m_collisionCooldown > 0.f)
		m_collisionCooldown -= deltaTime;
}

void Character::UpdateForceDecrement()
{
	if (m_dynamicForce != sf::Vector2f(0.f, 0.f))
	{
		// Decrement the x force value
		if (m_dynamicForce.x > 0)
			m_dynamicForce.x -= COLLISION_FORCE_DECREMENT;

		else if (m_dynamicForce.x < 0)
			m_dynamicForce.x += COLLISION_FORCE_DECREMENT;

		// Decrement the y force value
		if (m_dynamicForce.y > 0)
			m_dynamicForce.y -= COLLISION_FORCE_DECREMENT;

		else if (m_dynamicForce.y < 0)
			m_dynamicForce.y += COLLISION_FORCE_DECREMENT;

		// Clamp very small values to zero to prevent lingering drift
		if (std::abs(m_dynamicForce.x) < COLLISION_FORCE_DECREMENT)
			m_dynamicForce.x = 0.f;
		if (std::abs(m_dynamicForce.y) < COLLISION_FORCE_DECREMENT)
			m_dynamicForce.y = 0.f;
	}
}

void Character::Render(sf::RenderTarget& target)
{
	// Draw the rider
	if (m_hasRider)
		target.draw(m_riderSprite);

	GameObject::Render(target);
}

bool Character::GetCanCollide() const
{
	if (m_canCollide && m_collisionCooldown <= 0.f)
		return true;

	else
		return false;
}

bool Character::GetHasRider() const
{
	return m_hasRider;
}

bool Character::GetIsAlive() const
{
	return m_isAlive;
}

int Character::GetPointsValue() const
{
	return m_pointValue;
}

bool Character::GetIsFacingRight() const
{
	return m_isFacingRight;
}

void Character::ResetCollisionCooldown()
{
	m_collisionCooldown = m_maxCollisionCooldown;
}

void Character::AddNewForce(sf::Vector2f forceToAdd)
{
	m_dynamicForce += forceToAdd;

	// Clamp X and Y forces
	m_dynamicForce.x = std::clamp(m_dynamicForce.x, -COLLISION_MAX_FORCE, COLLISION_MAX_FORCE);
	m_dynamicForce.y = std::clamp(m_dynamicForce.y, -COLLISION_MAX_FORCE, COLLISION_MAX_FORCE);
}

void Character::Update(float deltaTime)
{
	debugName;

	if (m_isSpawning) // If the player is spawning
	{
		MoveToSpawnPosition();
	}

	// Set the animation state for this frame
	SetAnimationState();

	// Update animation component
	m_animationComponent->Update(deltaTime);

	// Update window bounds collision
	UpdateWindowsBoundCollision();

	// Apply gravity to character
	AddGravity();

	// Reduce jump force if jumping
	ReduceJumpForce();

	// Apply movement
	Move();

	// Reduce forces
	UpdateForceDecrement();

	// Update rider sprites position
	SetRiderPosition();

	// Update the players feet position
	UpdateFeetPosition();

	// Update the cooldown of collisions
	UpdateCollisionCooldown(deltaTime);
	
}
