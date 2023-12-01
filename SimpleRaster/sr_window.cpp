#include <SDL2/SDL.h>
#include <cstring>
#include <cstdio>

#include "sr_window.h"

//-----------------------------------------------------------------------------

// 窗口信息
class SR_WinCtx
{
public:
    SR_WinCtx();
    ~SR_WinCtx();

public:
    // 创建缓冲区，samplingCount 子缓冲采样层次 1、0=不缩放 2=2x2 4=4x4 8=8x8
    void createBuffer(int width, int height, int sampleLayer = 2);

    // 销毁缓冲区
    void destroyBuffer();

    // 清空缓冲区
    void clearBuffer(Uint32 color = 0, float dept = -FLT_MAX);

    // 判断颜色缓冲区是否被赋值
    bool isChildBeSet(const SR_Vec2f &parent, int x, int y);

    // 获取 parent 点像素下 (x, y) 点子像素的颜色值
    SR_Color getChildColor(const SR_Vec2f &parent, int x, int y);

    // 获取子像素点在父像素 parent 点下的浮点坐标
    SR_Vec2f getChildPos(const SR_Vec2f &parent, int x, int y);

    // 设置 parent 点像素下 (x, y) 的子像素颜色值
    void setChildColor(const SR_Vec2f &parent, int x, int y, const SR_Color &color);

    // 对子像素做深度测试
    bool childZbufferTest(const SR_Vec2f &parent, int x, int y, float depth);

    // // 设置颜色
    // void setSurfaceColor(Uint32 color, int x, int y);
    // void setChildSurfaceColor(Uint32 color, int x, int y);

    // // 设置深度
    // void setDepth(float depth, int x, int y);
    // void setChildDepth(float depth, int x, int y);

    // // 获取颜色
    // Uint32 getSurfaceColor(int x, int y) const;
    // Uint32 getChildSurfaceColor(int x, int y) const;

    // // 获取深度
    // float getDepth(int x, int y) const;
    // float setChildDepth(int x, int y) const;

public:
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Event evt;

    // 帧缓冲
    Uint32 *surfaceBuffer;

    // 深度缓冲
    float *zBuffer;

    // 缓冲区尺寸
    int bufferWidth;
    int bufferHeight;

    // 深度子缓冲层
    float *zBufferChild;

    // 颜色子缓冲层
    Uint32 *surfaceChild;

    // 子缓冲采样层次 1、0=不缩放 2=2x2 4=4x4 8=8x8
    int childSampleLayer;
};

SR_Window::SR_Window(int width, int height)
{
    info = new SR_WinCtx();

    info->createBuffer(width, height);
    this->start = nullptr;
    this->update = nullptr;
}

