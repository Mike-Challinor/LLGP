#include "Enemy.h"

// --- Constructor ---
Enemy::Enemy(LLGP::AssetRegistry& assetRegistry, WaypointManager& waypointManager, float xPos, float yPos, const std::string& objectName) :
	Character(assetRegistry, xPos, yPos, objectName), m_waypointManager(waypointManager)
{
	// Set variables for the rider
	m_riderXOffset = 9;
	m_riderYOffset = -4;
	m_riderIndex = 2;

	// Set the objects name for debugging
	m_objectName = m_objectName;
	
	// Set the texture rect of the rider
	m_riderSprite.setTextureRect(m_assetRegistry.LoadSprite("rider", m_riderIndex));

	// Initialise variables and animations
	InitVariables();
	InitAnimations();

}

// --- Destructor ---
Enemy::~Enemy()
{

}

// --- Function for initialising the enemies animations ---
void Enemy::InitAnimations()
{
	m_animations[LLGP::AnimationState::idle] = Animation{ 1, 0 };
	m_animations[LLGP::AnimationState::walking] = Animation{ 4, 0 };
	m_animations[LLGP::AnimationState::flying] = Animation{ 2, 5 };

	// Check for null pointer
	if (m_animationComponent)
	{
		// Set the animation state to idle if pointer is not null
		m_animationComponent->SetAnimationState(LLGP::idle, m_textureMap, m_animations[LLGP::AnimationState::idle].numberOfFrames
			, m_animations[LLGP::AnimationState::idle].startingFrame
		);
	}
	else
	{
		std::cerr << "AnimationComponent is not initialized!" << std::endl;
	}
}

// --- Functions for initialising the enemies variables ---
void Enemy::InitVariables()
{
	// Set the movement speed
	m_movementSpeed = ENEMY_MAX_WALKING_SPEED;

	// Initialize the first waypoint if available
	if (!m_currentWaypoint && m_waypointManager.HasWaypoints())
	{
		m_currentWaypoint = m_waypointManager.GetNextWaypoint();
	}

	// Flip sprite if needed
	if (m_sprite.getGlobalBounds().position.x + (m_sprite.getGlobalBounds().size.x / 2) > SCREEN_WIDTH / 2)
	{
		if (m_isFacingRight)
		{
			FlipSprite();
		}
	}
}

// --- Function for updating the windows bound collision ---
void Enemy::UpdateWindowsBoundCollision()
{
	// Top bounds of the screen
	if (this->CheckTopColl())
	{
		m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, 0.f));
	}

	// Bottom bounds of the screen

	else if (this->CheckFeetColl())
	{
		m_sprite.setPosition(sf::Vector2f(m_sprite.getGlobalBounds().position.x, SCREEN_HEIGHT - m_sprite.getGlobalBounds().size.y));
	}
}

// --- Function for setting the target position of the enemies target ---
void Enemy::SetTarget(sf::Vector2f targetPosition)
{
	m_targetPosition = targetPosition;
}

// --- Function for moving the enemy towards the target ---
void Enemy::MoveTowardsTarget(const sf::Vector2f& target, float deltaTime)
{
	// Get the current position of the enemy
	sf::Vector2f currentPosition = GetPosition();

	// Calculate the direction vector from the current position to the target
	sf::Vector2f direction = target - currentPosition;

	// Get the distance from the current position to the target
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// If the distance is small enough, stop the movement
	if (distance < 0.1f)
	{
		m_velocity = { 0.f, 0.f }; // Stop the movement
		m_isMoving = false;
		m_hasTarget = false;
		FlipSprite(); 
		return;
	}

	// Normalize the direction vector (make it a unit vector)
	direction /= distance;

	// Scale the velocity by movement speed
	float speed = m_movementSpeed; // Use the movement speed here
	m_velocity = direction * speed; // Velocity is the direction times speed

	// Apply the velocity to the position (using deltaTime for frame-rate independence)
	sf::Vector2f newPosition = currentPosition + m_velocity * deltaTime;

	// Update position directly using velocity
	SetPosition(newPosition.x, newPosition.y);

	// Set the enemy moving to true
	m_isMoving = true;

	// Flip sprite if necessary
	if (direction.x < 0 && m_isFacingRight || direction.x > 0 && !m_isFacingRight)
	{
		FlipSprite();
	}
}

