     #include "player.h"

// Constructor
player::player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float x, float y, int player_id)
	: m_inputManager(inputManager), m_assetRegistry(assetRegistry), m_mountSprite(m_texture)
{
	// Set the players id
	m_playerID = player_id;

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

	// Set players position
	m_mountSprite.setPosition(sf::Vector2f(x, y));

	// Get the ostrich/stork spritemap
	m_texture = assetRegistry.LoadTexture();
	m_playerSprites = assetRegistry.LoadPlayerSprites(m_playerID);

	InitAnimations();

	m_mountSprite.setTexture(m_texture);

	// Create pointer to the animation component
	m_animationComponent = make_unique<LLGP::AnimationComponent>(m_mountSprite, m_mountName);

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
	this->m_movementSpeed = MOVEMENTSPEED;
}

void player::InitAnimations()
{
	m_animations[LLGP::AnimationState::idle] = Animation{ 1, 0 };
	m_animations[LLGP::AnimationState::walking] = Animation{ 4, 0 };
	m_animations[LLGP::AnimationState::flying] = Animation{ 2, 6 };
}

// Collision Checks
bool player::checkLeftColl()
{
	if (m_mountSprite.getGlobalBounds().position.x <= 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool player::checkRightColl()
{
	if (m_mountSprite.getGlobalBounds().position.x + m_mountSprite.getGlobalBounds().size.x >= SCREEN_WIDTH)
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
	if (m_mountSprite.getGlobalBounds().position.y <= 0.f)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool player::checkBottomColl()
{
	if (m_mountSprite.getGlobalBounds().position.y + m_mountSprite.getGlobalBounds().size.y >= SCREEN_HEIGHT)
	{
		return true;
	}

	else
	{
		return false;
	}
}

void player::FlipSprite()
{
	// Flip sprite left
	if (isFacingRight)
	{
		// Set the origin to the sprite's center (or appropriate pivot point)
		m_mountSprite.setOrigin(sf::Vector2f(m_mountSprite.getLocalBounds().size.x, 0.f));
		// Set the scale to flip
		m_mountSprite.setScale(sf::Vector2f(-1.f, 1.f));
	}

	// Flip sprite right
	else
	{
		// Set the origin to the sprite's center (or appropriate pivot point)
		m_mountSprite.setOrigin(sf::Vector2f(0.f, 0.f));

		// Set the scale to flip
		m_mountSprite.setScale(sf::Vector2f(1.f, 1.f));
	}

	isFacingRight = !isFacingRight;
}

void player::AddGravity()
{
	sf::Vector2f playerPos = m_mountSprite.getPosition();

	// Add gravity
	if (playerPos.y + m_mountSprite.getGlobalBounds().size.y < SCREEN_HEIGHT)
	{
		// Set position with gravity added
		m_mountSprite.setPosition(sf::Vector2f(playerPos.x, playerPos.y += GRAVITY));
	}

	else
	{
		m_canJump = true;
		m_isGrounded = true;

	}
}

void player::Move()
{
	// Apply movement to the player sprite
	m_mountSprite.setPosition(m_mountSprite.getPosition() + m_direction);
}

void player::Jump()
{
	if (!m_isJumping)
	{
		m_isJumping = true;
		m_jumpForce = INITIAL_JUMP_FORCE;
		m_canJump = false;
	}
}

void player::ReduceJumpForce()
{
	// Adjust position
	float yPos = m_mountSprite.getPosition().y;
	float newYPos = yPos -= m_jumpForce;
	m_mountSprite.setPosition(sf::Vector2f(m_mountSprite.getGlobalBounds().position.x, newYPos));


	// Reduce jump force
	m_jumpForce -= JUMP_FORCE_DECREMENT;

	// Is jump force 0?
	if (m_jumpForce <= 0.0f)
	{
		m_isJumping = false;
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

void player::keyInputListener(LLGP::Key key)
{
	m_activeKeys.insert(key);

	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{
		if (m_isGrounded)
		{
			m_animationComponent->SetAnimationState(LLGP::walking, m_playerSprites, m_animations[LLGP::AnimationState::walking].numberOfFrames
				, m_animations[LLGP::AnimationState::walking].startingFrame);
		}
		
		
		m_direction = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{
		
		if (m_isGrounded)
		{
			m_animationComponent->SetAnimationState(LLGP::walking, m_playerSprites, m_animations[LLGP::AnimationState::walking].numberOfFrames
				, m_animations[LLGP::AnimationState::walking].startingFrame);
		}

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
		if (!m_isJumping)
		{
			// Only stop if no keys are held
			m_direction = sf::Vector2f(0.f, 0.f);

			m_animationComponent->SetAnimationState(LLGP::idle, m_playerSprites, m_animations[LLGP::AnimationState::idle].numberOfFrames
				, m_animations[LLGP::AnimationState::idle].startingFrame);
		}
		
	}
}



// Update functions
void player::updateInput()
{
	// Keyboard input
	m_inputManager.Update();

	UpdateMovementDirection();
	
}

void player::updateWindowsBoundCollision()
{
	// Left
	if (this->checkLeftColl())
	{
		m_mountSprite.setPosition(sf::Vector2f(0.f, m_mountSprite.getGlobalBounds().position.y));
	}

	// Right
	else if (this->checkRightColl())
	{
		m_mountSprite.setPosition(sf::Vector2f(SCREEN_WIDTH - m_mountSprite.getGlobalBounds().size.x, m_mountSprite.getGlobalBounds().position.y));
	}

	// Top
	if (this->checkTopColl())
	{
		m_mountSprite.setPosition(sf::Vector2f(m_mountSprite.getGlobalBounds().position.x, 0.f));
	}

	// Bottom

	else if (this->checkBottomColl())
	{
		m_mountSprite.setPosition(sf::Vector2f(m_mountSprite.getGlobalBounds().position.x, SCREEN_HEIGHT - m_mountSprite.getGlobalBounds().size.y));
	}
}

void player::update()
{
	// Update animation component
	m_animationComponent->Update();

	// Update window bounds collision
	this->updateWindowsBoundCollision();

	// Reduce jump force if jumping
	if (m_isJumping)
	{
		ReduceJumpForce();
	}

	// Apply movement
	this->Move();

	// Apply gravity to player
	this->AddGravity();
}

// Render functions
void player::render(sf::RenderTarget& target)
{
	target.draw(this->m_mountSprite);
}

