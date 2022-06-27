#include <fs.hpp>

void fsInit(void)
{
    if (!nitroFSInit(NULL))
    {
        printf("Failed to initialize Nitro FS");
        while (true)
        {
        }
    }
}

std::string readFile(const char *path)
{
    std::ifstream file(path);
    if (file.bad())
    {
        return "";
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string buf(size, ' ');
    file.seekg(0);
    file.read(&buf[0], size);
    return buf;
}

bool fileExists(const char *path)
{
    if (FILE *file = fopen(path, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}