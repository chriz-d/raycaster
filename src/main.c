#include <stdio.h>
#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* args[]) {
    printf("Hello World!\n");
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error init Video module, %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
        screenSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x0, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
        SDL_Delay(3000);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }


    return 0;
}