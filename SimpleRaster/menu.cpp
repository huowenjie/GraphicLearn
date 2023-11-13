#include <cstdio>
#include <cstring>
#include "menu.h"

#define STR_BUFFER_LEN 64
#define INT_BUFFER_LEN 64

//-----------------------------------------------------------------------------

const char *InputStr(const char *prompt)
{
    if (!prompt || !prompt[0])
    {
        return NULL;
    }

    std::printf("%s: ", prompt);

    static char strBuff[STR_BUFFER_LEN] = { 0 };

    if (!std::fgets(strBuff, STR_BUFFER_LEN, stdin))
    {
        return NULL;
    }

    char *ch = std::strrchr(strBuff, '\n');

    if (*ch)
    {
        *ch = 0;
    }

    return strBuff[0] ? strBuff : NULL;
}

int InputInt(const char *prompt)
{
    if (!prompt || !prompt[0])
    {
        return -1;
    }

    std::printf("%s: ", prompt);

    static char intBuff[INT_BUFFER_LEN] = { 0 };

    if (!std::fgets(intBuff, INT_BUFFER_LEN, stdin))
    {
        return -1;
    }

    char *ch = std::strrchr(intBuff, '\n');

    if (*ch)
    {
        *ch = 0;
    }

    int ret = -1;
    int fields = std::sscanf(intBuff, "%d", &ret);

    return (fields > 0) ? ret : -1;
}

int SelectMenu(char (*menu)[MENU_STR_LEN], int count)
{
    if (!menu || count < 2)
    {
        return -1;
    }

    std::printf("\n");
    std::printf("%s\n", "========================================");
    std::printf("%s\n", menu[0]);
    std::printf("%s\n", "========================================");

    for (int i = 1; i < count; i++)
    {
        std::printf("%s\n", menu[i]);
    }

    std::printf("%s\n", "----------------------------------------");
    std::printf("\n");

    static char intBuff[INT_BUFFER_LEN] = { 0 };

    if (!std::fgets(intBuff, INT_BUFFER_LEN, stdin))
    {
        return -1;
    }

    char *ch = std::strrchr(intBuff, '\n');

    if (*ch)
    {
        *ch = 0;
    }

    int ret = -1;
    int fields = std::sscanf(intBuff, "%d", &ret);

    return (fields > 0) ? ret : -1;
}

//-----------------------------------------------------------------------------
