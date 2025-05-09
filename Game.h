#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <type_traits>

#include "Player.h"
#include "Platform.h"
#include "Enemy.h"
#include "Bounder.h"
#include "Hunter.h"
#include "AssetRegistry.h"
#include "CollisionManager.h"

enum class EnemyType
{
	Player,
	Bounder,
	Hunter
};

enum class PlayerType
{
	Player1,
	Player2
};

class Game
{
private:
	// Classes
	LLGP::InputManager& m_inputManager;
	LLGP::AssetRegistry& m_assetRegistry;

	// Vectors
	std::vector<std::unique_ptr<Player>> m_players;
	std::vector<std::unique_ptr<Platform>> m_platforms;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
	std::vector<sf::Vector2f> m_spawnPositions;
	std::vector<sf::Text> m_allText;
	std::vector<sf::Sprite> m_player1LivesSprites;
	std::vector<sf::Sprite> m_player2LivesSprites;

	// Functions
	sf::Vector2f GetRandomSpawnLocation();
	void SpawnEnemy(EnemyType type);
	void SpawnPlayer(PlayerType type);
	void SetText();

	// Text
	sf::Font m_font;
	sf::Text m_player1ScoreText;
	sf::Text m_player2ScoreText;

	// Shapes
	sf::RectangleShape m_lava;
	sf::Sprite m_player1LivesSprite;
	sf::Sprite m_player2LivesSprite;

public:
	Game(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry);
	~Game();

	void Update(float deltaTime);
	void UpdateInputs();
	void Render(sf::RenderTarget& target);

	// Template function to spawn a character into a container
	template<typename CharacterType, typename Container, typename... Args>
	void SpawnCharacter(Container& container, Args&&... args)
	{
		static_assert(std::is_base_of<Character, CharacterType>::value, "CharacterType must derive from Character");

		auto character = std::make_unique<CharacterType>(std::forward<Args>(args)...);
		character->Spawn();
		container.push_back(std::move(character));
	}
};
