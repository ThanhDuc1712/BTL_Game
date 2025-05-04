#ifndef GAME_H
#define GAME_H

#include "includes.h"

struct Tank {
    int x,y;
    int dx, dy;
    int speed;
    int lives;
    bool immotank;
    int immotankTimer;
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
    GRASS = 6
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
bool checkvacham(int nextX, int nextY);
int snapToGrid(int value, int tileSize, int differ);
void BulletTile(Bullet& bullet);

struct EnemyTank {
    int x, y;
    int dx, dy;
    int speed;
    Sprite sprite;
    Direction direction;
    bool alive;
    int frameCounter;

    int bulletIndex;
};
extern Tank player;
const int enemyMax = 12;
extern EnemyTank enemy[enemyMax];
extern Bullet enemyBullets[];
extern const int MaxEnemy_bullet;
void idivEnemy(int index, int x, int y);
void updateEnemy();
void renderEnemy(Graphics& gfx);
void updateBullets(Bullet bullets[], int maxBullets);
void renderBullets(Bullet bullets[], int maxBullets, Graphics& gfx);
bool checkvacham_tank(const SDL_Rect& a, const SDL_Rect& b);
bool checkvacham_enemyOther(int id, int nextX, int nextY);
bool checkvacham_tankOther(int nextX, int nextY);




void checkBulletEnemyCollision(Bullet& bullet);
void checkEnemyBulletPlayerCollision();
void checkWaterCollision();
void resetPlayerPosition();

struct LivesDisplay {
    SDL_Texture* texture;
    Sprite sprite;
    void init(SDL_Texture* tex);
    void render(Graphics& gfx);
    void updateLives(int lives);
};
extern LivesDisplay livesDisplay;
void handleTankHit();


bool gameOver(const Tank& player);

#endif // GAME_H
