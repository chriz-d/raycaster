#include <stdio.h>
#include <SDL.h> // for SDL_Delay

#include "renderer.h"

int main(int argc, char* args[]) {
    printf("Hello World!\n");
    initSDL();
    render();
    SDL_Delay(3000);
    return 0;
}