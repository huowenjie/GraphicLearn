#include <SDL2/SDL.h>
#include "gr_window.h"
#include "gr_mem.h"

/*---------------------------------------------------------------------------*/

struct gr_window
{
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Event evt;
};

GR_WINDOW *gr_create_window(int width, int height)
{
    GR_WINDOW *window = NULL;

    SDL_Window *sdlwnd = NULL;
    SDL_Texture *texture = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return NULL;
	}

    window = GR_MALLOC(sizeof(GR_WINDOW));
    if (!window) {
        goto err;
    }
    memset(window, 0, sizeof(GR_WINDOW));

    sdlwnd = SDL_CreateWindow(
        "GraphicRender",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );

    if (!sdlwnd) {
        goto err;
    }

    renderer = SDL_CreateRenderer(sdlwnd, -1, 0);
    if (!renderer) {
        goto err;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        width,
        height
    );

    if (!texture) {
        goto err;
    }

    window->window = sdlwnd;
    window->texture = texture;
    window->renderer = renderer;

    return window;

err:
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (sdlwnd) {
        SDL_DestroyWindow(sdlwnd);
    }

    if (window) {
        GR_FREE(window);
    }

    SDL_Quit();
    return NULL;
}

void gr_destroy_window(GR_WINDOW *window)
{
    if (window) {
        if (window->texture) {
            SDL_DestroyTexture(window->texture);
        }

        if (window->renderer) {
            SDL_DestroyRenderer(window->renderer);
        }

        if (window->window) {
            SDL_DestroyWindow(window->window);
        }

        if (window) {
            GR_FREE(window);
        }

        SDL_Quit();
    }
}

GR_BOOL gr_win_event_capture(GR_WINDOW *window, GR_EVENT *evt)
{
    SDL_Event *sdlevt = &(window->evt);
    GR_BOOL capture = SDL_PollEvent(sdlevt) ? GR_TRUE : GR_FALSE;

    switch (sdlevt->type) {
    case SDL_QUIT:
        evt->type = GR_EVT_WIN;
        evt->winevt = GR_EVT_WIN_CLOSE;
        break;
    }

    return capture;
}

void gr_win_render(GR_WINDOW *window, const GR_UINT32 *buffer, int bw, int bh)
{
    SDL_Texture *texture = window->texture;
    SDL_Renderer *renderer = window->renderer;

    SDL_UpdateTexture(texture, NULL, buffer, bw * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
