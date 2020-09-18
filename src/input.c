#include <SDL.h>
#include "player.h"
#include "mathUtil.h"

static SDL_Event e;

void processInputs(int *running) {
    while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                *running = 0;
            } else if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_LEFT: player_setRotation(addAngle(player_getRotation(), -3)); break;
                    case SDLK_RIGHT: player_setRotation(addAngle(player_getRotation(), 3)); break;
                    case SDLK_w: {
                        player_setXPos(player_getXPos() + player_getXPos() * sinDeg(player_getRotation()) * 0.01);
                        player_setYPos(player_getYPos() - player_getYPos() * cosDeg(player_getRotation()) * 0.01);
                    }  break;
                    default: break;
                }
            }
    }
}