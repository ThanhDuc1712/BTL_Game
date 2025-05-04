#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 928;
const int SCREEN_HEIGHT = 608;
const int INITIAL_SPEED = 4;
extern const char* WINDOW_TITLE;
extern const char* TANK_SPRITE_FILE;
extern const char* ENEMY_SPRITE_FILE;
extern const int TANK_CLIPS[][4];
extern const int TANK_FRAMES;


enum Direction {NORTH, SOUTH, EAST, WEST};

extern const int MAP_HEIGHT;
extern const int MAP_WIDTH ;

extern SDL_Texture* bulletTexture;
extern SDL_Texture* bulletTexture1;

extern SDL_Rect camera;
extern const int spawnPos[12][2];

extern const char* LIVE_SPRITE_FILE;
extern const int LIVE_CLIPS[][4];
extern const int LIVE_FRAMES;



#endif // _DEFS__H
