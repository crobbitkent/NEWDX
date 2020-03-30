
#pragma once

#include "GameEngine.h"
#include <stack>


enum PLAYER_STATE
{
	PS_IDLE,

	PS_BWALK,
	PS_WALK,
	PS_AWALK,

	PS_TURN,

	PS_ATTACK,

	PS_JUMP,
	PS_FALL,
	PS_FALLING,
	PS_LAND,

	PS_BHEAL,
	PS_HEAL,
	PS_AHEAL,

	PS_FIRE,

	PS_DAMAGED,

	PS_END,
};



enum DIR_TYPE
{
	DIR_UP = -3,
	DIR_BOT = -2,
	DIR_LEFT = -1,
	DIR_RIGHT = 1,
	DIR_END = 0,
};


enum BUG_STATE
{
	BS_WALK,
	BS_TURN,
	BS_DIE,
	BS_DEAD,

	BS_DASH,
	BS_BWALK,
};
