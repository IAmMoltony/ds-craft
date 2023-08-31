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
    LOG_INFO = 0,
    LOG_WARNING = 1,
    LOG_ERROR = 2,
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
