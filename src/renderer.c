#include <SDL.h>
#include <math.h>
#include "renderer.h"
#include "mathUtil.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WALL_SIZE 64
#define PROJ_PLANE_DISTANCE (WINDOW_WIDTH / 2) / tanDeg(FOV / 2)
#define FOV 60

#define DEGREE_PER_RAY ((float )FOV / (float) WINDOW_WIDTH)



static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

float shootRay(float);
float getHorizIntersectDist(float);
float getVertIntersectDist(float);
int isOutOfBounds(int, int);
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

float xPos = 3.5;
float yPos = 3.5;

void initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error init Video module, %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void quitSDL() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render(float angle) {

    // for(int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
    //     SDL_RendererInfo info;
    //     SDL_GetRenderDriverInfo(i, &info);
    //     printf("Name: %s\nFlags: %d\nMax Texture width: %d\nMax Texture Height: %d\n\n", info.name, info.flags, info.max_texture_width, info.max_texture_height);
    // }

    //printf("proj plane distance: %f", (WINDOW_WIDTH / 2) / tanDeg(FOV / 2));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
    float currentRayAngle = angle;
    currentRayAngle = addAngle(currentRayAngle, -1 * (FOV / 2));
    for(int i = 0; i < WINDOW_WIDTH; i++) {
        float distance = shootRay(currentRayAngle);
        float projectedWallHeight = ((float)WALL_SIZE / distance) * PROJ_PLANE_DISTANCE;
        // draw stuff
        //printf("%f\n", distance);
        SDL_RenderDrawLine(renderer, i, (WINDOW_HEIGHT / 2) - (projectedWallHeight / 2), i, (WINDOW_HEIGHT / 2) + (projectedWallHeight / 2));
        //printf("Distance: %f Angle: %f\n", distance, currentRayAngle);
        currentRayAngle = addAngle(currentRayAngle, DEGREE_PER_RAY);
    }
    SDL_RenderPresent(renderer);
    printf("%s", SDL_GetError());
}

float shootRay(float angle) {
    float distanceHoriz = getHorizIntersectDist(angle);
    float distanceVert = getVertIntersectDist(angle);
    if(distanceHoriz < distanceVert) {
        return distanceHoriz;
    } else {
        return distanceVert;
    }
}

float getHorizIntersectDist(float angle) {
    int px_64 = xPos * WALL_SIZE;
    int py_64 = yPos * WALL_SIZE;
    int boundaryX;
    int boundaryY;
    if(90 > angle || 270 < angle) { // ray faces up
        boundaryY = ((int)(py_64 / WALL_SIZE)) * WALL_SIZE - 1;
    } else { // ray faces down
        boundaryY = ((int)(py_64 / WALL_SIZE)) * WALL_SIZE + WALL_SIZE;
    }
    if(angle != 90 && angle != 270) {
        boundaryX = px_64 + ((py_64 - boundaryY) * tanDeg(angle)); 
    } else {
        boundaryX = 9999;
    }
    //printf("tan(%f) = %f\n", angle, tanDeg(angle));
    //printf("%d\n", boundaryX);
    int gridBoundaryX = boundaryX / WALL_SIZE;
    int gridBoundaryY = boundaryY / WALL_SIZE;
    //printf("Checking %d, %d for wall.\n", gridBoundaryX, gridBoundaryY);
    while(!isOutOfBounds(gridBoundaryX, gridBoundaryY) && !(map[gridBoundaryY][gridBoundaryX])) {
        if(90 > angle || 270 < angle) { // ray faces up
            boundaryY -= WALL_SIZE;
        } else {
            boundaryY += WALL_SIZE;
        }
        boundaryX += (WALL_SIZE * tanDeg(angle));
        //printf("%d\n", boundaryX);
        gridBoundaryX = boundaryX / WALL_SIZE;
        gridBoundaryY = boundaryY / WALL_SIZE;
        //printf("Checking %d, %d for wall.\n", gridBoundaryX, gridBoundaryY);
    }
    //printf("Wall found at %d, %d with angle %f\n", gridBoundaryX, gridBoundaryY, angle);
    return sqrt(pow(px_64 - boundaryX, 2) + pow(py_64 - boundaryY, 2));
}

int isOutOfBounds(int x, int y) {
    return x < 0 || y < 0 || x > 20 || y > 20;
}

float getVertIntersectDist(float angle) {
    int px_64 = xPos * WALL_SIZE;
    int py_64 = yPos * WALL_SIZE;
    int boundaryX;
    int boundaryY;
    if(0 < angle && 180 > angle) { // ray faces right
        boundaryX = ((int)(px_64 / WALL_SIZE)) * WALL_SIZE + WALL_SIZE;
        //printf("Right!\n");
    } else { // ray faces left
        boundaryX = ((int)(px_64 / WALL_SIZE)) * WALL_SIZE - 1;
        //printf("Left!\n");
    }
    boundaryY = py_64 + ((px_64 - boundaryX) / tanDeg(angle));
    //printf("tan(%f) = %f\n", angle, tanDeg(angle));
    //printf("boundaryX = %d\nboundaryY = %d\n", boundaryX, boundaryY);
    int gridBoundaryX = boundaryX / WALL_SIZE;
    int gridBoundaryY = boundaryY / WALL_SIZE;
    //printf("Checking %d, %d for wall.\n", gridBoundaryX, gridBoundaryY);
    while(!isOutOfBounds(gridBoundaryX, gridBoundaryY) && !(map[gridBoundaryY][gridBoundaryX])) {
        if(0 < angle && 180 > angle) { // ray faces right
            boundaryX += WALL_SIZE;
        } else {
            boundaryX -= WALL_SIZE;
        }
        boundaryY -= (WALL_SIZE / tanDeg(angle));
        //printf("%d\n", boundaryX);
        gridBoundaryX = boundaryX / WALL_SIZE;
        gridBoundaryY = boundaryY / WALL_SIZE;
        //printf("Checking %d, %d for wall.\n", gridBoundaryX, gridBoundaryY);
    }
    //printf("Wall found at %d, %d with angle %f\n", gridBoundaryX, gridBoundaryY, angle);
    return sqrt(pow(px_64 - boundaryX, 2) + pow(py_64 - boundaryY, 2));
}

SDL_Window* getWindow() {
    return window;
}

SDL_Surface* getSurface() {
    return SDL_GetWindowSurface(window);
}