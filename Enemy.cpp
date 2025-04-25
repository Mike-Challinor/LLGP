#include "Enemy.h"

Enemy::Enemy(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName) :
	Character(assetRegistry, xPos, yPos, objectName)
{
	m_objectName = m_objectName;
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
}

void Enemy::SetTarget(sf::Vector2f targetPosition)
{
	m_targetPosition = targetPosition;
}

void Enemy::MoveToTarget()
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
	}
}

void Enemy::FindTarget()
{
}

void Enemy::Update(float deltaTime)
{
	DecideNextMove();
	MoveToTarget();
	Character::Update(deltaTime);
}

void Enemy::DecideNextMove()
{

}
