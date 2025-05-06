#pragma once

#include "Character.h"
#include "Platform.h"
#include "Hunter.h"
#include "Bounder.h"

class CollisionManager
{
public:
    static void HandlePlatformCollision(Character& character, Platform& platform);
    static void HandleCharacterCollision(Character& a, Character& b);
};

