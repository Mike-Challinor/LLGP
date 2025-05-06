#include "Bounder.h"

Bounder::Bounder(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName)
    : Enemy(assetRegistry, xPos, yPos, objectName)
{
    FindTarget();
}

void Bounder::DecideNextMove()
{
    // If the enemy has collided, find a new target
    if (m_hasCollided)
    {
        FindTarget();
    }
}

void Bounder::Update(float deltaTime)
{
    Enemy::Update(deltaTime);
}
