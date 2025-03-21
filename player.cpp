     #include "player.h"

// Constructor
player::player(LLGP::InputManager& inputManager, float x, float y, int player_id)
	: m_inputManager(inputManager), ostrichSprite(texture)
{
	// Set the players id
	m_playerID = player_id;

	// Add listeners for keys
	if (m_playerID == 1)
	{
		inputManager.AddKeyListener(LLGP::Key::W, this, [this]() { keyInputListener(LLGP::Key::W); });
		inputManager.AddKeyListener(LLGP::Key::A, this, [this]() { keyInputListener(LLGP::Key::A); });
		inputManager.AddKeyListener(LLGP::Key::D, this, [this]() { keyInputListener(LLGP::Key::D); });
	}

	else if (m_playerID == 2)
	{
		inputManager.AddKeyListener(LLGP::Key::Up, this, [this]() { keyInputListener(LLGP::Key::Up); });
		inputManager.AddKeyListener(LLGP::Key::Left, this, [this]() { keyInputListener(LLGP::Key::Left); });
		inputManager.AddKeyListener(LLGP::Key::Right, this, [this]() { keyInputListener(LLGP::Key::Right); });
	}

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
}

// Destructor
player::~player()
{
	// Remove listeners if necessary
	m_inputManager.RemoveKeyListener(LLGP::Key::W, this, [this]() { keyInputListener(LLGP::Key::W); });
	m_inputManager.RemoveKeyListener(LLGP::Key::A, this, [this]() { keyInputListener(LLGP::Key::A); });
	m_inputManager.RemoveKeyListener(LLGP::Key::D, this, [this]() { keyInputListener(LLGP::Key::D); });
}

// Init functions
void player::initVariables()
{
	this->m_movementSpeed = MOVEMENTSPEED;
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
	if (ostrichSprite.getGlobalBounds().position.x + ostrichSprite.getGlobalBounds().size.x >= screen_bounds.size.x)
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

void player::AddGravity()
{
	sf::Vector2f playerPos = ostrichSprite.getPosition();

	// Add gravity
	if (playerPos.y + ostrichSprite.getGlobalBounds().size.y < SCREEN_HEIGHT)
	{
		// Set position with gravity added
		ostrichSprite.setPosition(sf::Vector2f(playerPos.x, playerPos.y += GRAVITY));
	}

	else
	{
		m_canJump = true;
	}
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

void player::keyInputListener(LLGP::Key key)
{
	switch (key)
	{
	case LLGP::Key::W:

		if (m_canJump)
		{
			std::cout << "W key pressed! Jumping...\n";
			Jump();
		}
		
		break;

	case LLGP::Key::Up:

		if (m_canJump)
		{
			std::cout << "Up key pressed! Jumping...\n";
			Jump();
		}
		break;

	case LLGP::Key::A:

		std::cout << "A key pressed! Moving left...\n";

		// Move left
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(-m_movementSpeed, 0.f));
		break;

	case LLGP::Key::Left:

		std::cout << "Left key pressed! Moving left...\n";

		// Move left
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(-m_movementSpeed, 0.f));
		break;

	case LLGP::Key::D:

		std::cout << "D key pressed! Moving right...\n";

		// Move right
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(m_movementSpeed, 0.f));
		break;


	case LLGP::Key::Right:

		std::cout << "Right key pressed! Moving right...\n";

		// Move right
		ostrichSprite.setPosition(ostrichSprite.getPosition() + sf::Vector2f(m_movementSpeed, 0.f));
		break;

	default:
		std::cout << "Unhandled key pressed.\n";
		break;
	}
}


// Update functions
void player::updateInput()
{
	// Keyboard input
	m_inputManager.Update();
	
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
	// Update window bounds collision
	this->updateWindowsBoundCollision(screen_bounds);

	if (m_isJumping)
	{
		//adjust position
		float yPos = ostrichSprite.getPosition().y;
		float newYPos = yPos -= m_jumpForce;
		ostrichSprite.setPosition(sf::Vector2f(ostrichSprite.getGlobalBounds().position.x, newYPos));


		//reduce jump force
		m_jumpForce -= JUMP_FORCE_DECREMENT;

		// Is jump force 0?
		if (m_jumpForce <= 0.0f)
		{
			m_isJumping = false;
		}
	}

	// Apply gravity to player
	this->AddGravity();
}

// Render functions
void player::render(sf::RenderTarget& target)
{
	target.draw(this->ostrichSprite);
}

