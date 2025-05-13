#include "InputManager.h"
#include <iostream>

// --- Function for adding a key press listener ---
void LLGP::InputManager::AddKeyPressListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyPressEvents[key].AddListener(contextObj, inFunc);
}

// --- Function for removing a key press listener ---
void LLGP::InputManager::RemoveKeyPressListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyPressEvents[key].RemoveListener(contextObj, inFunc);
}

// --- Function for adding a key released listener ---
void LLGP::InputManager::AddKeyReleasedListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyReleaseEvents[key].AddListener(contextObj, inFunc);
}

// --- Function for removing a key released listener ---
void LLGP::InputManager::RemoveKeyReleasedListener(Key key, void* contextObj, const std::function<void()>& inFunc)
{
	keyReleaseEvents[key].RemoveListener(contextObj, inFunc);
}

// --- Function for finding whether a key has been pressed ---
void LLGP::InputManager::OnKeyPress(Key key)
{
	if (keyPressEvents.find(key) != keyPressEvents.end())
	{
		keyPressEvents[key](); // If key has been pressed, add to events list
	}
}

// --- Function for finding whether a key has been released ---
void LLGP::InputManager::OnKeyRelease(Key key)
{
	if (keyReleaseEvents.find(key) != keyReleaseEvents.end())
	{
		keyReleaseEvents[key](); // If key has been released, add to events list
	}
}

// --- Function for clearing all listeners from all key events ---
void LLGP::InputManager::ClearAllListeners()
{
	// Clear key press events
	for (auto& [key, event] : keyPressEvents)
	{
		event.Empty();
	}

	// Clear key release events
	for (auto& [key, event] : keyReleaseEvents)
	{
		event.Empty();
	}
}

// --- Main update function ---
void LLGP::InputManager::Update()
{
    // Track the state of keys to handle key releases
    static std::unordered_map<Key, bool> keyWasPressed;

    // Check for key presses
    // 
    // --- Player 1 inputs ---
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
        if (!keyWasPressed[Key::A]) // If it wasn't already pressed
        {
            OnKeyPress(Key::A);
            keyWasPressed[Key::A] = true; // Mark as pressed
        }
    }
    else if (keyWasPressed[Key::A]) // Key was pressed previously and is now released
    {
        OnKeyRelease(Key::A);
        keyWasPressed[Key::A] = false; // Mark as released
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        if (!keyWasPressed[Key::D]) // If it wasn't already pressed
        {
            OnKeyPress(Key::D);
            keyWasPressed[Key::D] = true; // Mark as pressed
        } 
    }
    else if (keyWasPressed[Key::D]) // Key was pressed previously and is now released
    {
        OnKeyRelease(Key::D);
        keyWasPressed[Key::D] = false; // Mark as released
    } 

    // Player 2 inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        if (!keyWasPressed[Key::Up]) // If it wasn't already pressed
        {
            OnKeyPress(Key::Up);
            keyWasPressed[Key::Up] = true; // Mark as pressed
        }
    } 
    else if (keyWasPressed[Key::Up]) // Key was pressed previously and is now released
    {
        OnKeyRelease(Key::Up);
        keyWasPressed[Key::Up] = false; // Mark as released
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        if (!keyWasPressed[Key::Left]) // If it wasn't already pressed
        {
            OnKeyPress(Key::Left);
            keyWasPressed[Key::Left] = true; // Mark as pressed
        }
    }
    else if (keyWasPressed[Key::Left]) // Key was pressed previously and is now released
    {
        OnKeyRelease(Key::Left);
        keyWasPressed[Key::Left] = false; // Mark as released
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        if (!keyWasPressed[Key::Right]) // If it wasn't already pressed
        {
            OnKeyPress(Key::Right);
            keyWasPressed[Key::Right] = true; // Mark as pressed
        }
    }
    else if (keyWasPressed[Key::Right]) // Key was pressed previously and is now released
    {
        OnKeyRelease(Key::Right);
        keyWasPressed[Key::Right] = false; // Mark as released
    }
}

