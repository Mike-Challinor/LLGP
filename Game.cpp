#include "Game.h"

#include <random>


Game::Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry)
	: m_inputManager(inputManager), m_assetRegistry(assetRegistry), m_player1ScoreText(m_font), 
    m_player2ScoreText(m_font), m_player1LivesSprite(m_player1LivesSprite), m_player2LivesSprite(m_player2LivesSprite)
{
	// Load the sprite sheet
    m_assetRegistry.LoadSpriteSheet();

	// Create the platforms
	m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 0.f, 200.f, "top_left_platform"));
	m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 150.f, SCREEN_HEIGHT - 70.f, "bottom_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 0.f, 350.f, "bottom_left_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 250.f, 400.f, "bottom_middle_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, 200.f, 250.f, "top_middle_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, SCREEN_WIDTH - 300.f, 325.f, "middle_right_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, SCREEN_WIDTH - 84.f, 200.f, "top_right_platform"));
    m_platforms.push_back(std::make_unique<Platform>(m_assetRegistry, SCREEN_WIDTH - 85.f, 375.f, "bottom_right_platform"));

    // Create the lava
    m_lava.setFillColor(sf::Color(255, 32, 0, 255));
    m_lava.setSize(sf::Vector2f(SCREEN_WIDTH, 50.f));
    m_lava.setPosition(sf::Vector2f(0.f, SCREEN_HEIGHT - m_lava.getGlobalBounds().size.y));

    //  --- Create the sprites for lives --- //
    m_player1LivesSprite.setTexture(assetRegistry.LoadTexture());
    m_player1LivesSprite.setTextureRect(assetRegistry.LoadSprite("rider"));
    m_player1LivesSprite.setPosition(sf::Vector2f(160.f, 160.f));

    m_player2LivesSprite.setTexture(assetRegistry.LoadTexture());
    m_player2LivesSprite.setTextureRect(assetRegistry.LoadSprite("rider"));
    m_player2LivesSprite.setPosition(sf::Vector2f(160.f, 160.f));
     
    // Get the spawner locations
    for (auto& platform : m_platforms)
    {
        if (platform->GetHasSpawner())
        {
            m_spawnPositions.push_back(platform->GetSpawnPosition());
        }
    }

    // Spawn players
    SpawnPlayer(PlayerType::Player1);
    SpawnPlayer(PlayerType::Player2);

    // Create enemies
    SpawnEnemy(EnemyType::Bounder);
    SpawnEnemy(EnemyType::Hunter);

    // --- Setup text --- //
    m_font = m_assetRegistry.GetFont(); // Get the font

    // Add text to vector
    m_allText.push_back(m_player1ScoreText);
    m_allText.push_back(m_player2ScoreText);

    // Set the string and position for each text
    m_player1ScoreText.setString("Player 1: 0");
    m_player1ScoreText.setCharacterSize(18);
    m_player1ScoreText.setPosition(sf::Vector2f(10.f, 10.f));
    m_player1ScoreText.setFillColor(sf::Color::Yellow);
    m_player2ScoreText.setString("Player 2: 0");
    m_player2ScoreText.setCharacterSize(18);
    m_player2ScoreText.setFillColor(sf::Color::Yellow);
    m_player2ScoreText.setPosition(sf::Vector2f(SCREEN_WIDTH - m_player2ScoreText.getGlobalBounds().size.x - 10.f, 10.f));

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

void Game::SpawnEnemy(EnemyType type)
{
    // Get new random spawner location
    sf::Vector2f spawnPos = GetRandomSpawnLocation();

    switch (type)
    {
    case EnemyType::Bounder:
        SpawnCharacter<Bounder>(m_enemies, m_assetRegistry, spawnPos.x - 14.f, spawnPos.y, "enemy");
        break;

    case EnemyType::Hunter:
        SpawnCharacter<Hunter>(m_enemies, m_assetRegistry, spawnPos.x - 14.f, spawnPos.y, "enemy", m_players);
        break;
    }

}

void Game::SpawnPlayer(PlayerType type)
{
    switch (type)
    {
    case PlayerType::Player1:
        SpawnCharacter<Player>(m_players, m_inputManager, m_assetRegistry, 200.f, SCREEN_HEIGHT - 65.f, 1, "ostrich");
        break;

    case PlayerType::Player2:
        SpawnCharacter<Player>(m_players, m_inputManager, m_assetRegistry, 400.f, SCREEN_HEIGHT - 70.f, 2, "stork");
        break;
    }
}

void Game::SetText()
{

}

void Game::Update(float deltaTime)
{
    // Remove all enemies that are not alive
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const std::unique_ptr<Enemy>& e) {
                return !e->GetIsAlive();
            }),
        m_enemies.end());

    // Create a vector for all current characters
    std::vector<Character*> characters; 

    // Loop through and add all players to the characters list
    for (auto& player : m_players)
    {
        characters.push_back(player.get());

        if (player->GetPlayerID() == 1)
        {
            sf::String score = std::to_string(player->GetScore());
            m_player1ScoreText.setString("Player 1: " + score);
            m_player1ScoreText.setPosition(sf::Vector2f(10.f, 10.f));
        }

        else if (player->GetPlayerID() == 2)
        {
            sf::String score = std::to_string(player->GetScore());
            m_player2ScoreText.setString("Player 2: " + score);
            m_player2ScoreText.setPosition(sf::Vector2f(SCREEN_WIDTH - m_player2ScoreText.getGlobalBounds().size.x - 10.f, 10.f));
        }
            
    }
        
        

    // Loop through and add all enemies to the characters list
    for (auto& enemy : m_enemies)
        characters.push_back(enemy.get());

    // Loop through all characters
    for (auto* character : characters)
    {
        if (character->GetIsAlive())
        {
            // Update all characters
            character->Update(deltaTime);

            // Character-vs-platform collisions
            if (!character->GetIsSpawning())
            {
                for (auto& platform : m_platforms)
                {
                    CollisionManager::HandlePlatformCollision(*character, *platform);
                }
            }
        }
        
    }

    // Character-vs-character collisions:
    for (size_t i = 0; i < characters.size(); ++i)
    {
        for (size_t j = i + 1; j < characters.size(); ++j)
        {
            if (!characters[i]->GetIsSpawning() && !characters[j]->GetIsSpawning() && characters[i]->GetIsAlive() && characters[j]->GetIsAlive())
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

    target.draw(m_lava);

	for (auto& platform : m_platforms)
	{
		platform->Render(target);
	}

    target.draw(m_player1ScoreText);
    target.draw(m_player2ScoreText);

    target.draw(m_player1LivesSprite);
    target.draw(m_player2LivesSprite);
}
