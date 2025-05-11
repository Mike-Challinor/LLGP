#include "Enemy.h"

Enemy::Enemy(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName) :
	Character(assetRegistry, xPos, yPos, objectName)
{
	m_riderXOffset = 9;
	m_riderYOffset = -4;
	m_riderIndex = 2;
	m_objectName = m_objectName;
	
	m_riderSprite.setTextureRect(m_assetRegistry.LoadSprite("rider", m_riderIndex));

	InitVariables();
	InitAnimations();

}

Enemy::~Enemy()
{

}

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

void Enemy::InitVariables()
{
	m_movementSpeed = ENEMY_MAX_WALKING_SPEED;

	if (m_sprite.getGlobalBounds().position.x + (m_sprite.getGlobalBounds().size.x / 2) > SCREEN_WIDTH / 2)
	{
		if (m_isFacingRight)
		{
			FlipSprite();
		}
	}
}

void Enemy::UpdateWindowsBoundCollision()
{
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

void Enemy::SetTarget(sf::Vector2f targetPosition)
{
	m_targetPosition = targetPosition;
}

void Enemy::MoveToTarget()
{
	if (m_hasTarget)
	{
		sf::Vector2f currentPos = m_sprite.getPosition();
		sf::Vector2f direction = m_targetPosition - currentPos;

		if (direction.x < 0 && m_isFacingRight || direction.x > 0 && !m_isFacingRight)
		{
			FlipSprite();
		}

		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance > 0.1f)
		{
			direction /= distance;

			if (m_isFalling)
			{
				m_movementSpeed = ENEMY_MAX_FLYING_HORIZONTAL_SPEED;
				m_verticalMovementSpeed = ENEMY_MAX_FLYING_VERTICAL_SPEED;
			}

			else
			{
				m_movementSpeed = ENEMY_MAX_WALKING_SPEED;
				m_verticalMovementSpeed = ENEMY_MAX_WALKING_SPEED;
			}

			// Apply the horizontal movement
			m_velocity.x = direction.x * m_movementSpeed;
			m_velocity.y = direction.y * m_verticalMovementSpeed;

			m_isMoving = true;
		}
		else
		{
			m_velocity = { 0.f, 0.f };
			m_isMoving = false;
			m_hasTarget = false;
			FlipSprite();
		}
	}
	
}

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
	int xOffset = m_riderXOffset;

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


void Enemy::Update(float deltaTime)
{
	DecideNextMove();
	MoveToTarget();
	//MoveTowardsTarget(m_targetPosition, deltaTime);

	Character::Update(deltaTime);
}

void Enemy::DecideNextMove()
{
	FindTarget();
}

void Enemy::ResetTarget()
{
	m_hasCollided = true;
	m_hasTarget = false;
}

void Enemy::MoveTowardsTarget(const sf::Vector2f& target, float deltaTime)
{
	// Get the current position of the enemy
	sf::Vector2f currentPosition = GetPosition();

	// Calculate the direction vector from the current position to the target
	sf::Vector2f direction = target - currentPosition;

	// Get the distance from the current position to the target
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// Normalize the direction vector to make it a unit vector (just the direction, not the magnitude)
	direction /= distance;

	// Speed scaling based on distance
	float speed = ENEMY_MAX_WALKING_SPEED;

	// Calculate the new position based on the direction, speed, and deltaTime
	sf::Vector2f newPosition = currentPosition + direction * speed * deltaTime;

	// Set the new position of the enemy
	SetPosition(newPosition.x, newPosition.y);
}

void Enemy::RemoveRider()
{
	m_hasRider = false;
}
