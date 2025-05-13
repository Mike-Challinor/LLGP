#include "Hunter.h"

// --- Constructor ---
Hunter::Hunter(LLGP::AssetRegistry& assetRegistry,
    WaypointManager& waypointManager,
    float xPos,
    float yPos,
    const std::string& objectName,
    const std::vector<std::unique_ptr<Player>>& players)
    : Enemy(assetRegistry, waypointManager, xPos, yPos, objectName)
{
    // Set the name for debuggig
    debugName = "Hunter";

    // Set the points value for the hunter
    m_pointValue = POINTS_VALUE_HUNTER;

    // Get refs to the players
    for (const auto& p : players)
    {
        m_players.push_back(p.get());
    }

    // Find an initial target
    FindTarget();
}

// --- Function for finding a target (overidden from Enemy class) ---
void Hunter::FindTarget()
{
    if (m_players.empty()) return; // Return if no players are passed through

    // Get the closest distance
    float closestDistance = std::numeric_limits<float>::max();
    sf::Vector2f closestPos;

    // Get the hunters current position
    sf::Vector2f myPos = m_sprite.getPosition();

    // Loop through the players and check the distance from the hunter
    for (Character* player : m_players)
    {
        // Get the distance from the player
        float distance = std::hypot(
            player->GetPosition().x - myPos.x,
            player->GetPosition().y - myPos.y
        );

        // If the distance of the player is less than the closest distance
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPos = player->GetPosition(); // Set the closest position to the players position
        }
    }

    // Set the target position
    SetTarget(closestPos);
}

// --- Function for deciding the next move (overidden from Enemy c;ass) ---
void Hunter::DecideNextMove(float deltaTime)
{   
    // Call the base function
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

// --- Main update function ---
void Hunter::Update(float deltaTime)
{
    // --- Call the base update function ---
    Enemy::Update(deltaTime);
}
