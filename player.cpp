     #include "player.h"

// Constructor
player::player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, int player_id)
	: GameObject(assetRegistry, xPos, yPos),  // Initialize GameObject base class
	m_inputManager(inputManager)
{
	// Set the players id
	m_playerID = player_id;

	// Initialize feet position to be at the bottom center of the player
	UpdateFeetPosition();

	// Add listeners for keys
	if (m_playerID == 1)
	{
		m_mountName = "ostrich";

		// Key press listeners
		inputManager.AddKeyPressListener(LLGP::Key::W, this, [this]() { keyInputListener(LLGP::Key::W); });
		inputManager.AddKeyPressListener(LLGP::Key::A, this, [this]() { keyInputListener(LLGP::Key::A); });
		inputManager.AddKeyPressListener(LLGP::Key::D, this, [this]() { keyInputListener(LLGP::Key::D); });

		// Key release listeners
		inputManager.AddKeyReleasedListener(LLGP::Key::W, this, [this]() { OnKeyReleased(LLGP::Key::W); });
		inputManager.AddKeyReleasedListener(LLGP::Key::A, this, [this]() { OnKeyReleased(LLGP::Key::A); });
		inputManager.AddKeyReleasedListener(LLGP::Key::D, this, [this]() { OnKeyReleased(LLGP::Key::D); });

	}

	else if (m_playerID == 2)
	{
		m_mountName = "stork";

		// Key press listeners
		inputManager.AddKeyPressListener(LLGP::Key::Up, this, [this]() { keyInputListener(LLGP::Key::Up); });
		inputManager.AddKeyPressListener(LLGP::Key::Left, this, [this]() { keyInputListener(LLGP::Key::Left); });
		inputManager.AddKeyPressListener(LLGP::Key::Right, this, [this]() { keyInputListener(LLGP::Key::Right); });

		// Key release listeners
		inputManager.AddKeyReleasedListener(LLGP::Key::Up, this, [this]() { OnKeyReleased(LLGP::Key::Up); });
		inputManager.AddKeyReleasedListener(LLGP::Key::Left, this, [this]() { OnKeyReleased(LLGP::Key::Left); });
		inputManager.AddKeyReleasedListener(LLGP::Key::Right, this, [this]() { OnKeyReleased(LLGP::Key::Right); });
	}

	// Get the ostrich/stork spritemap
	m_texture = assetRegistry.LoadTexture();
	m_playerSprites = assetRegistry.LoadPlayerSprites(m_playerID);

	InitAnimations();

	m_sprite.setTexture(m_texture);

	// Create pointer to the animation component
	m_animationComponent = make_unique<LLGP::AnimationComponent>(m_sprite, m_mountName);

	// Check for null pointer
	if (m_animationComponent)
	{
		// Set the animation state if pointer is not null
		m_animationComponent->SetAnimationState(LLGP::idle, m_playerSprites, m_animations[LLGP::AnimationState::idle].numberOfFrames
			, m_animations[LLGP::AnimationState::idle].startingFrame
		);
	}
	else
	{
		std::cerr << "AnimationComponent is not initialized!" << std::endl;
	}

	// Create collision shapes
	auto bodyCollision = std::make_unique<CollisionComponent>();
	bodyCollision->AddCollisionShape("body", m_sprite.getGlobalBounds(), CollisionType::Solid);
	SetCollisionComponent(std::move(bodyCollision));

	this->InitVariables();
	
}

// Destructor
player::~player()
{
	// Remove key press listeners
	m_inputManager.RemoveKeyPressListener(LLGP::Key::W, this, [this]() { keyInputListener(LLGP::Key::W); });
	m_inputManager.RemoveKeyPressListener(LLGP::Key::A, this, [this]() { keyInputListener(LLGP::Key::A); });
	m_inputManager.RemoveKeyPressListener(LLGP::Key::D, this, [this]() { keyInputListener(LLGP::Key::D); });
	m_inputManager.RemoveKeyPressListener(LLGP::Key::Up, this, [this]() { keyInputListener(LLGP::Key::Up); });
	m_inputManager.RemoveKeyPressListener(LLGP::Key::Left, this, [this]() { keyInputListener(LLGP::Key::Left); });
	m_inputManager.RemoveKeyPressListener(LLGP::Key::Right, this, [this]() { keyInputListener(LLGP::Key::Right); });

	// Remove key released listeners
	m_inputManager.RemoveKeyReleasedListener(LLGP::Key::W, this, [this]() { OnKeyReleased(LLGP::Key::W); });
	m_inputManager.RemoveKeyReleasedListener(LLGP::Key::A, this, [this]() { OnKeyReleased(LLGP::Key::A); });
	m_inputManager.RemoveKeyReleasedListener(LLGP::Key::D, this, [this]() { OnKeyReleased(LLGP::Key::D); });
	m_inputManager.RemoveKeyReleasedListener(LLGP::Key::Up, this, [this]() { OnKeyReleased(LLGP::Key::Up); });
	m_inputManager.RemoveKeyReleasedListener(LLGP::Key::Left, this, [this]() { OnKeyReleased(LLGP::Key::Left); });
	m_inputManager.RemoveKeyReleasedListener(LLGP::Key::Right, this, [this]() { OnKeyReleased(LLGP::Key::Right); });
}

sf::IntRect player::GetSpriteRectByName(const std::string& name) const
{
	auto it = m_playerSprites.find(name);
	if (it != m_playerSprites.end())
	{
		return it->second; // Found the sprite
	}

	// Handle missing sprite
	throw std::runtime_error("Sprite not found: " + name);
}

// Init functions
void player::InitVariables()
{
	m_movementSpeed = MOVEMENTSPEED;
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

void player::FlipSprite()
{
	// Flip sprite left
	if (isFacingRight)
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

	isFacingRight = !isFacingRight;
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

		if (isFacingRight)
		{
			FlipSprite();
		}

		m_direction = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{

		if (!isFacingRight)
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

void player::UpdateFeetPosition()
{
	// Update the feet position (the bottom center of the player)
	m_feetPosition = sf::Vector2f(m_sprite.getPosition().x + m_sprite.getGlobalBounds().size.x / 2.f,
		m_sprite.getPosition().y + m_sprite.getGlobalBounds().size.y);
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

void player::SetPosition(float xPos, float yPos)
{
	m_sprite.setPosition(sf::Vector2f(xPos, yPos));
}

void player::StopHorizontalMovement()
{
	m_direction.x = 0.f;
	m_isJumping = false;
	m_canJump = true;
}

void player::StopJumpingMovement()
{
	m_isJumping = false;
	m_canJump = true;
}

void player::StopFalling()
{
	m_direction.y = 0.f;
	m_isGrounded = true;
	m_isJumping = false;
	m_canJump = true;	
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

