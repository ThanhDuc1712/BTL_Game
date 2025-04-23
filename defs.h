#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 608;
const int INITIAL_SPEED = 4;
extern const char* WINDOW_TITLE;
extern const char* TANK_SPRITE_FILE;
extern const int TANK_CLIPS[][4];
extern const int TANK_FRAMES;

enum Direction {NORTH, SOUTH, EAST, WEST};

extern const int MAP_HEIGHT;
extern const int MAP_WIDTH ;
extern SDL_Rect camera;


#endif // _DEFS__H
