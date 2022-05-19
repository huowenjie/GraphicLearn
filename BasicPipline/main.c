#include <stddef.h>

#include "bp_matrix.h"
#include "bp_renderer.h"
#include "bp_trace.h"

#define PI ((float)3.14159265359f)

/* 初始化 */
void init();

/* 渲染 */
void update();

int main(int argc, char *argv[])
{
    BP_CONTEXT *ctx = bp_init(init, update, NULL);
    bp_render(ctx);
    bp_quit(ctx);
}

/* 相机位置 */
static struct vec4f cam_pos = { 0.8f, 0.0f, 1.5f, 1.0f };

/* 相机向上方向 */
static struct vec4f cam_up = { 0.0f, 1.0f, 0.0f, 0.0f };

/* 相机目标 */
static struct vec4f cam_target = { 0.0f, 0.0f, 0.0f, 1.0f };

/* 标准坐标系 */
static struct vec4f from_min = { -1.0f, -1.0f, -1.0f, 1.0f };
static struct vec4f from_max = {  1.0f,  1.0f,  1.0f, 1.0f };

/* 设备坐标系 */
static struct vec4f to_min = { 0.0f, 0.0f, -1.0f, 1.0f };
static struct vec4f to_max = { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 1.0f, 1.0f };

static struct mat4x4f view;

/* 顶点数组 */
struct vec4f_point vt_array[] = {
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }
#if 0
    { -0.5f, -0.5f,  0.5f, 1.0f },
    {  0.5f, -0.5f,  0.5f, 1.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f },
    {  0.5f,  0.5f,  0.5f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f },
    {  0.5f, -0.5f, -0.5f, 1.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f },
    {  0.5f,  0.5f, -0.5f, 1.0f }
#endif
};

void init()
{
    int i = 0;

    mat4x4f_unit(&view);

    /* 相机 + 透视变换 */
    mat4x4f_lookat(&view, &cam_pos, &cam_target, &cam_up);
    mat4x4f_perspective(&view, PI / 4.0f, to_max.x / to_max.y, 0.1f, 100.0f);
    mat4x4f_projection(&view, &from_min, &from_max, &to_min, &to_max);

    /* 将世界空间的坐标点变换到屏幕空间 */
    for (; i < sizeof(vt_array) / sizeof(vt_array[0]); i++) {
        mat4x4f_mult2vec(&view, (const struct vec4f *)&vt_array[i], (struct vec4f *)&vt_array[i]);
        vec4f_homogeneous((struct vec4f *)&vt_array[i]);
    }
}

void update()
{
    bp_draw_array(BP_DT_TRIANGLES, vt_array, sizeof(vt_array) / sizeof(vt_array[0]));
}
