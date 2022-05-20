#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "bp_matrix.h"
#include "bp_renderer.h"
#include "bp_trace.h"

#define PI ((float)3.14159265359f)

/* 计时器 */
static time_t start = 0;

/* 缓冲数组 */
static struct vec4f_point *buffer = NULL;

/* 初始化 */
void init();

/* 渲染 */
void update();

/* 释放资源 */
void quit();

/*-------------------------------------------------------*/

int main(int argc, char *argv[])
{
    BP_CONTEXT *ctx = bp_init(init, update, quit);
    bp_render(ctx);
    bp_quit(ctx);
}

/*-------------------------------------------------------*/

/* 相机位置 */
static struct vec4f cam_pos = { 0.0f, 0.0f, 1.5f, 1.0f };

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
    { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f },
    {  0.5f, -0.5f, 0.0f, 1.0f, 0.3f, 0.3f, 0.2f, 1.0f },
    { -0.5f,  0.5f, 0.0f, 1.0f, 0.1f, 0.4f, 0.3f, 1.0f },
    {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.3f, 0.2f, 1.0f },
    {  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.3f, 1.0f, 1.0f }
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

/*-------------------------------------------------------*/

void init()
{
    start = time(NULL);
    buffer = malloc(sizeof(vt_array));
    memcpy(buffer, vt_array, sizeof(vt_array));
}

void update()
{
    int i = 0;

    struct vec4f axis = { 0.0f, 1.0f, 0.0f, 0.0f };
    time_t cur = time(NULL) - start;

    mat4x4f_unit(&view);

    /* 构造旋转矩阵，一秒转动 10 度 */
    mat4x4f_rotate(&view, 10.0f * ((float)cur) * PI / 180.0f, &axis);

    /* 相机 + 透视变换 */
    mat4x4f_lookat(&view, &cam_pos, &cam_target, &cam_up);
    mat4x4f_perspective(&view, PI / 4.0f, to_max.x / to_max.y, 0.1f, 100.0f);
    mat4x4f_projection(&view, &from_min, &from_max, &to_min, &to_max);

    /* 将世界空间的坐标点变换到屏幕空间 */
    for (; i < sizeof(vt_array) / sizeof(vt_array[0]); i++) {
        mat4x4f_mult2vec(&view, (const struct vec4f *)&vt_array[i], (struct vec4f *)&buffer[i]);
        vec4f_homogeneous((struct vec4f *)&buffer[i]);
    }

    bp_draw_array(BP_DT_TRIANGLES, buffer, sizeof(vt_array) / sizeof(vt_array[0]));
}

void quit()
{
    if (buffer) {
        free(buffer);
    }
}
