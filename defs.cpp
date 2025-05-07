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
    {192, 224}, {576,224},
    {0,352}, {768, 352},
    {96,480}, {672, 480},
    {32,800}, {192,800}, {576,800}, {736, 800},
    {0,960}, {768,960}
};


const char* LIVE_SPRITE_FILE ="Live.png";
const int LIVE_CLIPS[][4] = {
    {0 ,0 ,96 , 32},
    {0, 32, 96, 32},
    {0, 64, 96, 32}
};
const int LIVE_FRAMES = sizeof(LIVE_CLIPS)/sizeof(LIVE_CLIPS[0]);

int playerScore = 0;
int High_Score = 0;
const char* SCORE_FONT_FILE = "SigmarOne-Regular.ttf";
TTF_Font* scoreFont = nullptr;

const char* VICTORY_FILE = "VICTORY.png";
const int VICTORY_CLIP[][4] = {
    {0,0,160,64},
    {160,0,160,64}
};
const int VICTORY_FRAMES = sizeof(VICTORY_CLIP)/sizeof(VICTORY_CLIP[0]);

const char* VICTORY_MUSIC_FILE = "VICTORY_MUSIC.mp3";
Mix_Music* victoryMusic = nullptr;




