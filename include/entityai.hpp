/**
 * @file entityai.hpp
 * @brief Classes for managing entity AI
 *
 * this is in its own file so that different entity ai can be reused
*/

#pragma once

#include "block.hpp"
#include "facing.hpp"
#include "camera.h"

/**
 * @brief AI for pigs.
 * 
 * It can actually be used in more than just pigs because of how generic and dumb animal-like it is. It just makes the entity wander around aimlessly, sometimes jumping.
*/
class PigEntityAI
{
public:
	static void update(s16 &x, s16 &y, float &velX, float &velY, bool &falling, bool &jumping, Facing &facing, bool &moving, u8 &damageOverlayTimer, u16 &panicModeTimer, const Block::List &blocks, const Camera &camera, const u16 &frames, const Rect &rectTop, const Rect &rectBottom, const Rect &rectLeft, const Rect &rectRight, const mm_sound_effect *sfx1, const mm_sound_effect *sfx2, const mm_sound_effect *sfx3);
};