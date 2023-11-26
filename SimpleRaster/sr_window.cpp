#include <SDL2/SDL.h>
#include <cstring>
#include <cstdio>

#include "sr_window.h"

//-----------------------------------------------------------------------------

// 窗口信息
class SR_WinCtx
{
public:
    SR_WinCtx()
    {
        window = nullptr;
        texture = nullptr;
        renderer = nullptr;

        surfaceBuffer = nullptr;
        zBuffer = nullptr;
    }
public:
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Event evt;

    // 帧缓冲
    Uint32 *surfaceBuffer;

    // 深度缓冲
    float *zBuffer;
};

SR_Window::SR_Window(int width, int height)
{
    info = new SR_WinCtx();
    winWidth = width;
    winHeight = height;

    Uint32 *buffer = new Uint32[width * height];
    float *zbuf = new float[width * height];

    for (int i = 0; i < width * height; i++) {
        buffer[i] = 0x00000000;
    }

    for (int i = 0; i < width * height; i++) {
        zbuf[i] = -FLT_MAX;
    }

    info->surfaceBuffer = buffer;
    info->zBuffer = zbuf;
    this->start = nullptr;
    this->update = nullptr;
}

SR_Window::~SR_Window()
{
    if (info) {
        if (info->surfaceBuffer) {
            delete info->surfaceBuffer;
            info->surfaceBuffer = nullptr;
        }

        if (info->zBuffer) {
            delete info->zBuffer;
            info->zBuffer = nullptr;
        }

        delete info;
        info = nullptr;
    }
}

bool SR_Window::initialize()
{    
    SDL_Window *window = nullptr;
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!info) {
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

    window = SDL_CreateWindow(
        "SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        winWidth,
        winHeight,
        0
    );

    if (!window) {
        goto err;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        goto err;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        winWidth,
        winHeight
    );

    if (!texture) {
        goto err;
    }

    info->window = window;
    info->texture = texture;
    info->renderer = renderer;

    return true;

err:
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return false;
}

bool SR_Window::terminate()
{
    if (!info) {
        return false;
    }

    if (info->texture) {
        SDL_DestroyTexture(info->texture);
    }

    if (info->renderer) {
        SDL_DestroyRenderer(info->renderer);
    }

    if (info->window) {
        SDL_DestroyWindow(info->window);
    }

    SDL_Quit();
    return true;
}

void SR_Window::setStart(void (*start)(SR_Window &))
{
    this->start = start;
}

void SR_Window::setUpdate(void (*update)(SR_Window &))
{
    this->update = update;
}

void SR_Window::drawPixel(const SR_Vec2f &pos, const SR_Color &color)
{
    int x = pos.x;
    int y = pos.y;
    Uint32 *surfaceBuffer = info->surfaceBuffer;

    if (x > winWidth - 1 || x < 0) {
        return;
    }

    if (y > winHeight - 1 || y < 0) {
        return;
    }

    y = winHeight - 1 - y;

    // 点光栅化
    surfaceBuffer[y * winWidth + x] = color.toUintRGB();
}

void SR_Window::drawLine(const SR_Vec2f &a, const SR_Vec2f &b, const SR_Color &color)
{
    int x0 = a.x;
    int y0 = a.y;
    int x1 = b.x;
    int y1 = b.y;

    int dx = 0;
    int dy = 0;

    int i = x0;
    int j = y0;

    float d = 0.0f;

    if (x0 > winWidth - 1 || x0 < 0 ||
        x1 > winWidth - 1 || x1 < 0 ) {
        return;
    }

    if (y0 > winHeight - 1 || y0 < 0 ||
        y1 > winHeight - 1 || y1 < 0) {
        return;
    }

    if (x0 > x1) {
        int tmp = 0;

        tmp = x0;
        x0 = x1;
        x1 = tmp;

        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    // 两个坐标点相同则绘制点
    if (!dx && !dy) {
        drawPixel(SR_Vec2f(x0, y0), color);
        return;
    }

    i = x0;
    j = y0;

    /*
     * f(x, y) = f(x, y)
     * f(x + 1, y) = f(x, y) + (y0 - y1)
     * f(x, y + 1) = f(x, y) + (x1 - x0)
     * f(x + 1, y + 1) = f(x, y) + (y0 - y1) + (x1 - x0)
     * 
     * 有了以上公式，可以使绘制效率更高一点
     */

    if (dy > 0 && dy <= dx) {
        d = (float)((y0 - y1) * (x0 + 1)) + 
            ((float)(x1 - x0)) * ((float)(y0 + 0.5f)) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 (0, 1]，x 变化快于 y，以 x 为锚
        while (i <= x1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d < 0) {
                j++;
                d = d + (float)((x1 - x0) + (y0 - y1));
            } else {
                d = d + (float)(y0 - y1);
            }

            i++;
        }
    } else if (dy > 0 && dy > dx) {
        d = ((float)(y0 - y1)) * ((float)x0 + 0.5f) + 
            (float)(x1 - x0) * (y0 + 1) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 [1, infinity) y 的变化快于 x，以 y 为锚
        while (j <= y1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d > 0) {
                i++;
                d = d + (float)((x1 - x0) + (y0 - y1));
            } else {
                d = d + (float)(x1 - x0);
            }

            j++;
        }
    } else if (dy < 0 && -dy <= dx) {
        d = (float)((y0 - y1) * (x0 + 1)) + 
            ((float)(x1 - x0)) * ((float)(y0 - 0.5f)) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 [-1, 0)，x 变化快于 y，以 x 为锚
        while (i <= x1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d > 0) {
                j--;
                d = d - (float)(x1 - x0) + (float)(y0 - y1);
            } else {
                d = d + (float)(y0 - y1);
            }

            i++;
        }
    } else if (dy < 0 && -dy > dx) {
        // 斜率 (-infinite, -1)，y 变化快于 x，以 y 为锚
        d = ((float)(y0 - y1)) * ((float)x0 + 0.5f) + 
            (float)(x1 - x0) * (y0 - 1) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 [1, infinity) y 的变化快于 x，以 y 为锚
        while (j >= y1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d < 0) {
                i++;
                d = d - (float)(x1 - x0) + (float)(y0 - y1);
            } else {
                d = d - (float)(x1 - x0);
            }

            j--;
        }
    } else if (!dx) {
        // 绘制一条平行于 y 轴的竖线
        i = y0;
        j = y0 < y1 ? 1 : -1;

        while (i != y1) {
            drawPixel(SR_Vec2f(i, j), color);
            i += j;
        }
    } else if (!dy) {
        // 绘制一条平行于 x 轴的横线
        while (i <= x1) {
            drawPixel(SR_Vec2f(i, j), color);
            i++;
        }
    }
}

