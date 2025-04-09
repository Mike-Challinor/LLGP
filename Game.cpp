#include "Game.h"

Game::Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry)
	: m_inputManager(inputManager), m_assetRegistry(assetRegistry)
{
	// Load the sprite sheet
	assetRegistry.LoadSpriteSheet();

	// Create the players
	m_players.push_back(std::make_unique<player>(inputManager, assetRegistry, 10.f, SCREEN_HEIGHT - 40.f, 1));
	m_players.push_back(std::make_unique<player>(inputManager, assetRegistry, 180.f, SCREEN_HEIGHT - 40.f, 2));	
}

Game::~Game()
{
}

void Game::Update()
{
	for (auto& player : m_players)
	{
		player->Update();
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
}
