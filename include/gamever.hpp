/**
 * @file gamever.hpp
 * @brief Game version management
*/

#pragma once

#include <nds.h>
#include <string>

/**
 * @brief Namespace for the game verson module
 */
namespace gamever
{
	/**
	 * @brief Initialize game version
	 */
	void init(void);
	
	/**
	 * @brief Get the version string
	 */
	const char *getVersionString(void);
	
	/**
	 * @brief Get the hash of the specified version
	 * @param versionString The version string to get the hash of
	 */
	u64 getVersionHash(const std::string &versionString);
}