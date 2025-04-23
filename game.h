#ifndef GAME_H
#define GAME_H

#include "includes.h"

struct Tank {
    int x,y;
    int dx, dy;
    int speed;
    Sprite sprite;
    Direction direction;
    Tank();

    void move();
    void setDirection(Direction dir);
    void updateVelocity(Direction dir);
    void stop();

};

struct Bullet{
    int x, y;
    int dx, dy;
    int speed;
    bool active;
    SDL_Texture* texture;
    Direction direction;

    void fire(int sx, int sy, Direction dir, SDL_Texture* texV, SDL_Texture* texH);
    void move();
    void render(Graphics& gfx);
    void reset();


};


enum TileType{
    EMPTY = 0,
    BRICK = 1,
    STEEL = 2,
    FOREST = 3,
    BRIDGE = 4,
    WATER = 5,
};


const int TILE_SIZE = 32;
const int MAP_COLS = 25;
const int MAP_ROWS = 38;

extern int tileMap[MAP_ROWS][MAP_COLS];
extern SDL_Rect brickClipMap[MAP_ROWS][MAP_COLS];
extern SDL_Texture* tileTextures[6];

bool loadTileMap(const char* filename);

void loadTileTextures(Graphics& path);
void renderTileMap(Graphics& path, bool onlyforest);
bool isPassTile(int tileType);
bool checkvatram(int nextX, int nextY);
int snapToGrid(int value, int tileSize, int differ);
void BulletTile(Bullet& bullet);



bool gameOver(const Tank& player);

#endif // GAME_H
