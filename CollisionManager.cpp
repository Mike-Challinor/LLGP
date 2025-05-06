#include "CollisionManager.h"

void CollisionManager::HandlePlatformCollision(Character& character, Platform& platform)
{
    sf::FloatRect playerBounds = character.GetCollisionBounds();
    sf::FloatRect platformBounds = platform.GetCollisionBounds();


    if (playerBounds.findIntersection(platformBounds).has_value())
    {
        // Center points
        float dx = (playerBounds.position.x + playerBounds.size.x / 2) - (platformBounds.position.x + platformBounds.size.x / 2);
        float dy = (playerBounds.position.y + playerBounds.size.y / 2) - (platformBounds.position.y + platformBounds.size.y / 2);

        float overlapX = (playerBounds.size.x + platformBounds.size.x) / 2 - std::abs(dx);
        float overlapY = (playerBounds.size.y + platformBounds.size.y) / 2 - std::abs(dy);

        if (overlapX < overlapY)
        {
            // Horizontal collision
            if (dx > 0)
            {
                // Colliding from right
                character.SetPosition(platformBounds.position.x + platformBounds.size.x, playerBounds.position.y);
            }
            else
            {
                // Colliding from left
                character.SetPosition(platformBounds.position.x - playerBounds.size.x, playerBounds.position.y);
            }
            character.StopHorizontalMovement();
        }
        else
        {
            if (dy > 0)
            {
                // Hitting from below
                character.SetPosition(playerBounds.position.x, platformBounds.position.y + platformBounds.size.y);
                character.StopJumpingMovement();
            }
            else
            {
                // Landing on top
                character.SetPosition(playerBounds.position.x, platformBounds.position.y - playerBounds.size.y);
                character.StopFalling();
            }
        }
    }
}

void CollisionManager::HandleCharacterCollision(Character& a, Character& b)
{
    // Return out of function if the characters are within collision cooldown
    if (!a.GetCanCollide() || !b.GetCanCollide()) { return; }

    // Get the bounds of the characters
    sf::FloatRect aBounds = a.GetCollisionBounds();
    sf::FloatRect bBounds = b.GetCollisionBounds();

    // See if the intersection has a value
    if (aBounds.findIntersection(bBounds).has_value())
    {
        // Get velocity of each character
        sf::Vector2f aVel = a.GetVelocity();
        sf::Vector2f bVel = b.GetVelocity();

        // Bounce: reverse horizontal direction
        a.SetVelocity({ -aVel.x * BOUNCE_MULTIPLIER, aVel.y });
        b.SetVelocity({ -bVel.x * BOUNCE_MULTIPLIER, bVel.y });

        // Flip each sprite
        a.FlipSprite();
        b.FlipSprite();

        // Let Hunters reevaluate their targets
        if (auto* hunterA = dynamic_cast<Hunter*>(&a))
            hunterA->ResetTarget();

        if (auto* hunterB = dynamic_cast<Hunter*>(&b))
            hunterB->ResetTarget();

        // Let Bounders reevaluate their targets
        if (auto* bounderA = dynamic_cast<Bounder*>(&a))
            bounderA->ResetTarget();

        if (auto* bounderB = dynamic_cast<Bounder*>(&b))
            bounderB->ResetTarget();

        // Reset the cooldown for each character
        a.ResetCollisionCooldown();
        b.ResetCollisionCooldown();
    }
}
