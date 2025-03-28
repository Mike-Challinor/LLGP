#include "InputManager.h"
#include <iostream>

void LLGP::InputManager::AddKeyPressListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyPressEvents[key].AddListener(contextObj, inFunc);
}

void LLGP::InputManager::RemoveKeyPressListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyPressEvents[key].RemoveListener(contextObj, inFunc);
}

void LLGP::InputManager::AddKeyReleasedListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyReleaseEvents[key].AddListener(contextObj, inFunc);
}

void LLGP::InputManager::RemoveKeyReleasedListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyReleaseEvents[key].RemoveListener(contextObj, inFunc);
}

void LLGP::InputManager::OnKeyPress(Key key)
{
	if (keyPressEvents.find(key) != keyPressEvents.end())
	{
		keyPressEvents[key]();
	}
}

void LLGP::InputManager::OnKeyRelease(Key key)
{
	if (keyReleaseEvents.find(key) != keyReleaseEvents.end())
	{
		keyReleaseEvents[key]();
	}
}

void LLGP::InputManager::ClearAllListeners()
{
	// Clear key press events
	for (auto& [key, event] : keyPressEvents)
	{
		event.Empty();
	}

	// Clear key press events
	for (auto& [key, event] : keyReleaseEvents)
	{
		event.Empty();
	}
}

void LLGP::InputManager::Update()
{
    // Track the state of keys to handle key releases
    static std::unordered_map<Key, bool> keyWasPressed;

    // Check for key presses
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        if (!keyWasPressed[Key::W]) // If it wasn't already pressed
        {
            OnKeyPress(Key::W);
            keyWasPressed[Key::W] = true; // Mark as pressed
        }
    }
    else if (keyWasPressed[Key::W]) // Key was pressed previously and is now released
    {
        OnKeyRelease(Key::W);
        keyWasPressed[Key::W] = false; // Mark as released
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        if (!keyWasPressed[Key::A])
        {
            OnKeyPress(Key::A);
            keyWasPressed[Key::A] = true;
        }
    }
    else if (keyWasPressed[Key::A])
    {
        OnKeyRelease(Key::A);
        keyWasPressed[Key::A] = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        if (!keyWasPressed[Key::D])
        {
            OnKeyPress(Key::D);
            keyWasPressed[Key::D] = true;
        }
    }
    else if (keyWasPressed[Key::D])
    {
        OnKeyRelease(Key::D);
        keyWasPressed[Key::D] = false;
    }

    // Player 2 inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        if (!keyWasPressed[Key::Up])
        {
            OnKeyPress(Key::Up);
            keyWasPressed[Key::Up] = true;
        }
    }
    else if (keyWasPressed[Key::Up])
    {
        OnKeyRelease(Key::Up);
        keyWasPressed[Key::Up] = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        if (!keyWasPressed[Key::Left])
        {
            OnKeyPress(Key::Left);
            keyWasPressed[Key::Left] = true;
        }
    }
    else if (keyWasPressed[Key::Left])
    {
        OnKeyRelease(Key::Left);
        keyWasPressed[Key::Left] = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        if (!keyWasPressed[Key::Right])
        {
            OnKeyPress(Key::Right);
            keyWasPressed[Key::Right] = true;
        }
    }
    else if (keyWasPressed[Key::Right])
    {
        OnKeyRelease(Key::Right);
        keyWasPressed[Key::Right] = false;
    }
}

