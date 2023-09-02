/**
 * @file help.hpp
 * @brief Help system
*/

#pragma once

#include <nds.h>
#include <string>
#include "lang.hpp"
#include "font.hpp"

/**
 * @brief Show help screen
 * @param file help file
 * @param font font
 * @param fontRu font (for russian language)
 * @param frames frame counter. Passed as reference because it is modified in the function.
*/
void showHelp(const std::string &file, Font font, Font fontRu, u16 &frames);
