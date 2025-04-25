#include "Bounder.h"

Bounder::Bounder(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName)
    : Enemy(assetRegistry, xPos, yPos, objectName)
{
    FindTarget();
}

void Bounder::FindTarget()
{
    // Simple target selection for now (just fly across screen)
    if (m_sprite.getPosition().x < SCREEN_WIDTH / 2.f)
    {
        SetTarget({ SCREEN_WIDTH + 100.f, static_cast<float>(rand() % SCREEN_HEIGHT) });
    }
    else
    {
        SetTarget({ -100.f, static_cast<float>(rand() % SCREEN_HEIGHT) });
    }
}

void Bounder::DecideNextMove()
{

}

void Bounder::Update(float deltaTime)
{
    Enemy::Update(deltaTime);
}
