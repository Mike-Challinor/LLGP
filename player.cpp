     #include "Player.h"

// --- Constructor ---
Player::Player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, int player_id, const std::string& objectName)
	: Character(assetRegistry, xPos, yPos, objectName),  // Initialize Character base class
	m_inputManager(inputManager)
{
	// Set the players variables
	m_playerID = player_id;
	m_canFall = true;
	m_canCollide = true;
	m_pointValue = POINTS_VALUE_PLAYER;

	// Set player variables based off of playerID
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

// --- Destructor ---
Player::~Player()
{
	// Unsubscribe from key presses and releases
	for (const auto& key : m_usableKeys)
	{
		m_inputManager.RemoveKeyPressListener(key, this, m_keyPressHandlers[key]);
		m_inputManager.RemoveKeyReleasedListener(key, this, m_keyReleaseHandlers[key]);
	}
}

// --- Function for initialising animation (overriden from Character class) ---
void Player::InitAnimations()
{
	// Set animations map for each state 
	m_animations[LLGP::AnimationState::idle] = Animation{ 1, 0 };
	m_animations[LLGP::AnimationState::walking] = Animation{ 4, 0 };
	m_animations[LLGP::AnimationState::flying] = Animation{ 2, 5 };

	// Error checking for null pointer
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

// --- Function for setting the players usable keys (depending on playerID) ---
void Player::SetUsableKeys(LLGP::InputManager& inputManager)
{
	// Switch statement for each player
	switch (m_playerID)
	{
	case 1: // Player 1 
		m_usableKeys = {
			LLGP::Key::W,
			LLGP::Key::A,
			LLGP::Key::D
		};
		break;

	case 2: // Player 2
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

// --- Function for starting the respawn timer ---
void Player::StartRespawnTimer()
{
	m_respawnTimer.Reset();
}

// --- Function for decrementing dynamic forces (overriden from Character class) ---
void Player::UpdateForceDecrement()
{
	Character::UpdateForceDecrement(); // Call base function

	// Set whether inputs can be registered if enough of the force has been applied
	if (m_dynamicForce.x <= INITIAL_COLLISION_FORCE / 2);
		m_canInput = true;
}

// --- Function for updating players movement direction (overriden from Character class) ---
void Player::UpdateMovementDirection()
{
	// For A key or left key
	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{
		// Flip sprite if needed
		if (m_isFacingRight)
		{
			FlipSprite();
		}

		// Set the velocity of the player
		m_velocity = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	// For D key or right key
	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{
		// Flip sprite if needed
		if (!m_isFacingRight)
		{
			FlipSprite();
		}

		// Set the velocity of the player
		m_velocity = sf::Vector2f(m_movementSpeed, 0.f);
	}

	else
	{
		// Set the velocity of the player to 0
		m_velocity = sf::Vector2f(0.f, 0.f);
	}
}

// --- Function for key pressed listening ---
void Player::keyInputListener(LLGP::Key key)
{
	m_activeKeys.insert(key); // Set key as active

	// If A or left key then move player left
	if (m_activeKeys.count(LLGP::Key::A) || m_activeKeys.count(LLGP::Key::Left))
	{
		m_isMoving = true;
		m_velocity = sf::Vector2f(-m_movementSpeed, 0.f);
	}

	// If D or right key then move player right
	else if (m_activeKeys.count(LLGP::Key::D) || m_activeKeys.count(LLGP::Key::Right))
	{
		m_isMoving = true;
		m_velocity = sf::Vector2f(m_movementSpeed, 0.f);
	}

	// If W or Up key then make player jump
	if (m_activeKeys.count(LLGP::Key::W) || m_activeKeys.count(LLGP::Key::Up))
	{
		// Call jump function
		Jump();
	}
}

// --- Function for key released listening ---
void Player::OnKeyReleased(LLGP::Key key)
{
	m_activeKeys.erase(key); // Set key as inactive

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

// --- Function for returning the players score ---
int Player::GetScore() const
{
	return m_score;
}

// --- Function for returning the players remaining lives ---
int Player::GetLives() const
{
	return m_lives;
}

// --- Function for returning the players id ---
int Player::GetPlayerID() const
{
	return m_playerID;
}

// --- Function for returning whether the player can respawn ---
int Player::GetCanRespawn() const
{
	return m_canRespawn;
}

// --- Function for despawning the player (overriden from Character class) ---
void Player::Despawn()
{
	m_lives--; // Decrement players lives
	m_canRespawn = false; // Set the player respawn to false
	m_respawnTimer.Start(); // Start respawn timer
	Character::Despawn(); // Call function on base class
}

// --- Function for adding score to the player ---
void Player::AddScore(int pointsToAdd)
{
	m_score += pointsToAdd;
}

// --- Function for adding a new force to the player ---
void Player::AddNewForce(sf::Vector2f forceToAdd)
{
	Character::AddNewForce(forceToAdd);
	m_canInput = false;
}

// --- Function for setting whether the player is alive ---
void Player::SetIsAlive(bool isAlive)
{
	m_isAlive = isAlive;
}

// --- Main update function ---
void Player::Update(float deltaTime)
{
	// Update the player while alive
	if (m_isAlive)
	{
		Character::Update(deltaTime);
	}
}

// --- Function for updating inputs ---
void Player::UpdateInput()
{
	if (m_canInput)
	{
		// Keyboard input
		m_inputManager.Update(); // Update the input manager
		UpdateMovementDirection(); // Update the players movement direction

	}
}

// --- Function for updating the players respawn timer ---
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

// --- Function for rendering the player (overriden from GameObjects class) ---
void Player::Render(sf::RenderTarget& target)
{
	if (m_isAlive) // Only render if the player is alive
	{
		target.draw(m_riderSprite); // Render the rider sprite
		GameObject::Render(target); // Call GameObjects base function (render the main sprite)
	}
}

