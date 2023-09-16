/**
 * @file log.h
 * @brief Logging library
*/

#pragma once

/**
 * @brief Enum representing logging level
*/
typedef enum
{
    LOG_INFO = 0, /**< Information: Status or progress of the program */
    LOG_WARNING = 1, /**< Warning: Something is a bit off, but we can still continue */
    LOG_ERROR = 2, /**< Error: We are in trouble! */
} LogLevel;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize logging
 * @param level logging level
 * @param logFileName name of the log file
*/
void logInit(const LogLevel level, const char *logFileName);

/**
 * @brief Set logging level
 * @param level level
*/
void logSetLevel(const LogLevel level);

/**
 * @brief Print a message to the log
 * @param level level of the message
 * @param format formatting string
*/
void logMessage(const LogLevel level, const char *format, ...);

#ifdef __cplusplus
}
#endif
