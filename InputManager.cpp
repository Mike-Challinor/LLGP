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
