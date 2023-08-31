/**
 * @file defines.h
 * @brief Global macros and constants
 * @note This file is goingto go away soon because changing a constant and causing the entire game to recompile is not good.
*/

#pragma once

#define declsfx4(effectname)            \
    mm_sound_effect snd##effectname##1; \
    mm_sound_effect snd##effectname##2; \
    mm_sound_effect snd##effectname##3; \
    mm_sound_effect snd##effectname##4;
#define declsfx3(effectname)            \
    mm_sound_effect snd##effectname##1; \
    mm_sound_effect snd##effectname##2; \
    mm_sound_effect snd##effectname##3;
#define loadsfx4(effectname)           \
    mmLoadEffect(SFX_##effectname##1); \
    mmLoadEffect(SFX_##effectname##2); \
    mmLoadEffect(SFX_##effectname##3); \
    mmLoadEffect(SFX_##effectname##4);
#define loadsfx3(effectname)           \
    mmLoadEffect(SFX_##effectname##1); \
    mmLoadEffect(SFX_##effectname##2); \
    mmLoadEffect(SFX_##effectname##3);
#define unloadsfx4(effectname)           \
    mmUnloadEffect(SFX_##effectname##1); \
    mmUnloadEffect(SFX_##effectname##2); \
    mmUnloadEffect(SFX_##effectname##3); \
    mmUnloadEffect(SFX_##effectname##4);
#define unloadsfx3(effectname)           \
    mmUnloadEffect(SFX_##effectname##1); \
    mmUnloadEffect(SFX_##effectname##2); \
    mmUnloadEffect(SFX_##effectname##3);
#define setsfx4(effectname, effectid)                    \
    snd##effectname##1 = soundEffect(SFX_##effectid##1); \
    snd##effectname##2 = soundEffect(SFX_##effectid##2); \
    snd##effectname##3 = soundEffect(SFX_##effectid##3); \
    snd##effectname##4 = soundEffect(SFX_##effectid##4);
#define setsfx3(effectname, effectid)                    \
    snd##effectname##1 = soundEffect(SFX_##effectid##1); \
    snd##effectname##2 = soundEffect(SFX_##effectid##2); \
    snd##effectname##3 = soundEffect(SFX_##effectid##3);

// scale stuff
#define SCALE_NORMAL (1 << 12)
#define SCALE(times) (SCALE_NORMAL * (times))
#define HALF_SCALE ((1 << 12) / 2)
