#include "Game.h"

#include <random>

// --- Constructor ---
Game::Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry)
    : m_inputManager(inputManager), m_assetRegistry(assetRegistry), m_player1ScoreText(m_font),
    m_player2ScoreText(m_font), m_waveText(m_font)
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

    // --- Create the lava ---
    m_lava.setFillColor(sf::Color(255, 32, 0, 255));
    m_lava.setSize(sf::Vector2f(SCREEN_WIDTH, 50.f));
    m_lava.setPosition(sf::Vector2f(0.f, SCREEN_HEIGHT - m_lava.getGlobalBounds().size.y));

    // --- Create the lives icons ---
    int xPos = 195;
    int yPos = SCREEN_HEIGHT - 55;
    int iconWidth = 24;
    int padding = 2;

    // Loop through for player 1's icons
    for (int i = 0; i < 5; i++)
    {
        m_player1LifeIcons.push_back(std::make_unique<LivesIcon>(m_assetRegistry, xPos, yPos, "rider", 0));
        xPos = xPos + iconWidth + padding;
    }

    xPos = 350; // Update the x position for the player 2 icons

    // Loop through for player 2's icons
    for (int i = 0; i < 5; i++)
    {
        m_player2LifeIcons.push_back(std::make_unique<LivesIcon>(m_assetRegistry, xPos, yPos, "rider", 1));
        xPos = xPos + iconWidth + padding;
    }
     
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

    // --- Setup text --- //
    m_font = m_assetRegistry.GetFont(); // Get the font

    // Set the string and position for each text
    m_player1ScoreText.setString("Player 1: 0");
    m_player1ScoreText.setCharacterSize(18);
    m_player1ScoreText.setPosition(sf::Vector2f(10.f, 10.f));
    m_player1ScoreText.setFillColor(sf::Color::Yellow);
    m_player2ScoreText.setString("Player 2: 0");
    m_player2ScoreText.setCharacterSize(18);
    m_player2ScoreText.setFillColor(sf::Color::Cyan);
    m_player2ScoreText.setPosition(sf::Vector2f(SCREEN_WIDTH - m_player2ScoreText.getGlobalBounds().size.x - 10.f, 10.f));

    // Create the waypoints
    CreateWaypoints();

    // Start the wave manager
    m_waveManager.StartWave();

}

// --- Destructor ---
Game::~Game()
{

}

// --- Function for getting a random spawn location from the list of spawn positions ---
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

    // Get a random index
    int randomIndex = dist(gen);

    // Return a random position from the list of spawn positions from the random index
    return m_spawnPositions[randomIndex];
}

// --- Function for spawning enemies ---
void Game::SpawnEnemy(EnemyType type)
{
    // Get new random spawner location
    sf::Vector2f spawnPos = GetRandomSpawnLocation();

    // Switch statement for each enemy type
    switch (type)
    {
    case EnemyType::Bounder:
        SpawnCharacter<Bounder>(m_enemies, m_assetRegistry, m_waypointManager, spawnPos.x - 14.f, spawnPos.y, "enemy");
        break;

    case EnemyType::Hunter:
        SpawnCharacter<Hunter>(m_enemies, m_assetRegistry, m_waypointManager, spawnPos.x - 14.f, spawnPos.y, "enemy", m_players);
        break;
    }

}

