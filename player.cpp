     #include "Player.h"

// Constructor
Player::Player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, int player_id, const std::string& objectName)
	: Character(assetRegistry, xPos, yPos, objectName),  // Initialize Character base class
	m_inputManager(inputManager)
{
	// Set the players variables
	m_playerID = player_id;
	m_canFall = true;
	m_canCollide = true;
	m_pointValue = POINTS_VALUE_PLAYER;

	// --- Set player variables based off of playerID ---
	if (m_playerID == 1)
	{
		m_riderIndex = 0;
		m_playerColour = sf::Color::Yellow;
	}
	
	else
	{
		m_riderIndex = 1;
		m_playerColour = sf::Color::Cyan;
	}

	// Set the rect to the correct sprite for the players rider
	m_riderSprite.setTextureRect(m_assetRegistry.LoadSprite("rider", m_riderIndex));

	// Set the usable keys for the player
	SetUsableKeys(inputManager);

	// Initialise the animations
	InitAnimations();
}

// Destructor
Player::~Player()
{
	// Unsubscribe from key presses and releases
	for (const auto& key : m_usableKeys)
	{
		m_inputManager.RemoveKeyPressListener(key, this, m_keyPressHandlers[key]);
		m_inputManager.RemoveKeyReleasedListener(key, this, m_keyReleaseHandlers[key]);
	}
}

void Player::InitAnimations()
{
	m_animations[LLGP::AnimationState::idle] = Animation{ 1, 0 };
	m_animations[LLGP::AnimationState::walking] = Animation{ 4, 0 };
	m_animations[LLGP::AnimationState::flying] = Animation{ 2, 5 };

	// Check for null pointer
	if (m_animationComponent)
	{
		// Set the animation state to idle if pointer is not null
		m_animationComponent->SetAnimationState(LLGP::idle, m_textureMap, m_animations[LLGP::AnimationState::idle].numberOfFrames
			, m_animations[LLGP::AnimationState::idle].startingFrame, 1.f
		);
	}
	else
	{
		std::cerr << "AnimationComponent is not initialized!" << std::endl;
	}
}

void Player::SetUsableKeys(LLGP::InputManager& inputManager)
{
	switch (m_playerID)
	{
	case 1:
		m_usableKeys = {
			LLGP::Key::W,
			LLGP::Key::A,
			LLGP::Key::D
		};
		break;

	case 2:
		m_usableKeys = {
			LLGP::Key::Up,
			LLGP::Key::Left,
			LLGP::Key::Right
		};
		break;
	}

	// Set the key listeners
	for (const auto& key : m_usableKeys)
	{
		// Key press listeners
		m_keyPressHandlers[key] = [this, key]() { keyInputListener(key); };
		m_inputManager.AddKeyPressListener(key, this, m_keyPressHandlers[key]);

		// Key release listeners
		m_keyReleaseHandlers[key] = [this, key]() { OnKeyReleased(key); };
		m_inputManager.AddKeyReleasedListener(key, this, m_keyReleaseHandlers[key]);
	}
}

void Player::StartRespawnTimer()
{
	m_respawnTimer.Reset();
}

void Player::UpdateForceDecrement()
{
	Character::UpdateForceDecrement();

	if (m_dynamicForce.x <= INITIAL_COLLISION_FORCE / 2);
		m_canInput = true;
}

void Player::UpdateMovementDirection()
{
	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{

		if (m_isFacingRight)
		{
			FlipSprite();
		}

		m_velocity = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{

		if (!m_isFacingRight)
		{
			FlipSprite();
		}

		m_velocity = sf::Vector2f(m_movementSpeed, 0.f);
	}

	else
	{
		m_velocity = sf::Vector2f(0.f, 0.f);
	}
}

void Player::keyInputListener(LLGP::Key key)
{
	m_activeKeys.insert(key);

	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{
		m_isMoving = true;
		m_velocity = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{
		m_isMoving = true;
		m_velocity = sf::Vector2f(m_movementSpeed, 0.f);
	}

	if (m_activeKeys.count(LLGP::Key::W) || m_activeKeys.count(LLGP::Key::Up))
	{
		Jump();
	}
}


void Player::OnKeyReleased(LLGP::Key key)
{
	m_activeKeys.erase(key);

	// Check remaining keys to maintain movement
	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{
		m_velocity = sf::Vector2f(-m_movementSpeed, 0.f);
	}
	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{
		m_velocity = sf::Vector2f(m_movementSpeed, 0.f);
	}
	else
	{
		m_isMoving = false;

		if (!m_isJumping)
		{
			// Only stop if no keys are held
			m_velocity = sf::Vector2f(0.f, 0.f);
		}
	}
}

int Player::GetScore() const
{
	return m_score;
}

int Player::GetLives() const
{
	return m_lives;
}

int Player::GetPlayerID() const
{
	return m_playerID;
}

int Player::GetCanRespawn() const
{
	return m_canRespawn;
}

void Player::Despawn()
{
	m_lives--;
	m_canRespawn = false;
	m_respawnTimer.Start();
	Character::Despawn();
}

void Player::AddScore(int pointsToAdd)
{
	m_score += pointsToAdd;
}

void Player::AddNewForce(sf::Vector2f forceToAdd)
{
	Character::AddNewForce(forceToAdd);
	m_canInput = false;
}

void Player::SetIsAlive(bool isAlive)
{
	m_isAlive = isAlive;
}

// Update functions
void Player::UpdateInput()
{
	if (m_canInput)
	{
		// Keyboard input
		m_inputManager.Update();
		UpdateMovementDirection();

	}
}

void Player::Update(float deltaTime)
{
	// Update the player while alive
	if (m_isAlive)
	{
		Character::Update(deltaTime);
	}	
}

void Player::UpdateRespawnTimer(float deltaTime)
{
	// Update the respawn timer when dead
	m_respawnTimer.Update(deltaTime);

	// Allow respawning once timer has finished
	if (m_respawnTimer.IsFinished() && m_lives != 0)
	{
		m_canRespawn = true;
	}
}

void Player::Render(sf::RenderTarget& target)
{
	if (m_isAlive)
	{
		target.draw(m_riderSprite);
		GameObject::Render(target);
	}
}

