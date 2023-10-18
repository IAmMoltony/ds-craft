/**
 * @file random.hpp
 * @brief Random number generation
*/

#pragma once

#include <nds.h>

/**
 * @brief Set the seed of the random number generator
 * @param seed the new seed
*/
void randomSetSeed(u32 seed);

/**
 * @brief Generate a random number
*/
u32 randomGenerate(void);

/**
 * @brief Generate a random number in the specified range
 * @param min minimum value
 * @param max maximum value
*/
u32 randomRange(u32 min, u32 max);

/*
 * @brief Generate a random *signed* number in the specified range
 * @param min minimum value
 * @param max maximum value
*/
s32 randomRangeSigned(s32 min, s32 max);

/**
 * @brief Calculate a probability
 * @param chance the probability represented as percentage
*/
bool randomChance(u8 chance);
