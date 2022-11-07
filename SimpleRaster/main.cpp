#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_mat4x4f.h"
#include "sr_global.h"

static void update(SR_Window &window);

int main(int argc, char *argv[])
{
    SR_Window window;

    window.initialize();

    window.setUpdate(update);
    window.render();

    window.terminate();

    return 0;
}

void update(SR_Window &window)
{
    int w = window.getWidth();
    int h = window.getHeight();
    float respect = (float)w / (float)h;

    SR_Mat4x4f vp = SR_Mat4x4f::viewportMatrix(w, h);
    SR_Mat4x4f per = SR_Mat4x4f::perspectiveMatrix(PI / 4.0f, respect, 0.1f, 1.0f);
    SR_Mat4x4f cam = SR_Mat4x4f::cameraMatrix(
        SR_Vec4f(0.0f, 0.0f, 2.0f, 1.0f),
        SR_Vec4f(0.0f, 0.0f, 0.0f, 1.0f)
    );
    SR_Mat4x4f orth;

    // 正交视图
    SR_Mat4x4f proj = vp * orth * cam;

    SR_Vec4f vs[8] = {
        proj * SR_Vec4f(-0.5f, -0.5f, 0.5f, 1.0f),
        proj * SR_Vec4f(0.5f, -0.5f, 0.5f, 1.0f),
        proj * SR_Vec4f(0.0f, -0.5f, -0.5f, 1.0f),
        proj * SR_Vec4f(-0.5f, -0.5f, 0.5f, 1.0f)
    };

    for (int i = 0; i < 3; i++) {
        window.drawLine(
            SR_Vec2f(vs[i].x, vs[i].y),
            SR_Vec2f(vs[i + 1].x, vs[i + 1].y),
            SR_Color(1.0f)
        );
    }
}
