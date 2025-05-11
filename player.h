#pragma once

#include "InputManager.h"
#include "Character.h"
#include "Timer.h"

class Player : public Character
{
private:
	// Bools
	bool m_canInput;
	bool m_canRespawn = false;

	// Managers
	LLGP::InputManager& m_inputManager; // Reference to an existing InputManager

	// Components
	Timer m_respawnTimer{ 0.1f }; // Time duration

	// Vectors
	std::vector<LLGP::Key> m_usableKeys;

	// Ints
	int m_playerID;
	int m_lives = 5;
	int m_score = 0;

	// Colours
	sf::Color m_playerColour;

	// Lists
	std::set<LLGP::Key> m_activeKeys;

	// Maps
	std::unordered_map<LLGP::Key, std::function<void()>> m_keyPressHandlers;
	std::unordered_map<LLGP::Key, std::function<void()>> m_keyReleaseHandlers;

	// Init functions
	void InitAnimations() override;

	// Movement functions
	void UpdateMovementDirection() override;

	// Mutation functions
	void SetUsableKeys(LLGP::InputManager& inputManager);

	// Functions
	void StartRespawnTimer();

	// Update functions
	void UpdateForceDecrement() override;

public:

	// Constructors and Destructors
	Player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos = 10.f, float yPos = 10.f, int player_id = 0, const std::string& objectName = "Character");
	virtual ~Player();

	// Listener functions
	void keyInputListener(LLGP::Key key);
	void OnKeyReleased(LLGP::Key key);

	// Accessor functions
	int GetScore() const;
	int GetLives() const;
	int GetPlayerID() const;
	int GetCanRespawn() const;

	// Mutator functions
	void Despawn() override;
	void AddScore(int pointsToAdd);
	void AddNewForce(sf::Vector2f forceToAdd) override;
	void SetIsAlive(bool isAlive);

	// Update functions
	void UpdateInput();
	void Update(float deltaTime) override;
	void UpdateRespawnTimer(float deltaTime);

	// Render functions
	void Render(sf::RenderTarget& target) override;

};

