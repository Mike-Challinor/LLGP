#include "Bounder.h"

// --- Constructor ---
Bounder::Bounder(LLGP::AssetRegistry& assetRegistry, WaypointManager& waypointManager, float xPos, float yPos, const std::string& objectName)
    : Enemy(assetRegistry, waypointManager, xPos, yPos, objectName)
{
    m_pointValue = POINTS_VALUE_BOUNDER;
    debugName = "Bounder";
}

// --- Function for deciding the next move (overriden from Enemy class) ---
void Bounder::DecideNextMove(float deltaTime)
{
    Enemy::DecideNextMove(deltaTime);
}

// --- Update function ---
void Bounder::Update(float deltaTime)
{
    Enemy::Update(deltaTime);
}
