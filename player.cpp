     #include "player.h"

// Constructor
player::player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, int player_id)
	: Character(assetRegistry, xPos, yPos),  // Initialize GameObject base class
	m_inputManager(inputManager)
{
	// Set the players id
	m_playerID = player_id;

	// Add listeners for keys
	if (m_playerID == 1)
	{
		m_objectName = "ostrich";

		// Set usable keys
		m_usableKeys = {
			LLGP::Key::W,
			LLGP::Key::A,
			LLGP::Key::D
		};

	}

	else if (m_playerID == 2)
	{
		m_objectName = "stork";

		// Set usable keys
		m_usableKeys = {
			LLGP::Key::Up,
			LLGP::Key::Left,
			LLGP::Key::Right
		};
	}

	// Set the key listeners
	for (const auto& key : m_usableKeys)
	{
		// Key press listeners
		inputManager.AddKeyPressListener(key, this, [this, key]() { keyInputListener(key); });

		// Key release listeners
		inputManager.AddKeyReleasedListener(key, this, [this, key]() { OnKeyReleased(key); });
	}

	// Get the ostrich/stork spritemap
	m_texture = assetRegistry.LoadTexture();
	m_playerSprites = assetRegistry.LoadPlayerSprites(m_playerID);

	m_sprite.setTexture(m_texture);

	// Create pointer to the animation component
	m_animationComponent = make_unique<LLGP::AnimationComponent>(m_sprite, m_objectName);

	InitAnimations();

	// Check for null pointer
	if (m_animationComponent)
	{
		// Set the animation state to idle if pointer is not null
		m_animationComponent->SetAnimationState(LLGP::idle, m_playerSprites, m_animations[LLGP::AnimationState::idle].numberOfFrames
			, m_animations[LLGP::AnimationState::idle].startingFrame
		);
	}
	else
	{
		std::cerr << "AnimationComponent is not initialized!" << std::endl;
	}	
}

// Destructor
player::~player()
{
	// Set the key listeners
	for (const auto& key : m_usableKeys)
	{
		// Remove key press listeners
		m_inputManager.RemoveKeyPressListener(key, this, [this, key]() { keyInputListener(key); });

		// Remove key released listeners
		m_inputManager.RemoveKeyReleasedListener(key, this, [this, key]() { keyInputListener(key); });
	}
}

void player::InitAnimations()
{
	m_animations[LLGP::AnimationState::idle] = Animation{ 1, 0 };
	m_animations[LLGP::AnimationState::walking] = Animation{ 4, 0 };
	m_animations[LLGP::AnimationState::flying] = Animation{ 2, 5 };
}

// Collision Checks
bool player::CheckLeftColl()
{
	if (m_sprite.getGlobalBounds().position.x <= 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool player::CheckRightColl()
{
	if (m_sprite.getGlobalBounds().position.x + m_sprite.getGlobalBounds().size.x >= SCREEN_WIDTH)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool player::CheckTopColl()
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

bool player::CheckFeetColl()
{
	// Check if feet are colliding with the ground (using feet position)
	if (m_feetPosition.y >= SCREEN_HEIGHT)
	{
		m_isGrounded = true;
		m_isJumping = false;
		m_canJump = true;
		return true;
	}
	return false;
}

void player::AddGravity()
{
	sf::Vector2f playerPos = m_sprite.getPosition();

	// Add gravity if in the air
	if (!CheckFeetColl())
	{
		// Player is in the air so set the y velocity to the gravity value
		m_direction.y += GRAVITY;
	}

	// If grounded
	else
	{
		// player is grounded, so set the y velocity to 0
		m_direction.y = 0;
	}
}

void player::SetAnimationState()
{
	if (m_isJumping)
	{
		if (m_animationComponent->GetState() != LLGP::flying)
		{
			m_animationComponent->SetAnimationState(LLGP::flying, m_playerSprites, m_animations[LLGP::AnimationState::flying].numberOfFrames
				, m_animations[LLGP::AnimationState::flying].startingFrame);
		}
	}

	else if (m_isGrounded)
	{
		if (m_isMoving)
		{
			if (m_animationComponent->GetState() != LLGP::walking)
			{
				m_animationComponent->SetAnimationState(LLGP::walking, m_playerSprites, m_animations[LLGP::AnimationState::walking].numberOfFrames
					, m_animations[LLGP::AnimationState::walking].startingFrame);
			}
		}

		else
		{
			if (m_animationComponent->GetState() != LLGP::idle)
			{
				m_animationComponent->SetAnimationState(LLGP::idle, m_playerSprites, m_animations[LLGP::AnimationState::idle].numberOfFrames
					, m_animations[LLGP::AnimationState::idle].startingFrame);
			}
		}
	}
}

void player::Move()
{
	// Apply movement to the player sprite
	m_sprite.setPosition(m_sprite.getPosition() + m_direction);
}

void player::Jump()
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

void player::ReduceJumpForce()
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

void player::UpdateMovementDirection()
{
	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{

		if (m_isFacingRight)
		{
			FlipSprite();
		}

		m_direction = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{

		if (!m_isFacingRight)
		{
			FlipSprite();
		}

		m_direction = sf::Vector2f(m_movementSpeed, 0.f);
	}

	else
	{
		m_direction = sf::Vector2f(0.f, 0.f);
	}
}

void player::keyInputListener(LLGP::Key key)
{
	m_activeKeys.insert(key);

	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{
		m_isMoving = true;
		m_direction = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{
		m_isMoving = true;
		m_direction = sf::Vector2f(m_movementSpeed, 0.f);
	}

	if (m_activeKeys.count(LLGP::Key::W) || m_activeKeys.count(LLGP::Key::Up))
	{
		Jump();
	}
}


void player::OnKeyReleased(LLGP::Key key)
{
	m_activeKeys.erase(key);

	// Check remaining keys to maintain movement
	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{
		m_direction = sf::Vector2f(-m_movementSpeed, 0.f);
	}
	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{
		m_direction = sf::Vector2f(m_movementSpeed, 0.f);
	}
	else
	{
		m_isMoving = false;

		if (!m_isJumping)
		{
			// Only stop if no keys are held
			m_direction = sf::Vector2f(0.f, 0.f);
		}
	}
}

// Update functions
void player::UpdateInput()
{
	// Keyboard input
	m_inputManager.Update();

	UpdateMovementDirection();
	
}

void player::UpdateWindowsBoundCollision()
{
	// Left
	if (this->CheckLeftColl())
	{
		m_sprite.setPosition(sf::Vector2f(0.f, m_sprite.getGlobalBounds().position.y));
	}

	// Right
	else if (this->CheckRightColl())
	{
		m_sprite.setPosition(sf::Vector2f(SCREEN_WIDTH - m_sprite.getGlobalBounds().size.x, m_sprite.getGlobalBounds().position.y));
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

void player::Update()
{
	SetAnimationState();

	// Update animation component
	m_animationComponent->Update();

	// Update window bounds collision
	UpdateWindowsBoundCollision();

	// Apply gravity to player
	AddGravity();

	// Reduce jump force if jumping
	ReduceJumpForce();

	// Apply movement
	Move();

	// Update the players feet position
	UpdateFeetPosition();

}

// Render functions
void player::Render(sf::RenderTarget& target)
{
	GameObject::Render(target);
}

