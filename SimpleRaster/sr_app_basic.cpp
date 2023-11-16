#include "sr_menu.h"
#include "sr_app.h"
#include "sr_global.h"
#include "sr_window.h"

//-----------------------------------------------------------------------------

// 主菜单
static char basicMenu[][MENU_STR_LEN] =
{
    "基础渲染测试",
    "1) 点是否在三角形内",
    "0) 退出"
};

// 判断点是否在三角形内
static void pointInsideTriangle();
static void pitUpdate(SR_Window &window);

//-----------------------------------------------------------------------------

void renderBasic()
{
    int index = 0;

    while (1) 
    {
        index = SELECT_MENU(basicMenu);
        switch (index) 
        {
        case 1:
            pointInsideTriangle();
            break;

        case 0:
            return;
        }
    }

}

//-----------------------------------------------------------------------------

void pointInsideTriangle()
{
    SR_Window window;

    window.initialize();
    window.setUpdate(pitUpdate);
    window.render();
    window.terminate();
}

void pitUpdate(SR_Window &window)
{
    float width = (float)window.getWidth();
    float height = (float)window.getHeight();

    SR_Vec2f a(width / 5.0f, height / 5.0f);
    SR_Vec2f b(width * 4.0f / 5.0f, height / 5.0f);
    SR_Vec2f c(width / 2.0f, height * 4.0f / 5.0f);
    SR_Color color(0.0f, 1.0f, 0.0f);

    // 待测样本
    int count = 256;
    SR_Vec2f *tp = new SR_Vec2f[count];

    window.drawLine(a, b, color);
    window.drawLine(b, c, color);
    window.drawLine(c, a, color);

    for (int i = 0; i < count; i++) {
        SR_Vec2f &pt = tp[i];

        pt.x = randomf(1.0f, width - 2.0f);
        pt.y = randomf(1.0f, height - 2.0f);

        // 随机五种颜色
        int colorSel = randomi(1, 5);
        switch(colorSel) {
        case 1: color.setRGB(1.0f, 0.0f, 1.0f); break;
        case 2: color.setRGB(1.0f, 1.0f, 0.0f); break;
        case 3: color.setRGB(0.0f, 1.0f, 1.0f); break;
        case 4: color.setRGB(1.0f, 1.0f, 1.0f); break;
        case 5: color.setRGB(0.0f, 0.0f, 1.0f); break;
        }

        // 如果点在三角形内部，则颜色设置为和三角形一样的颜色
        if (window.insideTriangle(a, b, c, pt)) {
            color.setRGB(0.0f, 1.0f, 0.0f);
        }

        window.drawPixel(pt, color);
    }

    delete[] tp;
}

//-----------------------------------------------------------------------------
