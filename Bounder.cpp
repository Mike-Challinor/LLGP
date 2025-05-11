#include "Bounder.h"

Bounder::Bounder(LLGP::AssetRegistry& assetRegistry, WaypointManager& waypointManager, float xPos, float yPos, const std::string& objectName)
    : Enemy(assetRegistry, waypointManager, xPos, yPos, objectName)
{
    m_pointValue = POINTS_VALUE_BOUNDER;
    debugName = "Bounder";
}

void Bounder::DecideNextMove(float deltaTime)
{
    Enemy::DecideNextMove(deltaTime);

    /*
    // If the enemy has collided, find a new target
    if (m_hasCollided)
    {
        FindTarget();
    }*/
}

void Bounder::Update(float deltaTime)
{
    Enemy::Update(deltaTime);
}