// --- Function for spawning players ---
void Game::SpawnPlayer(PlayerType type)
{
    // Switch statement for each player
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

// --- Function for respawning players ---
void Game::RespawnPlayer(Player& player)
{
    // Set the players new location
    sf::Vector2f spawnPos = GetRandomSpawnLocation();
    player.SetPosition(spawnPos.x, spawnPos.y);

    // Set player to be alive
    player.SetIsAlive(true);
}

// --- Function for erasing players from the players vector ---
void Game::ErasePlayers()
{
    // Erase from players vector if pending removal
    std::erase_if(m_players, [this](const std::unique_ptr<Player>& player) {

        // Check if the player is in the list of pending removals
        return std::find_if(
            m_playersPendingRemoval.begin(),
            m_playersPendingRemoval.end(),
            [&](auto* ptr) { return ptr == &player; }) != m_playersPendingRemoval.end();
        });

    // Clear the players pending removal now they have been erased
    m_playersPendingRemoval.clear();
}

// --- Function for scheduling players for deletion ---
void Game::SchedulePlayersForDeletion()
{
    // Loop through all players
    for (auto& player : m_players)
    {
        // If the player is dead add them to pending removal vector
        if (player->GetLives() == 0)
        {
            m_playersPendingRemoval.push_back(&player);  // Store the pointer of the player to be removed
        }
    }
}

// --- Function for creating waypoints ---
void Game::CreateWaypoints()
{
    // Loop through the platforms
    for (const auto& platform : m_platforms)
    {
        sf::Vector2f pos = platform->GetPosition(); // Get the platforms positions
        float width = platform->GetSize().x; // Get the platforms width

        // Left edge
        m_waypointManager.AddWaypoint(std::make_unique<Waypoint>(pos.x, pos.y - 40.f));

        // Center
        m_waypointManager.AddWaypoint(std::make_unique<Waypoint>(pos.x + width / 2.f, pos.y - 40.f));

        // Right edge
        m_waypointManager.AddWaypoint(std::make_unique<Waypoint>(pos.x + width, pos.y - 40.f));
    }
}


// --- Main update function ---
void Game::Update(float deltaTime)
{
    // Update the wave manager
    UpdateWaveManager(deltaTime);

    // Remove all enemies that are not alive
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [this](const std::unique_ptr<Enemy>& e) {
                if (!e->GetIsAlive()) {
                    // Call EnemyDefeated() before the enemy is removed
                    m_waveManager.EnemyDefeated();
                    return true; // Mark the enemy for removal
                }
                return false;
            }),
        m_enemies.end());


    // Queue players for deletion (but don't erase yet)    
    SchedulePlayersForDeletion();

    // Create a vector for all current characters
    std::vector<Character*> characters; 

    // Loop through players and apply updates
    for (auto& player : m_players)
    {
        // Add to active characters list
        characters.push_back(player.get()); 

        // Update the UI for each player
        UpdateUI(*player);

        // Respawn players
        if (!player->GetIsAlive())
        {
            // Update the players respawn timer
            player->UpdateRespawnTimer(deltaTime);
            
            // If player can be respawned, then respawn them! :)
            if (player->GetCanRespawn())
            {
                RespawnPlayer(*player);
            }
        }
    }

    // Loop through and add all enemies to the characters list
    for (auto& enemy : m_enemies)
        characters.push_back(enemy.get());

    // Loop through all characters
    for (auto* character : characters)
    {
        // Check if the character is alive
        if (character->GetIsAlive())
        {
            // Update all characters
            character->Update(deltaTime);

            // Character-vs-platform collisions
            if (!character->GetIsSpawning())
            {
                // Loop through the platforms
                for (auto& platform : m_platforms)
                {
                    // Call the platforms collision
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

    // Remove all players that are not alive
    if (!m_playersPendingRemoval.empty())
    {
        ErasePlayers();
    }
}

// --- Function for updating inputs for the players ---
void Game::UpdateInputs()
{
    // Loop through all players
	for (auto& player : m_players)
	{
        // Call update input function on players
		player->UpdateInput();
	}
}

// --- Function for updating the UI ---
void Game::UpdateUI(Player& player)
{
    // Set the UI for each player
    if (player.GetPlayerID() == 1) // Player 1
    {
        // Set the score text
        sf::String score = std::to_string(player.GetScore());
        m_player1ScoreText.setString("Player 1: " + score);
        m_player1ScoreText.setPosition(sf::Vector2f(10.f, 10.f));

        // Set the life icons
        if (player.GetLives() < m_player1Lives && !m_player1LifeIcons.empty())
        {
            m_player1LifeIcons.pop_back(); // Remove the last element
            m_player1Lives = player.GetLives();
        }

    }

    else if (player.GetPlayerID() == 2) // Player 2
    {
        // --- Set the score text ---
        sf::String score = std::to_string(player.GetScore()); // Get the score and convert the int to a string
        m_player2ScoreText.setString("Player 2: " + score); // Set the string
        // Update the texts position relevant to its size
        m_player2ScoreText.setPosition(sf::Vector2f(SCREEN_WIDTH - m_player2ScoreText.getGlobalBounds().size.x - 10.f, 10.f));

        // --- Set the life icons ---
        if (player.GetLives() < m_player2Lives && !m_player2LifeIcons.empty())
        {
            m_player2LifeIcons.pop_back(); // Remove the last element
            m_player2Lives = player.GetLives();
        }
    }

    // Update UI for wave
    if (m_currentWave != m_waveManager.GetCurrentWave())
    {
        sf::String waveNumber = std::to_string(m_waveManager.GetCurrentWave()); // Get the current wave number and convert to string
        m_waveText.setString("Wave " + waveNumber); // Set the string
        // Update the texts position relevant to its size
        m_waveText.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - (m_waveText.getGlobalBounds().size.x / 2), 100.f));
    }
}

// --- Function for updating the wave manager ---
void Game::UpdateWaveManager(float deltaTime)
{
    // Update the wave manager
    m_waveManager.Update(deltaTime);

    // If the remaining enemies equals 0, then end the wave
    if (m_waveManager.GetRemainingEnemies() == 0)
    {
        m_waveManager.EndWave();
    }

    else
    {
        // Spawn enemy if allowed
        if (m_waveManager.GetCanSpawnEnemy())
        {
            SpawnEnemy(EnemyType::Bounder); // Spawn an enemy (just bounders for now)
            m_waveManager.EnemySpawned(); // Let the wave manager know an enemy has spawned
        }
    }
}

// --- Function for rendering all objects in the scene ---
void Game::Render(sf::RenderTarget& target)
{
    // --- Render the players ---
	for (auto& player : m_players)
	{
		player->Render(target);
	}


    // --- Render the enemies ---
    for (auto& enemy : m_enemies)
    {
        enemy->Render(target);
    }


    // --- Render the lava ---
    target.draw(m_lava);


    // --- Render the platforms ---
	for (auto& platform : m_platforms)
	{
		platform->Render(target);
	}


    // --- Render the text ---
    target.draw(m_player1ScoreText);
    target.draw(m_player2ScoreText);
    target.draw(m_waveText);


    // --- Render the life icons ---
    for (auto& lifeIcon : m_player1LifeIcons)
    {
        lifeIcon->Render(target);
    }
    for (auto& lifeIcon : m_player2LifeIcons)
    {
        lifeIcon->Render(target);
    }
}
