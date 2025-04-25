     #include "player.h"

// Constructor
player::player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, int player_id, const std::string& objectName)
	: Character(assetRegistry, xPos, yPos, objectName),  // Initialize Character base class
	m_inputManager(inputManager)
{
	// Set the players id
	m_playerID = player_id;
	m_canFall = true;

	SetUsableKeys(inputManager);

	InitAnimations();
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

void player::SetUsableKeys(LLGP::InputManager& inputManager)
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
		inputManager.AddKeyPressListener(key, this, [this, key]() { keyInputListener(key); });

		// Key release listeners
		inputManager.AddKeyReleasedListener(key, this, [this, key]() { OnKeyReleased(key); });
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

void player::keyInputListener(LLGP::Key key)
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


void player::OnKeyReleased(LLGP::Key key)
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

// Update functions
void player::UpdateInput()
{
	// Keyboard input
	m_inputManager.Update();

	UpdateMovementDirection();
	
}

void player::Update(float deltaTime)
{
	Character::Update(deltaTime);
}

// Render functions
void player::Render(sf::RenderTarget& target)
{
	GameObject::Render(target);
}

