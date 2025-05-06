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
    enemy.Spawn();

}


void Game::Update(float deltaTime)
{
    std::vector<Character*> characters; // Create a vector for all current characters

    for (auto& player : m_players)
        characters.push_back(player.get());

    for (auto& enemy : m_enemies)
        characters.push_back(enemy.get());

    for (auto* character : characters)
    {
        character->Update(deltaTime);

        if (!character->GetIsSpawning())
        {
            for (auto& platform : m_platforms)
            {
                CollisionManager::HandlePlatformCollision(*character, *platform);
            }
        }
    }

    // Character-vs-character collisions (optional now, easy to add):
    for (size_t i = 0; i < characters.size(); ++i)
    {
        for (size_t j = i + 1; j < characters.size(); ++j)
        {
            if (!characters[i]->GetIsSpawning() && !characters[j]->GetIsSpawning())
            {
                CollisionManager::HandleCharacterCollision(*characters[i], *characters[j]);
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
