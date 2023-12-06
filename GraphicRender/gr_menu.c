#include <stdio.h>
#include <string.h>
#include "gr_menu.h"

#define STR_BUFFER_LEN 64
#define INT_BUFFER_LEN 64

/*===========================================================================*/

const char *input_str(const char *prompt)
{
    char *ch = NULL;
    static char strBuff[STR_BUFFER_LEN] = { 0 };

    if (!prompt || !prompt[0])
    {
        return NULL;
    }

    printf("%s: ", prompt);

    if (!fgets(strBuff, STR_BUFFER_LEN, stdin))
    {
        return NULL;
    }

    ch = strrchr(strBuff, '\n');
    if (*ch)
    {
        *ch = 0;
    }

    return strBuff[0] ? strBuff : NULL;
}

int input_int(const char *prompt)
{
    int ret = -1;
    int fields = 0;

    char *ch = NULL;
    static char intBuff[INT_BUFFER_LEN] = { 0 };

    if (!prompt || !prompt[0])
    {
        return -1;
    }

    printf("%s: ", prompt);

    if (!fgets(intBuff, INT_BUFFER_LEN, stdin))
    {
        return -1;
    }

    ch = strrchr(intBuff, '\n');

    if (*ch)
    {
        *ch = 0;
    }

    fields = sscanf(intBuff, "%d", &ret);
    return (fields > 0) ? ret : -1;
}

int select_menu(char (*menu)[MENU_STR_LEN], int count)
{
    int i = 0;
    char *ch = '\0';

    int ret = -1;
    int fields = 0;

    static char intBuff[INT_BUFFER_LEN] = { 0 };

    if (!menu || count < 2)
    {
        return -1;
    }

    printf("\n");
    printf("%s\n", "========================================");
    printf("%s\n", menu[0]);
    printf("%s\n", "========================================");

    for (i = 1; i < count; i++)
    {
        printf("%s\n", menu[i]);
    }

    printf("%s\n", "----------------------------------------");
    printf("\n");

    if (!fgets(intBuff, INT_BUFFER_LEN, stdin))
    {
        return -1;
    }

    ch = strrchr(intBuff, '\n');
    if (*ch)
    {
        *ch = 0;
    }

    fields = sscanf(intBuff, "%d", &ret);
    return (fields > 0) ? ret : -1;
}

/*===========================================================================*/
