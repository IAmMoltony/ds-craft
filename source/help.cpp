#include <help.hpp>

extern void drawMovingBackground(glImage dirt[1], u8 frames);
extern glImage sprDirt[1];
extern mm_sound_effect sndClick;

void showHelp(const std::string &file, Language lang, Font font, Font fontRu, Font fontSmall,
              Font fontSmallRu, u8 frames)
{
    const std::string filename = "nitro:/help/" + file + "_en.txt"; // TODO change _en according to lnguage
    if (!fsFileExists(filename.c_str()))
    {
        // oops
        printf("Error loading help file %s\nPress start to return", filename.c_str());
        while (true)
        {
            if (keysDown() & KEY_START)
                break;
            swiWaitForVBlank();
        }
        consoleClear();
        return;
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

        drawMovingBackground(sprDirt, frames);

        switch (lang)
        {
        case Language::English:
            font.printCentered(0, 5, "Help");
            glColor(RGB15(31, 31, 0));
            fontSmall.printCentered(0, 25, title.c_str());
            glColor(RGB15(31, 31, 31));
            fontSmall.print(10, 50, content.c_str());
            fontSmall.printCentered(0, SCREEN_HEIGHT - 19, "Press any button...");
            break;
        case Language::Russian:
            fontRu.printCentered(0, 5, "Qqoq~#");
            glColor(RGB15(31, 31, 0));
            fontSmallRu.printCentered(0, 25, title.c_str());
            glColor(RGB15(31, 31, 31));
            fontSmallRu.print(10, 50, content.c_str());
            fontSmall.printCentered(0, SCREEN_HEIGHT - 19, "Obiokug n%cv% mpqrmv...");
            break;
        }

        glEnd2D();
        glFlush(0);

        swiWaitForVBlank();
    }
    mmEffectEx(&sndClick);
}