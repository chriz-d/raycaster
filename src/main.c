#include <stdio.h>
#include <SDL.h>

#include "renderer.h"
#include "player.h"
#include "input.h"


int main(int argc, char* args[]) {
    initSDL();
    player_setXPos(2.5);
    player_setYPos(2.5);
    player_setRotation(270);
    
    int frames = 0;
    int running = 1;
    while(running) {
        render();
        processInputs(&running);
        frames++;
    }
    return 0;
}