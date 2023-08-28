#pragma once

#include <nds.h>
#include <string>
#include "lang.hpp"
#include "font.hpp"

/**
 * @brief Show help screen
 * @param file help file
 * @param lang current language
 * @param font font
 * @param fontRu font (for russian language)
 * @param frames frame counter
*/
void showHelp(const std::string &file, Language lang, Font font, Font fontRu, u16 &frames);

// TODO lang can be ommitted entirely because it is available from Game::instance
// TODO same with frames
