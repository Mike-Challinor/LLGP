#pragma once

#include "InputManager.h"
#include "Character.h"

class Player : public Character
{
private:
	// Bools
	bool m_canInput;

	// Managers
	LLGP::InputManager& m_inputManager; // Reference to an existing InputManager

	// Vectors
	std::vector<LLGP::Key> m_usableKeys;

	// Ints
	int m_playerID;
	int m_lives = 5;
	int m_score = 0;

	// Lists
	std::set<LLGP::Key> m_activeKeys;

	// Init functions
	void InitAnimations() override;

	// Movement functions
	void UpdateMovementDirection() override;

	// Mutation functions
	void SetUsableKeys(LLGP::InputManager& inputManager);

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
	int GetPlayerID() const;

	// Mutator functions
	void Despawn() override;
	void AddScore(int pointsToAdd);
	void AddNewForce(sf::Vector2f forceToAdd) override;

	// Update functions
	void UpdateInput();
	void Update(float deltaTime) override;

	// Render functions
	void Render(sf::RenderTarget& target) override;

};

