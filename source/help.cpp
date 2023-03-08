#include "help.hpp"
#include "game.hpp"

void showHelp(const std::string &file, Language lang, Font font, Font fontRu, u16 &frames)
{
    const std::string filename = "nitro:/help/" + file + "_" +
                                 ((lang == Language::English) ? "en" : "ru") + ".txt";
    if (!fsFileExists(filename.c_str()))
    {
        // oops
        while (true)
        {
            scanKeys();
            if (keysDown() != 0 && !(keysDown() & KEY_TOUCH))
                return;

            ++frames;

            glBegin2D();
            Game::instance->drawMovingBackground();

            switch (lang)
            {
            case Language::English:
                font.setCharWidthHandler(Game::fontBigCharWidthHandler);
                font.printfScaleCentered(0, 5, SCALE(1.8), "oops");
                font.setCharWidthHandler(Game::fontSmallCharWidthHandler);
                font.print(10, 40, "This help entry could not be found.");
                font.print(10, 51, "Please check that the file exists:");
                font.printCentered(0, SCREEN_HEIGHT - 19, "Press any button...");
                break;
            case Language::Russian:
                fontRu.setCharWidthHandler(Game::fontBigRuCharWidthHandler);
                fontRu.printfScaleCentered(0, 5, SCALE(1.8), "ql");
                fontRu.setCharWidthHandler(Game::fontSmallRuCharWidthHandler);
                fontRu.print(10, 40, "_ub jbrkt# trsbdmk pg oqigu c\"u# pblfgpb.");
                fontRu.print(10, 51, "Ucgfkugt#@ zuq $uqu wbln tv~gtudvgu:");
                fontRu.printCentered(0, SCREEN_HEIGHT - 19, "Obiokug n%cv% mpqrmv...");
                break;
            }
            font.print(10, 62, filename.c_str());

            glEnd2D();
            glFlush(0);

            swiWaitForVBlank();
        }
    }

    std::string fileData = std::string(fsReadFile(filename.c_str()));

    // title and content initially hold placeholder values
    std::string title = "title";
    std::string content = "content";

    std::stringstream ss(fileData);
    std::getline(ss, title);

    fileData.erase(0, fileData.find("\n") + 1);
    content = fileData;

    while (true)
    {
        scanKeys();
        if (keysDown() != 0 && !(keysDown() & KEY_TOUCH))
            break;

        ++frames;

        glBegin2D();
        Game::instance->drawMovingBackground();

        switch (lang)
        {
        case Language::English:
            font.setCharWidthHandler(Game::fontBigCharWidthHandler);
            font.printfScaleCentered(0, 5, SCALE(1.8), "Help");
            font.setCharWidthHandler(Game::fontSmallCharWidthHandler);
            glColor(RGB15(31, 31, 0));
            font.printCentered(0, 25, title.c_str());
            glColor(RGB15(31, 31, 31));
            font.print(10, 50, content.c_str());
            font.printCentered(0, SCREEN_HEIGHT - 19, "Press any button...");
            break;
        case Language::Russian:
            fontRu.setCharWidthHandler(Game::fontBigRuCharWidthHandler);
            fontRu.printfScaleCentered(0, 5, SCALE(1.8), "Qqoq~#");
            fontRu.setCharWidthHandler(Game::fontSmallRuCharWidthHandler);
            glColor(RGB15(31, 31, 0));
            fontRu.printCentered(0, 25, title.c_str());
            glColor(RGB15(31, 31, 31));
            fontRu.print(10, 50, content.c_str());
            fontRu.printCentered(0, SCREEN_HEIGHT - 19, "Obiokug n%cv% mpqrmv...");
            break;
        }

        glEnd2D();
        glFlush(0);

        swiWaitForVBlank();
    }
    mmEffectEx(&Game::instance->sndClick);
}
