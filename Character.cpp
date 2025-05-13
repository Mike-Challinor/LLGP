#include "Character.h"

// --- Constructor ---
Character::Character(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName) : 
	GameObject(assetRegistry, xPos, yPos, objectName), m_riderSprite(m_texture)
{
	// Set the rider sprite
	m_riderSprite.setTextureRect(m_assetRegistry.LoadSprite("rider", m_riderIndex));
	SetRiderPosition();

	// Set the initial foot position of the character
	UpdateFeetPosition();

	// Initialise the characters variables
	InitVariables();
	
	// Create pointer to the animation component and initialise it
	CreateAnimationComponent(m_sprite, m_objectName);

}

// --- Destructor ---
Character::~Character()
{
}

// --- Function for initialising the characters variables ---
void Character::InitVariables()
{
	m_movementSpeed = PLAYER_MAX_HORIZONTAL_MOVEMENTSPEED;
}

// --- Function for updating the foot position of the character ---
void Character::UpdateFeetPosition()
{
	// Update the feet position (the bottom center of the player)
	m_feetPosition = sf::Vector2f(m_sprite.getPosition().x + m_sprite.getGlobalBounds().size.x / 2.f,
		m_sprite.getPosition().y + m_sprite.getGlobalBounds().size.y);
}

// --- Function for flipping the characters sprite ---
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

// --- Function for setting the animation state of the character ---
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

// --- Function for ending spawning ---
void Character::StopSpawning()
{
	// Set booleans
	m_isSpawning = false;
	m_canFall = true;
	m_canJump = true;
	m_canCollide = true;

	// Set the colour back to white
	m_sprite.setColor(sf::Color::White);
}

// --- Function for checking collision with the left of the screen ---
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

// --- Function for checking collision with the right of the screen ---
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

// --- Function for checking collision with the top of the screen ---
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

// --- Function for checking collision with the bottom of the screen ---
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

// --- Function for moving the character ---
void Character::Move()
{
	if (!m_isSpawning)
	{
		// Apply movement to the characters sprite
		m_sprite.setPosition(m_sprite.getPosition() + m_velocity + m_dynamicForce);
	}
}

// --- Function for jumping ---
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

// --- Function for reducing the jumps force ---
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

// --- Empty function declaration for updating movement direction ---
void Character::UpdateMovementDirection()
{
	// This could maybe be moved into the player character class
}

// --- Function for moving the character to the spawn position ---
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

// --- Function for setting the rider sprite position relevant to the base sprites position ---
void Character::SetRiderPosition()
{
	// Get the sprite’s actual visual origin (in world space)
	sf::Vector2f charPos = m_sprite.getPosition();
	sf::Vector2f origin = m_sprite.getOrigin();
	sf::Vector2f scale = m_sprite.getScale();

	// Set the x offset of the rider
	float xOffset = m_riderXOffset;

	if (!m_isFacingRight)
	{
		xOffset = m_riderXOffset / 2;
	}

	// Flip-aware offset (only flip horizontally)
	sf::Vector2f offset(xOffset, m_riderYOffset);

	// Flip horizontally if scale.x == -1
	offset.x *= scale.x; 

	// Position rider relative to the character's true visual anchor
	sf::Vector2f anchoredPos = charPos + offset;

	// Set the riders position to the anchored position
	m_riderSprite.setPosition(anchoredPos);
}

// --- Function for applying gravity to the character ---
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
			// Player is grounded, so set the y velocity to 0
			m_velocity.y = 0;
		}
	}
}

// --- Function for updating the windows bound collision ---
void Character::UpdateWindowsBoundCollision()
{
	// Left bound of the screen
	if (this->CheckLeftColl())
	{
		m_sprite.setPosition(sf::Vector2f(SCREEN_WIDTH, m_sprite.getGlobalBounds().position.y));
	}

	// Right bound of the screen
	else if (this->CheckRightColl())
	{
		m_sprite.setPosition(sf::Vector2f(0.f, m_sprite.getGlobalBounds().position.y));
	}


	// Top bound of the screen
	if (this->CheckTopColl())
	{
		m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, 0.f));
	}

	// Bottom bound of the screen

	else if (this->CheckFeetColl())
	{
		m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, SCREEN_HEIGHT - m_sprite.getGlobalBounds().size.y));
	}
}

// --- Function for creating the animation component ---
void Character::CreateAnimationComponent(sf::Sprite& sprite, const std::string& name)
{
	// Create pointer to the animation component
	m_animationComponent = make_unique<LLGP::AnimationComponent>(m_sprite, m_objectName);
}