// --- Move enemy towards a waypoint ---
void Enemy::MoveTowardsWaypoint(float deltaTime) {
	if (m_currentWaypoint == nullptr) {
		m_currentWaypoint = m_waypointManager.GetNextWaypoint();
		if (m_currentWaypoint == nullptr) {
			return;
		}
	}

	sf::Vector2f currentPosition = GetPosition();
	sf::Vector2f direction = m_currentWaypoint->position - currentPosition;

	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// If close enough, consider the waypoint reached and stop
	if (distance < 0.1f) {
		if (!m_currentWaypoint->isActivated) {
			m_currentWaypoint->isActivated = true;
		}
		return;  // Stop moving
	}

	// Normalize the direction and apply movement speed
	direction /= distance;
	m_velocity = direction * m_movementSpeed;

	// Update position based on velocity
	sf::Vector2f newPosition = currentPosition + m_velocity * deltaTime;
	SetPosition(newPosition.x, newPosition.y);

	// Set the enemy to be moving
	m_isMoving = true;

	// Flip sprite if needed
	if (direction.x < 0 && m_isFacingRight || direction.x > 0 && !m_isFacingRight) {
		FlipSprite();
	}
}

// --- Function for finding a target ---
void Enemy::FindTarget()
{
	if (!m_hasTarget)
	{
		// Simple target selection for now (just fly across screen)
		if (m_isFacingRight)
		{
			SetTarget({ SCREEN_WIDTH - 100.f, static_cast<float>(rand() % SCREEN_HEIGHT - 75.f) });
		}
		else
		{
			SetTarget({ 100.f, static_cast<float>(rand() % SCREEN_HEIGHT - 75.f) });
		}

		m_hasTarget = true;
	}
	
}

// --- Function for setting the riders position relevant to the enemies position ---
void Enemy::SetRiderPosition()
{

	if (m_animationComponent->GetState() == LLGP::idle || m_animationComponent->GetState() == LLGP::walking)
	{
		m_riderYOffset = -9;
	}

	else
	{
		m_riderYOffset = -4;
	}

	// Get the sprite’s actual visual origin (in world space)
	sf::Vector2f charPos = m_sprite.getPosition();
	sf::Vector2f scale = m_sprite.getScale();

	// Calculate xOffset based on facing direction
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

// --- Main update function ---
void Enemy::Update(float deltaTime)
{
	DecideNextMove(deltaTime);
	FindTarget();
	MoveTowardsTarget(m_targetPosition, deltaTime);
	//MoveTowardsWaypoint(deltaTime);

	Character::Update(deltaTime);
}

// --- Function for deciding the next move ---
void Enemy::DecideNextMove(float deltaTime) 
{
	// If the current waypoint is reached, pick a new one
	if (m_currentWaypoint && m_currentWaypoint->isActivated) 
	{
		// Set to the next waypoint
		if (m_waitTime <= 0) {
			m_currentWaypoint = m_waypointManager.GetNextWaypoint();
			m_waitTime = ENEMY_WAIT_TIME;  // Reset wait time
		}
		else 
		{
			m_waitTime -= deltaTime;  // Decrease wait time
		}
	}

	if (m_currentWaypoint == nullptr) {
		// No waypoints left or no target defined, find a new target
		FindTarget();
	}
}

// --- Function for resetting the enemies target ---
void Enemy::ResetTarget()
{
	m_hasCollided = true;
	m_hasTarget = false;
}

// --- Function for removing the rider from the enemy ---
void Enemy::RemoveRider()
{
	m_hasRider = false;
}
