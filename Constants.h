#pragma once

//screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const float JUMP_FORCE_DECREMENT = 0.5f;
const float COLLISION_FORCE_DECREMENT = 1.5f;
const float INITIAL_JUMP_FORCE = 10.f;
const float INITIAL_COLLISION_FORCE = 7.5f;
const float COLLISION_MAX_FORCE = 10.f;

const int POINTS_VALUE_BOUNDER = 500;
const int POINTS_VALUE_HUNTER = 750;
const int POINTS_VALUE_PLAYER = 1000;