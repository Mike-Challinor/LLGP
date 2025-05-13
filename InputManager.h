#pragma once
#include "Event.h"
#include "Key.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>

namespace LLGP
{
    class InputManager
    {
    private:
        // Maps
        std::unordered_map<Key, Event<>> keyPressEvents;
        std::unordered_map<Key, Event<>> keyReleaseEvents;

    public:
        // Functions
        void AddKeyPressListener(Key key, void* contextObj, const std::function<void()>& inFunc);
        void RemoveKeyPressListener(Key key, void* contextObj, const std::function<void()>& inFunc);
        void AddKeyReleasedListener(Key key, void* contextObj, const std::function<void()>& inFunc);
        void RemoveKeyReleasedListener(Key key, void* contextObj, const std::function<void()>& inFunc);
        void OnKeyPress(Key key);
        void OnKeyRelease(Key key);
        void ClearAllListeners();

        // Update functions
        void Update();
    };
}


 
