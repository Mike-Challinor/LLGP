#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "player.h"
#include "platform.h"
#include "Enemy.h"
#include "Bounder.h"
#include "Hunter.h"
#include "assetRegistry.h"

class Game
{
private:
	//Classes
	LLGP::InputManager& m_inputManager;
	LLGP::AssetRegistry& m_assetRegistry;

	// Vectors
	std::vector<std::unique_ptr<player>> m_players;
	std::vector<std::unique_ptr<Platform>> m_platforms;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
	std::vector<sf::Vector2f> m_spawnPositions;

	void HandlePlayerPlatformCollision(Character& character, Platform& platform);
	sf::Vector2f GetRandomSpawnLocation();
	void SpawnEnemy();

public:
	Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry);
	~Game();
	
	void Update(float deltaTime);
	void UpdateInputs();
	void Render(sf::RenderTarget& target);
};

