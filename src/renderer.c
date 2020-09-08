#include <SDL.h>
#include <math.h>
#include "renderer.h"
#include "player.h"
#include "mathUtil.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WALL_SIZE 64
#define FOV 70

#define DEGREE_PER_RAY ((float )FOV / (float) WINDOW_WIDTH)

static SDL_Window* window = NULL;

SDL_Window* getWindow(void);
SDL_Surface* getSurface(void);

int map[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

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

    float currentRayAngle = 0;
    currentRayAngle = addAngle(currentRayAngle, -1 * (FOV / 2));
    int px_64 = xPos * WALL_SIZE;
    int py_64 = yPos * WALL_SIZE;
    for(int i = 0; i < WINDOW_WIDTH; i++) {
        // horizontal
        int boundaryX;
        int boundaryY;
        if(90 > currentRayAngle || 270 < currentRayAngle) { // ray faces up
            boundaryY = ((int)(py_64 / WALL_SIZE)) * WALL_SIZE - 1;
        } else { // ray faces down
            boundaryY = ((int)(py_64 / WALL_SIZE)) * WALL_SIZE + WALL_SIZE;
        }
        boundaryX = px_64 + (py_64 - boundaryY) / tanDeg(currentRayAngle);
        int gridBoundaryX = boundaryX / WALL_SIZE;
        int gridBoundaryY = boundaryY / WALL_SIZE;
        while(!(map[gridBoundaryY][gridBoundaryX])) {
            if(90 > currentRayAngle || 270 < currentRayAngle) { // ray faces up
                boundaryY -= WALL_SIZE;
            } else {
                boundaryY += WALL_SIZE;
            }
            boundaryX += (WALL_SIZE / tanDeg(currentRayAngle));
            gridBoundaryX = boundaryX / WALL_SIZE;
            gridBoundaryY = boundaryY / WALL_SIZE;
        }
        printf("Wall found at %d, %d with angle %f\n", gridBoundaryX, gridBoundaryY, currentRayAngle);
        
        // vertical
        
        currentRayAngle = addAngle(currentRayAngle, DEGREE_PER_RAY);
    }
}

SDL_Window* getWindow() {
    return window;
}

SDL_Surface* getSurface() {
    return SDL_GetWindowSurface(window);
}