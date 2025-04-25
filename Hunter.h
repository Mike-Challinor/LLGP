#pragma once

#include "Enemy.h"
#include "player.h"

class Hunter : public Enemy
{
public:
    Hunter(LLGP::AssetRegistry& assetRegistry, 
        float xPos, 
        float yPos, 
        const std::string& objectName = "hunter", 
        const std::vector<std::unique_ptr<player>>& players = {});

    void Update(float deltaTime) override;

private:
    // Vectors
    std::vector<player*> m_players;


    void FindTarget() override;
    void DecideNextMove() override;
};

