#pragma once

#include "InputManager.h"
#include "Character.h"

class player : public Character
{
private:
	
	// Managers
	LLGP::InputManager& m_inputManager; // Reference to an existing InputManager

	// Vectors
	std::vector<LLGP::Key> m_usableKeys;

	// Ints
	int m_playerID;

	// Lists
	std::set<LLGP::Key> m_activeKeys;

	// Init functions
	void InitAnimations() override;

	// Movement functions
	void UpdateMovementDirection() override;

	// Mutation functions
	void SetUsableKeys(LLGP::InputManager& inputManager);

public:

	// Constructors and Destructors
	player(LLGP::InputManager& inputManager, LLGP::AssetRegistry& assetRegistry, float xPos = 10.f, float yPos = 10.f, int player_id = 0, const std::string& objectName = "Character");
	virtual ~player();

	// Listener functions
	void keyInputListener(LLGP::Key key);
	void OnKeyReleased(LLGP::Key key);

	// Update functions
	void UpdateInput();
	void Update(float deltaTime) override;

	// Render functions
	void Render(sf::RenderTarget& target) override;

};

