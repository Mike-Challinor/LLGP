#include "Game.h"

#include <random>


Game::Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry)
	: m_inputManager(inputManager), m_assetRegistry(assetRegistry)
{
	// Load the sprite sheet
    m_assetRegistry.LoadSpriteSheet();

	// Create the platforms
	m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 0.f, 200.f, "top_left_platform"));
	m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 150.f, SCREEN_HEIGHT - 70.f, "bottom_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 0.f, 350.f, "bottom_left_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 250.f, 400.f, "bottom_middle_platform"));

    // Create the players
    m_players.push_back(std::make_unique<player>(inputManager, m_assetRegistry, 200.f, SCREEN_HEIGHT - 140.f, 1, "ostrich"));
    m_players.push_back(std::make_unique<player>(inputManager, m_assetRegistry, 400.f, SCREEN_HEIGHT - 140.f, 2, "stork"));

    // Get the spawner locations
    for (auto& platform : m_platforms)
    {
        if (platform->GetHasSpawner())
        {
            m_spawnPositions.push_back(platform->GetSpawnPosition());
        }
    }

    // Create enemies
    SpawnEnemy();

}

Game::~Game()
{
}

void Game::HandlePlayerPlatformCollision(Character& character, Platform& platform)
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

sf::Vector2f Game::GetRandomSpawnLocation()
{
    // Set position to 0,0 if there are no spawn locations
    if (m_spawnPositions.empty())
    {
        return sf::Vector2f(0.f, 0.f);
    }
        
    // Random engine 
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(m_spawnPositions.size()) - 1);

    int randomIndex = dist(gen);
    return m_spawnPositions[randomIndex];
}

void Game::SpawnEnemy()
{
    // Get new random spawner location
    sf::Vector2f spawnPos = GetRandomSpawnLocation();

    // Create the enemy
    m_enemies.push_back(std::make_unique<Hunter>(m_assetRegistry, spawnPos.x - 14.f, spawnPos.y, "enemy", m_players));

    // Get a reference to the enemy
    Enemy& enemy = *m_enemies.back();

    enemy.Spawn();    

    m_enemies.push_back(std::make_unique<Bounder>(m_assetRegistry, spawnPos.x - 14.f, spawnPos.y, "enemy"));
    Enemy& enemy2 = *m_enemies.back();
    enemy2.Spawn();

}


void Game::Update(float deltaTime)
{
	for (auto& player : m_players)
	{
		player->Update(deltaTime);
	}

    for (auto& vulture : m_enemies)
    {
        vulture->Update(deltaTime);
    }

    // Handle collisions between each player and all platforms
    for (auto& player : m_players)
    {
        for (auto& platform : m_platforms)
        {
            HandlePlayerPlatformCollision(*player, *platform);
        }
    }

    // Handle collisions between each vulture and all platforms
    for (auto& enemy : m_enemies)
    {
        if (!enemy->GetIsSpawning()) // Only handle collisions if the vulture is not spawning
        {
            for (auto& platform : m_platforms)
            {
                HandlePlayerPlatformCollision(*enemy, *platform);
            }
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

    for (auto& enemy : m_enemies)
    {
        enemy->Render(target);
    }

	for (auto& platform : m_platforms)
	{
		platform->Render(target);
	}
}
