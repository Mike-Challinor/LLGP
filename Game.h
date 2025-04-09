#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "player.h"
#include "assetRegistry.h"

class Game
{
private:
	//Classes
	LLGP::InputManager& m_inputManager;
	LLGP::AssetRegistry& m_assetRegistry;

	// Players
	unique_ptr<player> m_playerClass1;
	unique_ptr<player> m_playerClass2;

	// Vectors
	std::vector<std::unique_ptr<player>> m_players;

public:
	Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry);
	~Game();
	
	void Update();
	void UpdateInputs();
	void Render(sf::RenderTarget& target);
};

