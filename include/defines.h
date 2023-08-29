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

// whether print errors of filesystem to console
#define FS_ERROR_MESSAGES 1

// whether we should clear console when go into world
#define CLEAR_CONSOLE_ON_PLAY 0

// whether show amount of polygons to console
#define SHOW_POLYGON_RAM_COUNT 0

// scale stuff
#define SCALE_NORMAL (1 << 12)
#define SCALE(times) (SCALE_NORMAL * (times))
#define HALF_SCALE ((1 << 12) / 2)

// game data directories
#define DATA_DIR "fat:/dscraft_data"
#define WORLDS_DIR DATA_DIR "/worlds"
#define CONFIG_DIR DATA_DIR "/config"

// TODO move constants from this file to a config file in NitroFS so that it is possible to update these settings without recompiling 90% of the game
