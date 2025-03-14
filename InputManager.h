#pragma once
#include "Event.h"
#include "Key.h"
#include <functional>
#include <unordered_map>

namespace LLGP
{
    class InputManager
    {
    private:
        std::unordered_map<Key, Event<>> keyPressEvents;

    public:
        InputManager() = default;
        ~InputManager() = default;

        void AddKeyListener(Key key, void* contextObj, const std::function<void()>& inFunc);

        void RemoveKeyListener(Key key, void* contextObj, const std::function<void()>& inFunc);

        void OnKeyPress(Key key);

        void ClearAllListeners();
    };
}


 
