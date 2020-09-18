#include "player.h"

// Position of player
float xPos = 0;
float yPos = 0;

// Y-Rotation value of player. 0 is up when looking at the 2D grid. NEEDS to be between 0 and 360
float rotation = 0;

float player_getXPos() {
    return xPos;
}

float player_getYPos() {
    return yPos;
}

float player_getRotation() {
    return rotation;
}

void player_setXPos(float value) {
    xPos = value;
}

void player_setYPos(float value) {
    yPos = value;
}

void player_setRotation(float value) {
    rotation = value;
}