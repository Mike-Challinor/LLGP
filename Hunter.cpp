#include "Hunter.h"

Hunter::Hunter(LLGP::AssetRegistry& assetRegistry,
    WaypointManager& waypointManager,
    float xPos,
    float yPos,
    const std::string& objectName,
    const std::vector<std::unique_ptr<Player>>& players)
    : Enemy(assetRegistry, waypointManager, xPos, yPos, objectName)
{
    debugName = "Hunter";
    m_pointValue = POINTS_VALUE_HUNTER;

    // Get refs to the players
    for (const auto& p : players)
    {
        m_players.push_back(p.get());
    }

    FindTarget();
}

void Hunter::FindTarget()
{
    if (m_players.empty()) return; // Return if no players are passed through

    float closestDistance = std::numeric_limits<float>::max();
    sf::Vector2f closestPos;

    sf::Vector2f myPos = m_sprite.getPosition();

    for (Character* player : m_players)
    {
        float distance = std::hypot(
            player->GetPosition().x - myPos.x,
            player->GetPosition().y - myPos.y
        );

        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPos = player->GetPosition();
        }
    }

    SetTarget(closestPos);
}

void Hunter::DecideNextMove(float deltaTime)
{   
    Enemy::DecideNextMove(deltaTime);

    if (m_hasCollided)
    {
        // Find a target off screen
        Enemy::FindTarget();
    }

    else
    {
        // Continuously re-target the player
        FindTarget();
    }
    
}

void Hunter::Update(float deltaTime)
{
    Enemy::Update(deltaTime);
}
