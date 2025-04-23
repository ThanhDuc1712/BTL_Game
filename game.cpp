#include "includes.h"


Tank::Tank() : x(384), y(1184), dx(0), dy(0), speed(INITIAL_SPEED), direction(NORTH) {

camera.y = y - SCREEN_HEIGHT / 2 + 16;
camera.y = std::max(0, std::min(camera.y, MAP_HEIGHT - SCREEN_HEIGHT));
}

int snapToGrid(int value, int tileSize, int differ = 6) {
    int mod = value % tileSize;
    if (mod < differ) return value - mod;
    if (mod > tileSize - differ) return value + (tileSize - mod);
    return value;
}

void Tank::move(){
    int nextX = x + dx;
    int nextY = y + dy;

    if (direction == NORTH || direction == SOUTH) {
        nextX = snapToGrid(x, TILE_SIZE);
    } else if (direction == EAST || direction == WEST) {
        nextY = snapToGrid(y, TILE_SIZE);
    }

    nextX = max(0,min(nextX, MAP_WIDTH - 32));
    nextY = max(0,min(nextY, MAP_HEIGHT - 32));


    if(!checkvatram(nextX, nextY)){
        x = nextX; y = nextY;

        if(y > camera.y + camera.h - 32 && direction == SOUTH && camera.y + camera.h < MAP_HEIGHT ){
           camera.y += speed;
        }
        else if(y < camera.y + 200 && camera.y > 0){
           camera.y -= speed;
        }

        if (dx != 0 || dy != 0) {
            sprite.tick();
        }else{
            sprite.currentFrame = sprite.startFrame;
        }
    }else{
        stop();
    }

}

void Tank::stop() {
    dx = 0;
    dy = 0;
}


void Tank::setDirection(Direction dir) {
    direction = dir;
    switch (dir) {
        case NORTH: sprite.setFramerange(0, 1); break;
        case SOUTH: sprite.setFramerange(2, 3); break;
        case WEST:  sprite.setFramerange(4, 5); break;
        case EAST:  sprite.setFramerange(6, 7); break;
    }
}

void Tank::updateVelocity(Direction dir) {
    switch (dir) {
        case NORTH: dy = -speed; dx = 0; break;
        case SOUTH: dy = speed; dx = 0; break;
        case WEST:  dx = -speed; dy = 0; break;
        case EAST:  dx = speed; dy = 0; break;
    }
}


void Bullet::fire(int sx, int sy, Direction dir, SDL_Texture* texV, SDL_Texture* texH){
    //if(active) return;

    direction = dir;
    speed = 13;
    active = true;

    switch (dir) {
        case NORTH: dy = -speed; dx = 0;
            texture = texV;
            x = sx;
            y = sy;
        break;
        case SOUTH: dy = speed; dx = 0;
            texture = texV;
            x = sx;
            y = sy;
        break;
        case WEST:  dx = -speed; dy = 0;
            texture = texH;
            x = sx - 7;
            y = sy;
        break;
        case EAST:  dx = speed; dy = 0;
            texture = texH;
            x = sx - 7;
            y = sy;
        break;
    }
}

void Bullet::move(){
    if(!active) return;

    x += dx;
    y += dy;

    if (x < camera.x || x > camera.x + SCREEN_WIDTH || y < camera.y || y > camera.y + SCREEN_HEIGHT)  {
    reset();
    }
}

void Bullet::render(Graphics& gfx) {
    if (active && texture) {
        gfx.renderTextureCamera(texture, x, y);
    }
}

void Bullet::reset() {
    active = false;
}


int tileMap[MAP_ROWS][MAP_COLS];
SDL_Rect brickClipMap[MAP_ROWS][MAP_COLS];

bool loadTileMap(const char* filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        cerr << "failed to load map file: " << filename << endl;
        return false;
    }

    for(int i = 0; i < MAP_ROWS; i++){
        for(int j = 0; j < MAP_COLS; j++){
            file >> tileMap[i][j];
            brickClipMap[i][j] = {0, 0, TILE_SIZE, TILE_SIZE};
        }
    }
    file.close();
    return true;
}

SDL_Texture* tileTextures[6];

void loadTileTextures(Graphics& pic){
    tileTextures[BRICK] = pic.loadTexture("brick.png");
    tileTextures[STEEL] = pic.loadTexture("steel.png");
    tileTextures[FOREST] = pic.loadTexture("forest.png");
    tileTextures[BRIDGE] = pic.loadTexture("bridge.png");
    tileTextures[WATER] = pic.loadTexture("water.png");
}

void renderTileMap(Graphics& pic, bool onlyforest){
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            int tileType = tileMap[row][col];

            if (tileType == EMPTY) continue;

            if (onlyforest) {
                if (tileType != FOREST) continue;
            } else {
                if (tileType == FOREST) continue;
            }

            int x = col * TILE_SIZE;
            int y = row * TILE_SIZE;
            if (tileType == BRICK) {
                SDL_Rect src = brickClipMap[row][col];

                if (src.w <= 0 || src.h <= 0 || tileTextures[BRICK] == nullptr) continue;
                SDL_Rect dest = {x + src.x, (y + src.y) - camera.y, src.w, src.h};
                SDL_RenderCopy(pic.renderer, tileTextures[BRICK], &src, &dest);
            } else {
                if (tileTextures[tileType])
                pic.renderTextureCamera(tileTextures[tileType], x, y);
            }
        }
    }
}

bool isPassTile(int tileType){
    return tileType == EMPTY || tileType == FOREST || tileType == BRIDGE;
}

bool checkvatram(int nextX, int nextY){
    SDL_Rect tankf = {nextX, nextY, 32, 32};

    for(int row= 0; row < MAP_ROWS; row++){
        for(int col = 0; col < MAP_COLS; col++){
            int tileType = tileMap[row][col];
            if(isPassTile(tileType)) continue;

            SDL_Rect tileBox = {col*TILE_SIZE, row*TILE_SIZE, TILE_SIZE, TILE_SIZE};

            if(SDL_HasIntersection(&tankf, &tileBox)){
                return true;
            }

        }
    }

    return false;
}

void BulletTile(Bullet& bullet){
    if(!bullet.active) return;

    int col = bullet.x / TILE_SIZE;
    int row = bullet.y / TILE_SIZE;

    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS) return;

    int tileType = tileMap[row][col];

    if (tileType == EMPTY || tileType == FOREST || tileType == BRIDGE) return;

    if (tileType == STEEL) {
        bullet.reset();
        return;
    }

    if(tileType == BRICK){
        SDL_Rect& shot = brickClipMap[row][col];

        switch (bullet.direction) {
            case SOUTH: shot.y += 8; shot.h -= 8; break;
            case NORTH: shot.h -= 8; break;
            case EAST:  shot.x += 8; shot.w -= 8; break;
            case WEST :  shot.w -= 8; break;
        }


    if(shot.w <= 0 || shot.h <= 0){
        tileMap[row][col] = EMPTY;
    }

    bullet.reset();

    }

}



bool gameOver(const Tank& player){
    // dieu kien cho xe dung sau nay
    return false;
}
