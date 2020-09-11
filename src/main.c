#include <stdio.h>
#include <SDL.h> // for SDL_Delay

#include "renderer.h"
#include "mathUtil.h"

int main(int argc, char* args[]) {
    printf("Hello World!\n");
    initSDL();
    float angle = 45;
    int running = 1;
    SDL_Event e;
    while(running) {
        render(angle);
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                running = 0;
            } else if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_LEFT: angle = addAngle(angle, -3); break;
                    case SDLK_RIGHT: angle = addAngle(angle, 3); break;
                    default: break;
                }
            }
        }
    }
    return 0;
}