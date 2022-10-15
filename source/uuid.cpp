#include "uuid.hpp"

std::string uuidGenerate(void)
{
    std::random_device randomDev;
    std::seed_seq seed{time(NULL)};
    std::mt19937 mt(seed);
    std::uniform_int_distribution<> unifIntDist(0, 15);
    std::uniform_int_distribution<> unifIntDist2(8, 11);

    std::stringstream ss;
    ss << std::hex;

    uint8_t i; // i don't use u8 here because i dont want to include nds.h
    for (i = 0; i < 8; ++i)
        ss << unifIntDist(mt);
    ss << '-';

    for (i = 0; i < 4; ++i)
        ss << unifIntDist(mt);
    ss << "-4";

    for (i = 0; i < 3; ++i)
        ss << unifIntDist2(mt);
    ss << '-';

    ss << unifIntDist2(mt);

    for (i = 0; i < 3; ++i)
        ss << unifIntDist(mt);
    ss << '-';

    for (i = 0; i < 12; ++i)
        ss << unifIntDist(mt);

    return ss.str();
}