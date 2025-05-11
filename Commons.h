#pragma once
using namespace std;

const float PLAYER_MAX_HORIZONTAL_MOVEMENTSPEED = 3.5f;
const float PLAYER_MAX_VERTICAL_MOVEMENTSPEED = 3.5f;
const float ENEMY_MAX_WALKING_SPEED = 2.f;
const float ENEMY_MAX_FLYING_VERTICAL_SPEED = 3.f;
const float ENEMY_MAX_FLYING_HORIZONTAL_SPEED = 3.f;
const float GRAVITY = 1.5f;
const float ENEMY_WAIT_TIME = 0.25f;

enum FACING
{
	FACING_LEFT,
	FACING_RIGHT
};
