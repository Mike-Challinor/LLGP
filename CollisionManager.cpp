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

        Enemy* enemy = dynamic_cast<Enemy*>(&character);
        bool isEnemy = enemy != nullptr;

        if (overlapX < overlapY)
        {
            // Horizontal collision
            if (dx > 0)
            {
                // Colliding from right
                character.SetPosition(platformBounds.position.x + platformBounds.size.x, playerBounds.position.y);

                if (!character.GetIsFacingRight())
                {
                    character.AddNewForce(sf::Vector2f(INITIAL_COLLISION_FORCE, 0.f)); // Force impulse pushing right

                    if (isEnemy)
                    {
                        character.FlipSprite();
                        enemy->ResetTarget();
                    }
                }
                    
            }
            else
            {
                // Colliding from left
                character.SetPosition(platformBounds.position.x - playerBounds.size.x, playerBounds.position.y);

                if (character.GetIsFacingRight())
                {
                    character.AddNewForce(sf::Vector2f(-INITIAL_COLLISION_FORCE, 0.f)); // Force impulse pushing left

                    if (isEnemy)
                    {
                        character.FlipSprite();
                        enemy->ResetTarget();
                    }
                        
                }
                
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

    // See if the intersection has a value and return if it doesnt
    if (!aBounds.findIntersection(bBounds).has_value()) return;

    // Determine the types of the characters
    bool aIsPlayer = dynamic_cast<Player*>(&a);
    bool bIsPlayer = dynamic_cast<Player*>(&b);
    bool aIsEnemy = dynamic_cast<Enemy*>(&a);
    bool bIsEnemy = dynamic_cast<Enemy*>(&b);

    // Enemy vs Enemy
    if (aIsEnemy && bIsEnemy)
    {
        // Do nothing — they pass through
        return;
    }

    // Player vs Enemy or Enemy vs Player
    if ((aIsPlayer && bIsEnemy) || (aIsEnemy && bIsPlayer))
    {
        Player* player = aIsPlayer ? dynamic_cast<Player*>(&a) : dynamic_cast<Player*>(&b);
        Enemy* enemy = aIsEnemy ? dynamic_cast<Enemy*>(&a) : dynamic_cast<Enemy*>(&b);

        if (enemy->GetHasRider())
        {
            // Apply jousting logic: compare vertical positions
            if (player->GetPosition().y < enemy->GetPosition().y) // If the player beats the enemy
            {
                // Remove the rider from the enemy
                enemy->RemoveRider();
                enemy->FlipSprite();
                enemy->ResetTarget();
                enemy->ResetCollisionCooldown();

                // Add points to the player
                int pointsToAdd = enemy->GetPointsValue(); 
                player->AddScore(pointsToAdd);
            }

            else if (player->GetPosition().y > enemy->GetPosition().y) // If the enemy beats the player
            {
                // Despawn the defeated player
                player->Despawn();

                // Reset the enemies target
                enemy->FlipSprite();
                enemy->ResetTarget();
                enemy->ResetCollisionCooldown();
            }

            else // If the enemy and player collide at the same height
            {
                // Remove the rider from the enemy
                enemy->RemoveRider();
                enemy->FlipSprite();
                enemy->ResetTarget();
                enemy->ResetCollisionCooldown();

                // Add points to the player
                int pointsToAdd = enemy->GetPointsValue();
                player->AddScore(pointsToAdd);

                // Despawn defeated player
                player->Despawn();
            }

        }
            

        else // If the enemy does not have a rider then despawn the enemy
        {
            // Despawn defeated enemy
            enemy->Despawn();
        }
            
        player->ResetCollisionCooldown();
        return;
    }

    // Player vs Player
    if (aIsPlayer && bIsPlayer)
    {
        Player* playerA = dynamic_cast<Player*>(&a);
        Player* playerB = dynamic_cast<Player*>(&b);

        // Decide a winner based on vertical position
        if (playerA->GetPosition().y < playerB->GetPosition().y) // If player A beats player B
        {
            // Add score to victorious player
            int pointsToAdd = playerB->GetPointsValue();
            playerA->AddScore(pointsToAdd);

            // Despawn defeated player
            playerB->Despawn();
        }
        else if (playerA->GetPosition().y > playerB->GetPosition().y) // If player B beats player A
        {
            // Add score to victorious player
            int pointsToAdd = playerA->GetPointsValue();
            playerB->AddScore(pointsToAdd);

            // Despawn defeated player
            playerA->Despawn();

        }

        else // If players collide at same height
        {
            // Add score to both players
            int pointsToAdd = playerA->GetPointsValue();
            playerA->AddScore(pointsToAdd);
            playerB->AddScore(pointsToAdd);

            // Despawn both defeated players
            playerA->Despawn();
            playerB->Despawn();
        }

        playerA->ResetCollisionCooldown();
        playerB->ResetCollisionCooldown();

        return;
    }
}
