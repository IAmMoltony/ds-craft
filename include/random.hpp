/**
 * @file random.hpp
 * @brief Random number generation
*/

#pragma once

#include <nds.h>

/**
 * @brief Namespace for RNG module
*/
namespace rng
{
	/**
	 * @brief Set the seed of the random number generator
	 * @param seed the new seed
	*/
	void setSeed(u32 seed);

	/**
	 * @brief Generate a random number
	*/
	u32 generate(void);

	/**
	 * @brief Generate a random number in the specified range
	 * @param min minimum value
	 * @param max maximum value
	*/
	u32 range(u32 min, u32 max);

	/*
	 * @brief Generate a random *signed* number in the specified range
	 * @param min minimum value
	 * @param max maximum value
	*/
	s32 rangeSigned(s32 min, s32 max);

	/**
	 * @brief Calculate a probability
	 * @param chance the probability represented as percentage
	 */
	bool chance(u8 chance);
}