SR_Window::~SR_Window()
{
    if (info) {
        info->destroyBuffer();

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
        info->bufferWidth,
        info->bufferHeight,
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
        info->bufferWidth,
        info->bufferHeight
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
    if (!info) {
        return;
    }

    int w = info->bufferWidth;
    int h = info->bufferHeight;
    
    int x = pos.x;
    int y = pos.y;

    Uint32 *surfaceBuffer = info->surfaceBuffer;

    if (x > w - 1 || x < 0) {
        return;
    }

    if (y > h - 1 || y < 0) {
        return;
    }

    y = h - 1 - y;

    // 点光栅化
    surfaceBuffer[y * w + x] = color.toUintRGB();
}

void SR_Window::drawLine(const SR_Vec2f &a, const SR_Vec2f &b, const SR_Color &color)
{
    if (!info) {
        return;
    }

    int x0 = a.x;
    int y0 = a.y;
    int x1 = b.x;
    int y1 = b.y;

    int dx = 0;
    int dy = 0;

    int i = x0;
    int j = y0;

    float d = 0.0f;

    int w = info->bufferWidth;
    int h = info->bufferHeight;

    if (x0 > w - 1 || x0 < 0 ||
        x1 > w - 1 || x1 < 0 ) {
        return;
    }

    if (y0 > h - 1 || y0 < 0 ||
        y1 > h - 1 || y1 < 0) {
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
    if (!info) {
        return;
    }

    int w = info->bufferWidth;
    int h = info->bufferHeight;

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
     * 3.根据 a、b、c 来确定像素点的颜色
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
            float tx = j;
            float ty = i;

            float alpha = ((b.y - c.y) * tx + (c.x - b.x) * ty + b.x * c.y - c.x * b.y) / fa;
            float beta = ((c.y - a.y) * tx + (a.x - c.x) * ty + c.x * a.y - a.x * c.y) / fb;
            float gama = ((a.y - b.y) * tx + (b.x - a.x) * ty + a.x * b.y - b.x * a.y) / fc;

            if ((alpha >= 0.0f && beta >= 0.0f && gama >= 0.0f) &&
                 alpha <= 1.0f && beta <= 1.0f && gama <= 1.0f) {
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
                    frag.setResolution((float)w, (float)h);
                    pixelColor = fragmentShader(frag);
                } else {
                    pixelColor = frag.getFragColor();
                }

                pixelColor = pixelColor;
                SR_Vec2f pixelPos(tx, ty);

                /*
                 * 以 2x2 拆分采样法为例
                 * 
                 * 1. 遍历 j、i 像素点当前所有子像素，判定子像素中心点是否在三角形内，如果在三角形内，
                 *    则对该子像素做深度测试。如果通过深度测试，则该子像素的颜色=0.25 * 当前着色后的颜色;
                 *    否则，该子像素的值保持不变。
                 * 3. 遍历结束后，当前像素的颜色则为所有子像素颜色的累加和
                 */
                int count = info->childSampleLayer;

                if (count > 1) {
                    float childRate = 1.0f / ((float)(count * count));

                    SR_Color finalColor;

                    for (int m = 0; m < count; m++) {
                        for (int n = 0; n < count; n++) {
                            SR_Vec2f childPos = info->getChildPos(pixelPos, n, m);

                            // 通过重心坐标插值计算子像素深度的精确值
                            tx = childPos.x;
                            ty = childPos.y;

                            alpha = ((b.y - c.y) * tx + (c.x - b.x) * ty + b.x * c.y - c.x * b.y) / fa;
                            beta = ((c.y - a.y) * tx + (a.x - c.x) * ty + c.x * a.y - a.x * c.y) / fb;
                            gama = ((a.y - b.y) * tx + (b.x - a.x) * ty + a.x * b.y - b.x * a.y) / fc;

                            z = 1.0f / (alpha / va.vertex.w + beta / vb.vertex.w + gama / vc.vertex.w);

                            // 深度坐标插值
                            float dp = a.z * alpha + b.z * beta + c.z * gama;
                            dp *= z;

                            // 当前子像素若未被赋值则直接赋值当前像素点像素，避免出现黑线
                            if (!info->isChildBeSet(pixelPos, n, m)) {
                                info->setChildColor(pixelPos, n, m, pixelColor);
                            }

                            // 依次判断子像素是否在三角形内
                            if (insideTriangle(
                                SR_Vec2f(a.x, a.y),
                                SR_Vec2f(b.x, b.y),
                                SR_Vec2f(c.x, c.y),
                                childPos)) {
                                if (info->childZbufferTest(pixelPos, n, m, dp)) {
                                    finalColor = finalColor + childRate * pixelColor;
                                    info->setChildColor(pixelPos, n, m, pixelColor);
                                }
                            } else {
                                // 如果本像素未被赋值，则直接取当前像素的颜色赋值
                                finalColor = finalColor + childRate * info->getChildColor(pixelPos, n, m);
                            }
                        }
                    }

                    finalColor.clamp();
                    pixelColor = finalColor;
                }

                // 深度测试
                if (zbufferTest(pixelPos, fragCoord.z)) {
                    // 光栅化
                    drawPixel(pixelPos, pixelColor);
                }
            }
        }
    }
}

bool SR_Window::zbufferTest(const SR_Vec2f &pos, float depth)
{
    if (!info) {
        return false;
    }

    int w = info->bufferWidth;
    int h = info->bufferHeight;

    int x = pos.x;
    int y = pos.y;
    float *zbuf = info->zBuffer;

    if (x > w - 1 || x < 0) {
        return false;
    }

    if (y > h - 1 || y < 0) {
        return false;
    }

    y = h - 1 - y;

    if (depth >= zbuf[y * w + x]) {
        zbuf[y * w + x] = depth;
        return true;
    }
    return false;
}

bool SR_Window::zbufferChildTest(const SR_Vec2f &pos, float depth)
{
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

        info->clearBuffer(0, -FLT_MAX);

        if (this->update) {
            this->update(*this);
        }

        SDL_UpdateTexture(texture, nullptr, info->surfaceBuffer, info->bufferWidth * sizeof(Uint32));
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
    if (info) {
        return info->bufferWidth;
    }
    return 0;
}

