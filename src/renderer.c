#include <SDL.h>
#include "renderer.h"
#include "mathUtil.h"
#include "player.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WALL_SIZE 64
#define PROJ_PLANE_DISTANCE ((float)WINDOW_WIDTH / 2) / tanDeg((float)FOV / 2)
#define FOV 60

#define DEGREE_PER_RAY ((float )FOV / (float) WINDOW_WIDTH)

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static int framesInSecond = 0;

float shootRay(float);
float getHorizIntersectDist(float);
float getVertIntersectDist(float);
int isOutOfBounds(int, int);
SDL_Window* getWindow(void);
unsigned int updateFPS(unsigned int, void*);

int map[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

void initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Error init Video module, %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_AddTimer(1000, updateFPS, NULL);
}

void quitSDL() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// renders a frame
void render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    // Iterate through window width and shoot ray for each row
    // Depending on distance to wall a vertical stripe will be drawn
    float currentRayAngle = player_getRotation();
    currentRayAngle = addAngle(currentRayAngle, -1 * (FOV / 2));
    for(int i = 0; i < WINDOW_WIDTH; i++) {
        float distortedDistance = shootRay(currentRayAngle);
        // Correct distorted distance with cos
        float distance = distortedDistance  * cosDeg(player_getRotation() - currentRayAngle);
        float projectedWallHeight = ((float)WALL_SIZE / distance) * PROJ_PLANE_DISTANCE;
        SDL_RenderDrawLine(renderer, i, (WINDOW_HEIGHT / 2) - (projectedWallHeight / 2), i, (WINDOW_HEIGHT / 2) + (projectedWallHeight / 2));
        currentRayAngle = addAngle(currentRayAngle, DEGREE_PER_RAY);
    }
    SDL_RenderPresent(renderer);
    framesInSecond++;
    printf("%s", SDL_GetError());
}

// actually shoots two rays for horizontal and vertical each and returns the lower value
float shootRay(float angle) {
    float distanceHoriz = getHorizIntersectDist(angle);
    float distanceVert = getVertIntersectDist(angle);
    if(distanceHoriz < distanceVert) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
        return distanceHoriz;
    } else {
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, SDL_ALPHA_OPAQUE);
        return distanceVert;
    }
}

// Computes the distance to a walls horizontal intersection (north / south edge)
float getHorizIntersectDist(float angle) {
    int px_64 = player_getXPos() * WALL_SIZE;
    int py_64 = player_getYPos() * WALL_SIZE;
    float boundaryX; // X-Coordinate of grid intersection
    float boundaryY; // Y-Coordinate of grid intersection

    // Get grid edge of current square where player is
    if(90 > angle || 270 < angle) { // ray faces up
        boundaryY = ((int)(py_64 / WALL_SIZE)) * WALL_SIZE - 0.001;
    } else { // ray faces down
        boundaryY = ((int)(py_64 / WALL_SIZE)) * WALL_SIZE + WALL_SIZE;
    }
    if(angle != 90 && angle != 270) { // tan(90) and tan(270) is undefined!
        boundaryX = px_64 + ((py_64 - boundaryY) * tanDeg(angle)); 
    } else {
        boundaryX = 99999; // big value, so the vertical distance will be chosen
    }
    // Get values for checking with map array
    int gridBoundaryX = (int)boundaryX / WALL_SIZE;
    int gridBoundaryY = (int)boundaryY / WALL_SIZE;
    // Now increment / decrement boundaryX and Y by set offsets until wall is found 
    // or out of bounds
    while(!isOutOfBounds(gridBoundaryX, gridBoundaryY) && !(map[gridBoundaryY][gridBoundaryX])) {
        if(90 > angle || 270 < angle) { // ray faces up
            boundaryY -= WALL_SIZE;
            boundaryX += (WALL_SIZE * tanDeg(angle));
        } else {
            boundaryY += WALL_SIZE;
            boundaryX -= (WALL_SIZE * tanDeg(angle));
        }
        gridBoundaryX = (int)boundaryX / WALL_SIZE;
        gridBoundaryY = (int)boundaryY / WALL_SIZE;
    }
    
    // return the distance to found wall
    return sqrt(pow(px_64 - boundaryX, 2) + pow(py_64 - boundaryY, 2));
}

// returns if coordinate is out of bounds
int isOutOfBounds(int x, int y) {
    return x < 0 || y < 0 || x > 20 || y > 20;
}

// Computes the distance to a walls vertical intersection (west / east edge)
float getVertIntersectDist(float angle) {
    int px_64 = player_getXPos() * WALL_SIZE;
    int py_64 = player_getYPos() * WALL_SIZE;
    float boundaryX;
    float boundaryY;
    if(0 < angle && 180 > angle) { // ray faces right
        boundaryX = ((int)(px_64 / WALL_SIZE)) * WALL_SIZE + WALL_SIZE;
    } else { // ray faces left
        boundaryX = ((int)(px_64 / WALL_SIZE)) * WALL_SIZE - 0.001;
    }
    boundaryY = py_64 + ((px_64 - boundaryX) / tanDeg(angle));
    int gridBoundaryX = (int)boundaryX / WALL_SIZE;
    int gridBoundaryY = (int)boundaryY / WALL_SIZE;
    while(!isOutOfBounds(gridBoundaryX, gridBoundaryY) && !(map[gridBoundaryY][gridBoundaryX])) {
        if(0 < angle && 180 > angle) { // ray faces right
            boundaryX += WALL_SIZE;
            boundaryY -= (WALL_SIZE / tanDeg(angle));
        } else {
            boundaryX -= WALL_SIZE;
            boundaryY += (WALL_SIZE / tanDeg(angle));
        }
        gridBoundaryX = (int)boundaryX / WALL_SIZE;
        gridBoundaryY = (int)boundaryY / WALL_SIZE;
    }
    return sqrt(pow(px_64 - boundaryX, 2) + pow(py_64 - boundaryY, 2));
}

SDL_Window* getWindow() {
    return window;
}

SDL_Surface* getSurface() {
    return SDL_GetWindowSurface(window);
}

unsigned int updateFPS(unsigned int interval, void* param) {
    char fps[16];
    sprintf(fps, "FPS: %d", framesInSecond);
    SDL_SetWindowTitle(window, fps);
    framesInSecond = 0;
    SDL_AddTimer(interval, updateFPS, NULL);
    return 0;
}