#pragma once

//screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 16
#define MAP_HEIGHT 13
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

const float JUMP_FORCE_DECREMENT = 0.01f;
const float COLLISION_FORCE_DECREMENT = 0.01f;
const float INITIAL_JUMP_FORCE = 1.3f;
const float INITIAL_COLLISION_FORCE = 1.15f;
const float COLLISION_MAX_FORCE = 1.5f;

const int POINTS_VALUE_BOUNDER = 500.f;
const int POINTS_VALUE_HUNTER = 750.f;
const int POINTS_VALUE_PLAYER = 1000.f;