int SR_Window::getHeight() const
{
    if (info) {
        return info->bufferHeight;
    }
    return 0;
}

//-----------------------------------------------------------------------------

SR_WinCtx::SR_WinCtx()
{
    window = nullptr;
    texture = nullptr;
    renderer = nullptr;

    surfaceBuffer = nullptr;
    zBuffer = nullptr;
    bufferWidth = 0;
    bufferHeight = 0;
    zBufferChild = nullptr;
    surfaceChild = nullptr;
    childSampleLayer = 0;
}

SR_WinCtx::~SR_WinCtx()
{

}

void SR_WinCtx::createBuffer(int width, int height, int sampleLayer)
{
    switch (sampleLayer) {
        case 2:
        case 4:
        case 8:
            childSampleLayer = sampleLayer;
            break;
        default:
            childSampleLayer = 1;
            break;
    }

    bufferWidth = width;
    bufferHeight = height;

    int bufferSize = width * height;

    // 最后真正用于渲染的的帧缓冲和深度缓冲区
    surfaceBuffer = new Uint32[bufferSize];
    zBuffer = new float[bufferSize];

    for (int i = 0; i < bufferSize; i++) {
        surfaceBuffer[i] = 0x00000000;
        zBuffer[i] = -FLT_MAX;
    }

    if (childSampleLayer > 1) {
        int count = bufferSize * childSampleLayer * childSampleLayer;

        surfaceChild = new Uint32[count];
        zBufferChild = new float[count];

        for (int i = 0; i < count; i++) {
            surfaceChild[i] = 0x00000000;
            zBufferChild[i] = -FLT_MAX;
        }
    }
}

void SR_WinCtx::destroyBuffer()
{
    if (zBufferChild) {
        delete[] zBufferChild;
    }

    if (surfaceChild) {
        delete[] surfaceChild;
    }

    if (surfaceBuffer) {
        delete[] surfaceBuffer;
    }
    
    if (zBuffer) {
        delete[] zBuffer;
    }

    childSampleLayer = 0;
    bufferWidth = 0;
    bufferHeight = 0;
    surfaceBuffer = nullptr;
    zBuffer = nullptr;
}

void SR_WinCtx::clearBuffer(Uint32 color, float dept)
{
    if (!surfaceBuffer || !zBuffer) {
        return;
    }
    
    if (!zBufferChild || !surfaceChild) {
        return;
    }

    int bufferSize = bufferWidth * bufferHeight;
    
    for (int i = 0; i < bufferSize; i++) {
        surfaceBuffer[i] = color;
        zBuffer[i] = dept;
    }

    if (childSampleLayer > 1) {
        int count = bufferSize * childSampleLayer * childSampleLayer;

        for (int i = 0; i < count; i++) {
            surfaceChild[i] = color;
            zBufferChild[i] = dept;
        }
    }
}

bool SR_WinCtx::isChildBeSet(const SR_Vec2f &parent, int x, int y)
{
    int px = parent.x;
    int py = parent.y;

    int w = bufferWidth;
    int h = bufferHeight;

    if (px > w - 1 || px < 0) {
        return false;
    }

    if (py > h - 1 || py < 0) {
        return false;
    }

    if (x > childSampleLayer - 1 || x < 0 ) {
        return false;
    }

    if (y > childSampleLayer - 1 || y < 0 ) {
        return false;
    }
    
    w *= childSampleLayer;
    h *= childSampleLayer;

    int cx = px * childSampleLayer;
    int cy = py * childSampleLayer;

    cy = h - 1 - cy;

    Uint32 childColor = surfaceChild[(cy - y) * w + (cx + x)];
    childColor &= 0x00FFFFFF;

    return childColor > 0;
}

// 获取 parent 点像素下 (x, y) 点子像素的颜色值
SR_Color SR_WinCtx::getChildColor(const SR_Vec2f &parent, int x, int y)
{
    int px = parent.x;
    int py = parent.y;

    int w = bufferWidth;
    int h = bufferHeight;

    if (px > w - 1 || px < 0) {
        return SR_Color();
    }

    if (py > h - 1 || py < 0) {
        return SR_Color();
    }

    if (x > childSampleLayer - 1 || x < 0 ) {
        return SR_Color();
    }

    if (y > childSampleLayer - 1 || y < 0 ) {
        return SR_Color();
    }
    
    w *= childSampleLayer;
    h *= childSampleLayer;

    int cx = px * childSampleLayer;
    int cy = py * childSampleLayer;

    cy = h - 1 - cy;

    Uint32 childColor = surfaceChild[(cy - y) * w + (cx + x)];
    return SR_Color::fromUintRGB(childColor);
}

