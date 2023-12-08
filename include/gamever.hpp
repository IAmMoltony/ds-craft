/**
 * @file gamever.hpp
 * @brief Game version management
 */

#pragma once

#include <stdint.h>
#include <string>

/**
 * @brief Namespace for the game verson module
 */
namespace gamever
{
	/**
	 * @brief Game version initialization status
	 */
	enum class InitStatus
	{
		/**
		 * @brief OK - game version initialized successfully
		 */
		OK,

		/**
		 * @brief File open error - game version file failed to open.
		 *
		 * The reason why is typically stored in errno, so you can retreive it using strerror() or perror()
		 */
		FileOpenError,
	};

	/**
	 * @brief Initialize game version
	 * @param gameVerName path to game version file version
	 *
	 * @return `InitStatus::OK` if init was successful, or `InitStatus::FileOpenError` if there was an error.
	 */
	InitStatus init(const char *gameVerName);

	/**
	 * @brief Get the version string
	 */
	const char *getVersionString(void);

	/**
	 * @brief Get the hash of the specified version
	 * @param versionString The version string to get the hash of
	 *
	 * This is useful for checking if a certain verson is newer or older than another version.
	 *
	 * Please make sure your program can deal with 64bit numbers (even if it does require external libraries) before using this.
	 */
	uint64_t getVersionHash(const std::string &versionString);
}