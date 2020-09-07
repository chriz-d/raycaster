#include <SDL.h>
#include "renderer.h"
#include "player.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static SDL_Window* window = NULL;

SDL_Window* getWindow(void);
SDL_Surface* getSurface(void);

void initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error init Video module, %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    }
}

void quitSDL() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render() {
    SDL_Surface* screenSurface = getSurface();
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
    SDL_UpdateWindowSurface(window);
}

SDL_Window* getWindow() {
    return window;
}

SDL_Surface* getSurface() {
    return SDL_GetWindowSurface(window);
}