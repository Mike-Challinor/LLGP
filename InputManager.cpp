#include "InputManager.h"
#include <iostream>

void LLGP::InputManager::AddKeyListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyPressEvents[key].AddListener(contextObj, inFunc);
}

void LLGP::InputManager::RemoveKeyListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyPressEvents[key].RemoveListener(contextObj, inFunc);
}

void LLGP::InputManager::OnKeyPress(Key key)
{
	if (keyPressEvents.find(key) != keyPressEvents.end())
	{
		keyPressEvents[key]();
	}
}

void LLGP::InputManager::ClearAllListeners()
{
	for (auto& [key, event] : keyPressEvents)
	{
		event.Empty();
	}
}

void LLGP::InputManager::Update()
{
	// Player 1 inputs
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) OnKeyPress(Key::W);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) OnKeyPress(Key::A);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) OnKeyPress(Key::D);

	// Player 2 inputs
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) OnKeyPress(Key::Up);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) OnKeyPress(Key::Left);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) OnKeyPress(Key::Right);
}