void SR_Window::rasterizeTriangle(
    const SR_TriangleIndexList &list,
    const SR_VertexInfo &va,
    const SR_VertexInfo &vb,
    const SR_VertexInfo &vc,
    SR_Color (*fragmentShader)(const SR_Fragment &)
)
{
    const SR_Vec4f &a = va.vertex;
    const SR_Vec4f &b = vb.vertex;
    const SR_Vec4f &c = vc.vertex;

    float xmin = a.x;
    float xmax = a.x;
    float ymin = a.y;
    float ymax = a.y;

    float i = 0;
    float j = 0;

    float fa = 0.0f;
    float fb = 0.0f;
    float fc = 0.0f;

    /*
     * 1.确定三角形的包围盒 xmin xmax ymin ymax；
     * 2.将像素点索引代入两点式求出重心坐标系数 a，b，c；
     * 3.拟定一个屏幕外的点 (-1, -1)，两个三角形共线时，以共线为分隔线，其中一个三角形的
     *   边上的像素点必定和该屏幕外点处于同一侧，以此来判断是否绘制；
     * 4.根据 a、b、c 来确定像素点的颜色
     * 
     * f01(x, y) = (y0 - y1)x + (x1 - x0)y + x0y1 - x1y0
     * f12(x, y) = (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1
     * f20(x, y) = (y2 - y0)x + (x0 - x2)y + x2y0 - x0y2
     * 
     * a = f12(x, y) / f12(x0, y0)
     * b = f20(x, y) / f20(x1, y1)
     * c = f01(x, y) / f01(x2, y2)
     */
    xmin = xmin < b.x ? xmin : b.x;
    xmin = xmin < c.x ? xmin : c.x;
    xmax = xmax > b.x ? xmax : b.x;
    xmax = xmax > c.x ? xmax : c.x;

    ymin = ymin < b.y ? ymin : b.y;
    ymin = ymin < c.y ? ymin : c.y;
    ymax = ymax > b.y ? ymax : b.y;
    ymax = ymax > c.y ? ymax : c.y;

    fa = (b.y - c.y) * a.x + (c.x - b.x) * a.y + b.x * c.y - c.x * b.y;
    fb = (c.y - a.y) * b.x + (a.x - c.x) * b.y + c.x * a.y - a.x * c.y;
    fc = (a.y - b.y) * c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y;

    for (i = ymin; i <= ymax; i++) {
        for (j = xmin; j <= xmax; j++) {
            float alpha = ((b.y - c.y) * j + (c.x - b.x) * i + b.x * c.y - c.x * b.y) / fa;
            float beta = ((c.y - a.y) * j + (a.x - c.x) * i + c.x * a.y - a.x * c.y) / fb;
            float gama = ((a.y - b.y) * j + (b.x - a.x) * i + a.x * b.y - b.x * a.y) / fc;

            if ((alpha >= 0.0f && beta >= 0.0f && gama >= 0.0f) &&
                 alpha <= 1.0f && beta <= 1.0f && gama <= 1.0f) {
                float ta = fa * ((b.y - c.y) * (-1.0f) + (c.x - b.x) * (-1.0f) + b.x * c.y - c.x * b.y);
                float tb = fb * ((c.y - a.y) * (-1.0f) + (a.x - c.x) * (-1.0f) + c.x * a.y - a.x * c.y);
                float tc = fc * ((a.y - b.y) * (-1.0f) + (b.x - a.x) * (-1.0f) + a.x * b.y - b.x * a.y);

                if ((alpha >= 0.0f || ta > 0.0f) &&
                    (beta  >= 0.0f || tb > 0.0f) &&
                    (gama  >= 0.0f || tc > 0.0f)) {
                    SR_Fragment frag;

                    // 透视矫正参数
                    float z = 
                        1.0f / (alpha / va.vertex.w + beta / vb.vertex.w + gama / vc.vertex.w);

                    // 颜色插值
                    frag.interpolateFragColor(alpha, beta, gama, z, va.color, vb.color, vc.color);

                    // 片元坐标插值
                    SR_Vec4f fragCoord;
                    fragCoord = alpha * a + beta * b + gama * c;
                    fragCoord = fragCoord * z;

                    frag.setFragCoord(
                        SR_Vec3f(
                            fragCoord.x / fragCoord.w,
                            fragCoord.y / fragCoord.w,
                            fragCoord.z / fragCoord.w
                        )
                    );

                    // 像素最终颜色
                    SR_Color pixelColor;

                    if (fragmentShader) {
                        // 片元世界空间坐标插值
                        frag.interpolateFragGlobal(alpha, beta, gama, z, va.global, vb.global, vc.global);

                        // 顶点法线插值
                        frag.interpolateNormal(alpha, beta, gama, z, va.normal, vb.normal, vc.normal);

                        // 设置面法线
                        frag.setFragSurfaceNormal(list.normal);

                        // uv 坐标插值
                        frag.interpolateUV(alpha, beta, gama, z, va.uv, vb.uv, vc.uv);

                        // 设置分辨率
                        frag.setResolution((float)winWidth, (float)winHeight);
                        pixelColor = fragmentShader(frag);
                    } else {
                        pixelColor = frag.getFragColor();
                    }

                    pixelColor = pixelColor;
                    SR_Vec2f pixelPos(j, i);

                    // 深度测试
                    if (zbufferTest(pixelPos, fragCoord.z)) {
                        // 光栅化
                        drawPixel(pixelPos, pixelColor);
                    }
                }
            }
        }
    }
}

