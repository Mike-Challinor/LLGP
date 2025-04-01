#include "Game.h"

Game::Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry)
	: m_inputManager(inputManager), m_assetRegistry(assetRegistry)
{
	// Load the sprite sheet
	assetRegistry.LoadSpriteSheet();

	// Create the players
	m_players.push_back(std::make_unique<player>(inputManager, assetRegistry, 10.f, 10.f, 1));
	m_players.push_back(std::make_unique<player>(inputManager, assetRegistry, 180.f, 10.f, 2));

}

Game::~Game()
{
}

void Game::Update()
{
	for (auto& player : m_players)
	{
		player->update();
	}
}

void Game::UpdateInputs()
{
	for (auto& player : m_players)
	{
		player->updateInput();
	}
}

void Game::Render(sf::RenderTarget& target)
{
	for (auto& player : m_players)
	{
		player->render(target);
	}
}
