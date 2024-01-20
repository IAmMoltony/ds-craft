#include "entityai.hpp"
#include "random.hpp"
#include "limits.h"

void PigEntityAI::update(s16 &x, s16 &y, float &velX, float &velY, bool &falling, bool &jumping, Facing &facing, bool &moving, u8 &damageOverlayTimer, u16 &panicModeTimer, const Block::List &blocks, const Camera &camera, const u16 &frames, const Rect &rectTop, const Rect &rectBottom, const Rect &rectLeft, const Rect &rectRight, const mm_sound_effect *sfx1, const mm_sound_effect *sfx2, const mm_sound_effect *sfx3)
{
    // don't update if off screen
    if (x - camera.x < -20 ||
        x - camera.x > SCREEN_WIDTH ||
        y - camera.y < -40 ||
        y - camera.y > SCREEN_HEIGHT + 32)
        return;

    // damage overlay updating
    if (damageOverlayTimer != 255)
    {
        ++damageOverlayTimer;
        if (damageOverlayTimer == 20)
            damageOverlayTimer = 255;
    }

    // panic mode updating
    if (panicModeTimer != USHRT_MAX)
    {
        ++panicModeTimer;
        if (panicModeTimer == 300) // panic for 5 seconds
            panicModeTimer = USHRT_MAX;
    }

    // move
    x += velX;
    y += velY;

    // apply gravity if in air
    if (falling || jumping)
    {
        velY += 0.3f;
        if (velY > 5)
            velY = 5;
    }

    // clamp x
    if (x < 0)
        x = 0;
    if (x > 1000)
        x = 1000;

    s16 baseVelX = (panicModeTimer == USHRT_MAX) ? 1 : 2;

    // set x velocity if moving
    velX = moving ? (facing == Facing::Right ? baseVelX : -baseVelX) : 0;

    // randomly change direction
    if (rng::generate() % 250 == 1)
        facing = facing == Facing::Right ? Facing::Left : Facing::Right;

    // jump randomly
    if (rng::chance(11))
    {
        if (!jumping)
        {
            jumping = true;
            velY = -4;
        }
    }

    // stop or start moving randomly
    if (rng::chance(3))
        moving = !moving;

    // always move if panic
    if (panicModeTimer != USHRT_MAX)
        moving = true;

    // collision detection (every 4 frames for optimization)
    if (frames % 4 == 0)
    {
        for (auto &block : blocks)
        {
            // skip block if off screen
            if (block->getRect().x - camera.x < -40 ||
                block->getRect().y - camera.y < -40 ||
                block->y - camera.y > SCREEN_HEIGHT + 48)
                continue;

            // skip all blocks if too far to the right
            if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                break;

            // don't check collisions with non solid blocks
            if (!block->solid())
                continue;

            if (block->getRect().intersects(rectBottom))
            {
                falling = jumping = false;
                velY = 0;
                y = block->y - 24;
            }
            else
                falling = true;

            if (block->getRect().intersects(rectTop))
            {
                velY = 0;
                y = block->y + 17;
            }

            if (block->getRect().intersects(rectRight))
            {
                x = block->x - 22;
                if (!jumping)
                {
                    jumping = true;
                    velY = -4;
                }
            }

            if (block->getRect().intersects(rectLeft))
            {
                x = block->x + 16;
                if (!jumping)
                {
                    velY = -4;
                    jumping = true;
                }
            }
        }
    }

    // randomly do the oink oink (or whatever sound the caller passed)
    if (rng::chance(1) && rng::chance(51))
        playsfx(3, sfx1, sfx2, sfx3);
}