// --- Function for spawning the character ---
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

// --- Function for setting the base sprites position ---
void Character::SetPosition(float xPos, float yPos)
{
	m_sprite.setPosition(sf::Vector2f(xPos, yPos));
}

// --- Function for stopping horizontal movement ---
void Character::StopHorizontalMovement()
{
	m_velocity.x = 0.f;
	m_isJumping = false;
	m_canJump = true;
}

// --- Function for stopping jumping movement ---
void Character::StopJumpingMovement()
{
	m_isJumping = false;
	m_canJump = true;
	m_isFalling = true;
}

// --- Function for stopping falling movement ---
void Character::StopFalling()
{
	m_velocity.y = 0.f;
	m_isGrounded = true;
	m_isJumping = false;
	m_canJump = true;
	m_isFalling = false;
}

// --- Function for despawning ---
void Character::Despawn()
{
	m_isAlive = false;
}

// --- Function that returns whether the character is spawning ---
bool Character::GetIsSpawning()
{
	return m_isSpawning;
}

// --- Function that returns whether the character is falling ---
bool Character::GetIsFalling()
{
	return m_isFalling;
}

// --- Function that returns the base sprites global position ---
sf::Vector2f Character::GetPosition()
{
	return m_sprite.getGlobalBounds().position;
}

// --- Function that returns the characters velocity ---
sf::Vector2f Character::GetVelocity() const
{
	return m_velocity;
}

// --- Function that returns whether the character can collide ---
bool Character::GetCanCollide() const
{
	if (m_canCollide && m_collisionCooldown <= 0.f)
		return true;

	else
		return false;
}

// --- Function that returns whether the character has a rider ---
bool Character::GetHasRider() const
{
	return m_hasRider;
}

// --- Function that returns whether the character is alive ---
bool Character::GetIsAlive() const
{
	return m_isAlive;
}

// --- Function that returns the characters points value ---
int Character::GetPointsValue() const
{
	return m_pointValue;
}

// --- Function that returns the characters facing direction ---
bool Character::GetIsFacingRight() const
{
	return m_isFacingRight;
}

// --- Function that sets the characters velocity ---
void Character::SetVelocity(const sf::Vector2f newVelocity)
{
	m_velocity = newVelocity;
}

// --- Function that updates the collision cooldown ---
void Character::UpdateCollisionCooldown(float deltaTime)
{
	if (m_collisionCooldown > 0.f)
		m_collisionCooldown -= deltaTime;
}

// --- Function that decrements any dynamic forces applied to the character ---
void Character::UpdateForceDecrement()
{
	if (m_dynamicForce != sf::Vector2f(0.f, 0.f))
	{
		// Decrement the x force value
		if (m_dynamicForce.x > 0)
			m_dynamicForce.x -= COLLISION_FORCE_DECREMENT;
		// Increment the x force value
		else if (m_dynamicForce.x < 0)
			m_dynamicForce.x += COLLISION_FORCE_DECREMENT;

		// Decrement the y force value
		if (m_dynamicForce.y > 0)
			m_dynamicForce.y -= COLLISION_FORCE_DECREMENT;
		// Increment the y force value
		else if (m_dynamicForce.y < 0)
			m_dynamicForce.y += COLLISION_FORCE_DECREMENT;

		// Clamp very small values to zero to prevent lingering drift
		if (std::abs(m_dynamicForce.x) < COLLISION_FORCE_DECREMENT)
			m_dynamicForce.x = 0.f;
		if (std::abs(m_dynamicForce.y) < COLLISION_FORCE_DECREMENT)
			m_dynamicForce.y = 0.f;
	}
}

// --- Render function ---
void Character::Render(sf::RenderTarget& target)
{
	// Draw the rider
	if (m_hasRider)
		target.draw(m_riderSprite);

	GameObject::Render(target);
}

// --- Function that resets the collision cooldown ---
void Character::ResetCollisionCooldown()
{
	m_collisionCooldown = m_maxCollisionCooldown;
}

// --- Function that applies a new force to the character ---
void Character::AddNewForce(sf::Vector2f forceToAdd)
{
	// Increment the current force applied with the new force to add
	m_dynamicForce += forceToAdd;

	// Clamp X and Y forces
	m_dynamicForce.x = std::clamp(m_dynamicForce.x, -COLLISION_MAX_FORCE, COLLISION_MAX_FORCE);
	m_dynamicForce.y = std::clamp(m_dynamicForce.y, -COLLISION_MAX_FORCE, COLLISION_MAX_FORCE);
}

// --- Main update function ---
void Character::Update(float deltaTime)
{
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
