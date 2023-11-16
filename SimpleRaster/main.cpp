#include "sr_global.h"
#include "sr_menu.h"
#include "sr_app.h"

//-----------------------------------------------------------------------------

// 主菜单
static char mainMenu[][MENU_STR_LEN] =
{
	"主菜单",
	"1) 基础渲染测试",
    "2) 三棱锥光栅化",
	"0) 退出"
};

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int index = 0;

    initRandSeed();

    while (1) 
    {
        index = SELECT_MENU(mainMenu);
        switch (index) 
        {
        case 1:
            renderBasic();
            break;

        case 2:
            renderPyramid();
            break;

        case 0:
            return 0;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------

