#include "includes.h"

const char* WINDOW_TITLE = "Tank 1975";
const char* TANK_SPRITE_FILE = "tankchuyendong.png";
const char* ENEMY_SPRITE_FILE = "tankchuyendong1.png";
const int TANK_CLIPS[][4] = {
    {0, 0, 32, 32},
    {32, 0, 32, 32},
    {64, 0, 32, 32},
    {96, 0, 32, 32},
    {128, 0, 32, 32},
    {160, 0 , 32, 32},
    {192, 0, 32, 32},
    {224, 0, 32, 32}};
const int TANK_FRAMES = sizeof(TANK_CLIPS)/sizeof(TANK_CLIPS[0]);


const int MAP_HEIGHT = 1216;
const int MAP_WIDTH = 800;

SDL_Rect camera = {0, 0, 800, 608};

const int spawnPos[12][2] = {
    {0,0}, {0,320}, {800,0}, {800,320},
    {192,416}, {608,416}, {32,864}, {768,864},
    {224,864}, {576,864}, {320, 288}, {480, 288}
};


const char* LIVE_SPRITE_FILE ="Live.png";
const int LIVE_CLIPS[][4] = {
    {0 ,0 ,96 , 32},
    {0, 32, 96, 32},
    {0, 64, 96, 32}
};
const int LIVE_FRAMES = sizeof(LIVE_CLIPS)/sizeof(LIVE_CLIPS[0]);