// 获取子像素点在父像素 parent 点下的浮点坐标
SR_Vec2f SR_WinCtx::getChildPos(const SR_Vec2f &parent, int x, int y)
{
    if (x > childSampleLayer - 1 || x < 0 ) {
        return parent;
    }

    if (y > childSampleLayer - 1 || y < 0 ) {
        return parent;
    }

    if (childSampleLayer <= 1) {
        return parent;
    }

    SR_Vec2f pos = parent;

    x += (-childSampleLayer / 2);
    y += (-childSampleLayer / 2);

    float childRate = 1.0f / ((float)childSampleLayer);
    pos.x += (x * childRate);
    pos.y += (y * childRate);

    return pos;
}

// 设置 parent 点像素下 (x, y) 的子像素颜色值
void SR_WinCtx::setChildColor(const SR_Vec2f &parent, int x, int y, const SR_Color &color)
{
    int px = parent.x;
    int py = parent.y;

    int w = bufferWidth;
    int h = bufferHeight;

    if (px > w - 1 || px < 0) {
        return;
    }

    if (py > h - 1 || py < 0) {
        return;
    }

    if (x > childSampleLayer - 1 || x < 0 ) {
        return;
    }

    if (y > childSampleLayer - 1 || y < 0 ) {
        return;
    }
    
    w *= childSampleLayer;
    h *= childSampleLayer;

    int cx = px * childSampleLayer;
    int cy = py * childSampleLayer;

    cy = h - 1 - cy;
    surfaceChild[(cy - y) * w + (cx + x)] = color.toUintRGB();
}

// 对子像素做深度测试
bool SR_WinCtx::childZbufferTest(const SR_Vec2f &parent, int x, int y, float depth)
{
    int px = parent.x;
    int py = parent.y;

    int w = bufferWidth;
    int h = bufferHeight;

    if (px > w - 1 || px < 0) {
        return false;
    }

    if (py > h - 1 || py < 0) {
        return false;
    }

    if (x > childSampleLayer - 1 || x < 0 ) {
        return false;
    }

    if (y > childSampleLayer - 1 || y < 0 ) {
        return false;
    }

    w *= childSampleLayer;
    h *= childSampleLayer;

    int cx = px * childSampleLayer;
    int cy = py * childSampleLayer;

    cy = h - 1 - cy;
    if (depth >= zBufferChild[(cy - y) * w + (cx + x)]) {
        zBufferChild[(cy - y) * w + (cx + x)] = depth;
        return true;
    }
    return false;
}

#if 0
void SR_WinCtx::setSurfaceColor(Uint32 color, int x, int y)
{
    if (!surfaceBuffer) {
        surfaceBuffer[y * bufferWidth + x] = color;
    }
}

void SR_WinCtx::setChildSurfaceColor(Uint32 color, int x, int y)
{
    if (!surfaceChild) {
        int width = bufferWidth * childSampleLayer;
        surfaceChild[y * width + x] = color;
    }
}

void SR_WinCtx::setDepth(float depth, int x, int y)
{
    if (!zBuffer) {
        zBuffer[y * bufferWidth + x] = depth;
    }
}

void SR_WinCtx::setChildDepth(float depth, int x, int y)
{
    if (!zBufferChild) {
        int width = bufferWidth * childSampleLayer;
        zBufferChild[y * width + x] = depth;
    }
}

Uint32 SR_WinCtx::getSurfaceColor(int x, int y) const
{
    if (!surfaceBuffer) {
        return surfaceBuffer[y * bufferWidth + x];
    }
    return 0;
}

Uint32 SR_WinCtx::getChildSurfaceColor(int x, int y) const
{
    if (!surfaceChild) {
        int width = bufferWidth * childSampleLayer;
        return surfaceChild[y * width + x];
    }
    return 0;
}

float SR_WinCtx::getDepth(int x, int y) const
{
    if (!zBuffer) {
        return zBuffer[y * bufferWidth + x];
    }
    return -FLT_MAX;
}

float SR_WinCtx::setChildDepth(int x, int y) const
{
    if (!zBufferChild) {
        int width = bufferWidth * childSampleLayer;
        return zBufferChild[y * width + x];
    }
    return -FLT_MAX;
}
#endif

//-----------------------------------------------------------------------------