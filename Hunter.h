#pragma once

#include "Enemy.h"
#include "Player.h"

class Hunter : public Enemy
{
public:
    // Constructor
    Hunter(LLGP::AssetRegistry& assetRegistry, 
        WaypointManager& waypointManager,
        float xPos, 
        float yPos, 
        const std::string& objectName = "hunter", 
        const std::vector<std::unique_ptr<Player>>& players = {});

    // Update functions
    void Update(float deltaTime) override;

private:
    // Vectors
    std::vector<Player*> m_players;

    // Functions
    void FindTarget() override;
    void DecideNextMove(float deltaTime) override;
};

