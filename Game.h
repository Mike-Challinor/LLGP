#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "player.h"
#include "platform.h"
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

	void HandlePlayerPlatformCollision(player& player, Platform& platform);

public:
	Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry);
	~Game();
	
	void Update();
	void UpdateInputs();
	void Render(sf::RenderTarget& target);
};

