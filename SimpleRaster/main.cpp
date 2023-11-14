#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_mat4x4f.h"
#include "sr_global.h"
#include "sr_fragment.h"

#include "menu.h"
#include "sr_app.h"

//-----------------------------------------------------------------------------

// 主菜单
static char mainMenu[][MENU_STR_LEN] =
{
	"主菜单",
	"1) 三棱锥光栅化",
	"0) 退出"
};

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int index = 0;

    while (1) 
    {
        index = SELECT_MENU(mainMenu);
        switch (index) 
        {
        case 1:
            renderPyramid();
            break;

        case 0:
            goto end;
        }
    }

end:
    return 0;
}

//-----------------------------------------------------------------------------

