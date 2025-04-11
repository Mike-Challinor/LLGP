#include "Game.h"



Game::Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry)
	: m_inputManager(inputManager), m_assetRegistry(assetRegistry)
{
	// Load the sprite sheet
	assetRegistry.LoadSpriteSheet();

	// Create the players
	m_players.push_back(std::make_unique<player>(inputManager, assetRegistry, 10.f, SCREEN_HEIGHT - 40.f, 1));
	m_players.push_back(std::make_unique<player>(inputManager, assetRegistry, 180.f, SCREEN_HEIGHT - 40.f, 2));	

	// Create the platforms
	m_platforms.push_back(std::make_unique<Platform>(assetRegistry, 500.f, 500.f));
	m_platforms.push_back(std::make_unique<Platform>(assetRegistry, 200.f, 500.f));
}

Game::~Game()
{
}

void Game::HandlePlayerPlatformCollision(player& player, Platform& platform)
{
    sf::FloatRect playerBounds = player.GetCollisionBounds();
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
                player.SetPosition(platformBounds.position.x + platformBounds.size.x, playerBounds.position.y);
            }
            else
            {
                // Colliding from left
                player.SetPosition(platformBounds.position.x - playerBounds.size.x, playerBounds.position.y);
            }
            player.StopHorizontalMovement();
        }
        else
        {
            if (dy > 0)
            {
                // Hitting from below
                player.SetPosition(playerBounds.position.x, platformBounds.position.y + platformBounds.size.y);
                player.StopJumpingMovement();
            }
            else
            {
                // Landing on top
                player.SetPosition(playerBounds.position.x, platformBounds.position.y - playerBounds.size.y);
                player.StopFalling();
            }
        }
    }
}


void Game::Update()
{
	for (auto& player : m_players)
	{
		player->Update();
	}

    // Then handle collisions between each player and all platforms
    for (auto& player : m_players)
    {
        for (auto& platform : m_platforms)
        {
            HandlePlayerPlatformCollision(*player, *platform);
        }
    }
}

void Game::UpdateInputs()
{
	for (auto& player : m_players)
	{
		player->UpdateInput();
	}
}

void Game::Render(sf::RenderTarget& target)
{
	for (auto& player : m_players)
	{
		player->Render(target);
	}

	for (auto& platform : m_platforms)
	{
		platform->Render(target);
	}
}