bool SR_Window::zbufferTest(const SR_Vec2f &pos, float depth)
{
    int x = pos.x;
    int y = pos.y;
    float *zbuf = info->zBuffer;

    if (x > winWidth - 1 || x < 0) {
        return false;
    }

    if (y > winHeight - 1 || y < 0) {
        return false;
    }

    y = winHeight - 1 - y;

    if (depth >= zbuf[y * winWidth + x]) {
        zbuf[y * winWidth + x] = depth;
        return true;
    }
    return false;
}

bool SR_Window::insideTriangle(
    const SR_Vec2f &a,
    const SR_Vec2f &b,
    const SR_Vec2f &c,
    const SR_Vec2f &p
)
{
    // 采用重心法来判断
    float fa = (b.y - c.y) * a.x + (c.x - b.x) * a.y + b.x * c.y - c.x * b.y;
    float fb = (c.y - a.y) * b.x + (a.x - c.x) * b.y + c.x * a.y - a.x * c.y;
    float fc = (a.y - b.y) * c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y;

    float alpha = ((b.y - c.y) * p.x + (c.x - b.x) * p.y + b.x * c.y - c.x * b.y) / fa;
    float beta  = ((c.y - a.y) * p.x + (a.x - c.x) * p.y + c.x * a.y - a.x * c.y) / fb;
    float gama  = ((a.y - b.y) * p.x + (b.x - a.x) * p.y + a.x * b.y - b.x * a.y) / fc;

    return ((alpha >= 0.0f) && (alpha <= 1.0f)) &&
           ((beta  >= 0.0f) && (beta  <= 1.0f)) &&
           ((gama  >= 0.0f) && (gama  <= 1.0f));
}

void SR_Window::render()
{
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event *evt = nullptr;

    if (!info) {
        return;
    }

    texture = info->texture;
    renderer = info->renderer;
    evt = &(info->evt);

    if (this->start) {
        this->start(*this);
    }

    while (1) {
        while (SDL_PollEvent(evt)) {
            if (evt->type == SDL_QUIT) {
                return;
            }
        }

        int count = winWidth * winHeight;

        // 清屏
        Uint32 *buffer = info->surfaceBuffer;
        float *zbuffer = info->zBuffer;

        for (int i = 0; i < count; i++) {
            buffer[i] = 0x00000000;
        }

        for (int i = 0; i < count; i++) {
            zbuffer[i] = -FLT_MAX;
        }

        if (this->update) {
            this->update(*this);
        }

        SDL_UpdateTexture(texture, nullptr, info->surfaceBuffer, winWidth * sizeof(Uint32));
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        SDL_Delay(100);
    }
}

void SR_Window::clearWindow()
{
    SDL_Renderer *renderer = nullptr;

    if (!info) {
        return;
    }

    renderer = info->renderer;

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int SR_Window::getWidth() const
{
    return winWidth;
}

int SR_Window::getHeight() const
{
    return winHeight;
}

//-----------------------------------------------------------------------------