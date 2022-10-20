#include "srt_vec2f.h"
#include "srt_window.h"

int main(int argc, char *argv[])
{
    // SRT_Window window;

    // window.initialize();
    // window.render();
    // window.terminate();
    SRT_Vec2f v1(1.3f);
    SRT_Vec2f v2(2.1f, 4.0f);

    SRT_Vec2f v3 = v1 + v2;
    SRT_Vec2f v4 = -v3;
    SRT_Vec2f v5 = 10.0f * v1;
    SRT_Vec2f v6 = v1 * 20.0f;

    v1.printValue("v1");
    v2.printValue("v2");
    v3.printValue("v3");
    v4.printValue("v4");
    v5.printValue("v5");
    v6.printValue("v6");
    return 0;
}
