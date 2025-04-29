#include <stddef.h>
#include "gr_mem.h"
#include "gr_menu.h"
#include "gr_app.h"
#include "gr_global.h"
#include "gr_engine.h"

/*===========================================================================*/

// 主菜单
static char main_menu[][MENU_STR_LEN] =
{
	"主菜单",
	"1) 基础渲染测试",
    "2) 光线追踪",
	"0) 退出"
};

/*===========================================================================*/

int main(int argc, char *argv[])
{
    int index = 0;
    GR_MEM_START;

    randinit();

    while (1) 
    {
        index = SELECT_MENU(main_menu);
        switch (index) 
        {
        case 1:
            render_basic();
            break;

        case 2:
            raytrace();
            break;

        case 0:
            goto end;
        }
    }

end:
    GR_PRINT_LEAK_INFO;
    GR_MEM_END;
    return 0;
}

/*===========================================================================*/
