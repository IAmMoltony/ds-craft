import os
import ycm_core

flags = [
    '-Wall', '-Wextra', '-O2', '-Os', '-march=armv5te', '-mthumb-interwork', '-mthumb', '-mtune=arm946e-s', '-fomit-frame-pointer', '-ffast-math', '-iquote include', '-isystem /opt/devkitpro/libnds/include/', '-isystem /opt/devkitpro/devkitARM/arm-none-eabi/include/', '-Weffc++', '-fno-rtti', '-fno-exceptions', '-DARM9'
]
