#include "includes.h"

Tank::Tank() : x(384), y(1184), dx(0), dy(0), speed(INITIAL_SPEED), lives(3), immotank(false), immotankTimer(0), direction(NORTH){

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


    if(!checkvacham(nextX, nextY) && !checkvacham_tankOther(nextX, nextY)){
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

        checkWaterCollision();
    }else{
        stop();
    }
    if(immotank){
        immotankTimer--;
        if(immotankTimer <= 0) immotank = false;
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
    if (active && texture && x < 800 - 16) {
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
    tileTextures[GRASS] = pic.loadTexture("grass.png");
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
    return tileType == EMPTY || tileType == FOREST || tileType == BRIDGE || tileType == GRASS;
}

bool checkvacham(int nextX, int nextY){
    SDL_Rect tankf = {nextX, nextY, 32, 32};

    for(int row= 0; row < MAP_ROWS; row++){
        for(int col = 0; col < MAP_COLS; col++){
            int tileType = tileMap[row][col];
            if(isPassTile(tileType)) continue;

            if (tileType == BRICK) {
                SDL_Rect test = brickClipMap[row][col];

                SDL_Rect tileBox = {col * TILE_SIZE + test.x, row * TILE_SIZE + test.y, test.w, test.h};

                if(SDL_HasIntersection(&tankf, &tileBox)){
                return true;
                }

                continue;
            }


            SDL_Rect tileBox = {col*TILE_SIZE, row*TILE_SIZE, TILE_SIZE, TILE_SIZE};

            if (SDL_HasIntersection(&tankf, &tileBox)) {
                return true;
            }

        }
    }

    return false;
}

const int BULLET_WIDTH = 32;
const int BULLET_HEIGHT = 32;

void BulletTile(Bullet& bullet){
    if (!bullet.active) return;


    int centerX = bullet.x + BULLET_WIDTH / 2;
    int centerY = bullet.y + BULLET_HEIGHT / 2;

    int col = centerX / TILE_SIZE;
    int row = centerY / TILE_SIZE;
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS) return;
    int tileType = tileMap[row][col];

    if (tileType == EMPTY || tileType == FOREST || tileType == BRIDGE) return;
    if (tileType == STEEL) {
        bullet.reset();
        return;
    }
    if (tileType == BRICK) {
        SDL_Rect& shot = brickClipMap[row][col];
        switch (bullet.direction) {
            case SOUTH: shot.y += 8; shot.h -= 8; break;
            case NORTH: shot.h -= 8; break;
            case EAST:  shot.x += 8; shot.w -= 8; break;
            case WEST :  shot.w -= 8; break;
        }
        if (shot.w <= 0 || shot.h <= 0) {
            tileMap[row][col] = EMPTY;
        }
        bullet.reset();
    }
}


EnemyTank enemy[enemyMax];
const int MaxEnemy_bullet = 20;
Bullet enemyBullets[MaxEnemy_bullet];


void idivEnemy(int index, int x, int y){
    enemy[index].x = x;
    enemy[index].y = y;
    enemy[index].dx = 0;
    enemy[index].dy = 0;
    enemy[index].speed = 4;
    enemy[index].direction = SOUTH;
    enemy[index].alive = true;
    enemy[index].frameCounter = 0;
    enemy[index].sprite = {};
    enemy[index].sprite.setFramerange(2, 3);
    enemy[index].dx = 0;
    enemy[index].dy = enemy[index].speed;
    enemy[index].bulletIndex = index;
}

void updateEnemy(){
    for (int i = 0; i < enemyMax; ++i) {
        if (!enemy[i].alive) continue;

        EnemyTank& E = enemy[i];
        E.frameCounter++;
        E.sprite.tick();

        int nextX = E.x + E.dx;
        int nextY = E.y + E.dy;

        bool vacham = checkvacham(nextX, nextY);
        bool vung = (nextX < 0 || nextX + 32 > MAP_WIDTH || nextY < 0 || nextY + 32 > MAP_HEIGHT ||
                     (i <= 5 && nextY > MAP_HEIGHT / 2) || (i >= 6 && nextY < MAP_HEIGHT /2));
        bool checkCollision = checkvacham_enemyOther(i, nextX, nextY);
        if (vacham || vung || checkCollision) {
            int dirs[4] = {NORTH, SOUTH,EAST , WEST};
            for(int i = 3; i > 0; i--){
                int r = rand() % (i+1);
                swap(dirs[i], dirs[r]);
            }

            for (int tries = 0; tries < 4; tries++) {
                Direction newDir = (Direction)dirs[tries];
                int testDx = 0, testDy = 0;
                switch (newDir) {
                    case NORTH: testDy = -E.speed; break;
                    case SOUTH: testDy = E.speed; break;
                    case EAST:  testDx = E.speed; break;
                    case WEST:  testDx = -E.speed; break;
                }

                int testX = E.x + testDx;
                int testY = E.y + testDy;
                bool testVacham = checkvacham(testX, testY);
                bool testVung = (testX < 0 || testX + 32 > MAP_WIDTH || testY < 0 || testY + 32 > MAP_HEIGHT ||
                                 (i <= 5 && testY > MAP_HEIGHT / 2) || (i >= 6 && testY < MAP_HEIGHT / 2));

                if (!testVacham && !testVung) {
                    E.direction = newDir;
                    E.dx = testDx;
                    E.dy = testDy;
                    switch (newDir) {
                        case NORTH: E.sprite.setFramerange(0, 1); break;
                        case SOUTH: E.sprite.setFramerange(2, 3); break;
                        case WEST:  E.sprite.setFramerange(4, 5); break;
                        case EAST:  E.sprite.setFramerange(6, 7); break;
                    }
                    break;
                }
            }
            continue;
        }
        E.x = nextX;
        E.y = nextY;


        Bullet& shot = enemyBullets[E.bulletIndex];
        if (!shot.active) {
            int fireDx = 0, fireDy = 0;
            SDL_Texture* usedTexture = bulletTexture;

            switch (E.direction) {
                 case NORTH: fireDx = 0; fireDy = -13; usedTexture = bulletTexture; break;
                 case SOUTH: fireDx = 0; fireDy = 13; usedTexture = bulletTexture; break;
                 case EAST:  fireDx = 13; fireDy = 0; usedTexture = bulletTexture1; break;
                 case WEST:  fireDx = -13; fireDy = 0; usedTexture = bulletTexture1; break;
            }

            shot.x = E.x;
            shot.y = E.y;
            shot.dx = fireDx;
            shot.dy = fireDy;
            shot.speed = 13;
            shot.active = true;
            shot.texture = usedTexture;
        }
    }




}

void renderEnemy(Graphics& gfx){
    for(int i = 0; i < enemyMax; i++){
        if(enemy[i].alive && enemy[i].x < 800) gfx.renderSpriteCamera(enemy[i].x, enemy[i].y, enemy[i].sprite);

    }
}

void updateBullets(Bullet bullets[], int maxBullets) {
    for (int i = 0; i < maxBullets; i++) {
        if (!bullets[i].active) continue;
        bullets[i].move();
        BulletTile(bullets[i]);
    }
}

void renderBullets(Bullet bullets[], int maxBullets, Graphics& gfx) {
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].active) {
            bullets[i].render(gfx);
        }
    }
}

bool checkvacham_tank(const SDL_Rect& a, const SDL_Rect& b){
    return SDL_HasIntersection(&a, &b);
}

Tank player;
bool checkvacham_enemyOther(int id, int nextX, int nextY) {
    SDL_Rect rectA = {nextX, nextY, 32, 32};
    for (int i = 0; i < enemyMax; i++) {
        if (i == id || !enemy[i].alive) continue;
        SDL_Rect rectB = {enemy[i].x, enemy[i].y, 32, 32};
        if (checkvacham_tank(rectA, rectB)) return true;
    }
    SDL_Rect playerBox = {player.x, player.y, 32, 32};
    if (checkvacham_tank(rectA, playerBox)) return true;

    return false;
}


bool checkvacham_tankOther(int nextX, int nextY){
    SDL_Rect rectA = {nextX, nextY, 32, 32};

    for (int i = 0; i < enemyMax; ++i) {
        if (!enemy[i].alive) continue;
        SDL_Rect rectB = {enemy[i].x, enemy[i].y, 32, 32};
        if (checkvacham_tank(rectA, rectB)) return true;
    }

    return false;
}







void checkBulletEnemyCollision(Bullet& bullet) {
    if (!bullet.active) return;

    SDL_Rect bulletRect = {bullet.x, bullet.y, 32, 32};

    for (int i = 0; i < enemyMax; i++) {
        if (!enemy[i].alive) continue;

        SDL_Rect enemyRect = {enemy[i].x, enemy[i].y, 32, 32};
        if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
            enemy[i].alive = false;
            bullet.reset();
            break;
        }
    }
}

void checkEnemyBulletPlayerCollision() {
    SDL_Rect playerRect = {player.x, player.y, 32, 32};

    for (int i = 0; i < MaxEnemy_bullet; i++) {
        if (!enemyBullets[i].active) continue;

        SDL_Rect bulletRect = {enemyBullets[i].x, enemyBullets[i].y, 32, 32};
        if (SDL_HasIntersection(&bulletRect, &playerRect)) {
            handleTankHit();
            enemyBullets[i].reset();
            break;
        }
    }
}

void checkWaterCollision(){
    if(player.immotank) return;
    int tankX = player.x + 16;
    int tankY = player.y + 16;
    int col = tankX/TILE_SIZE;
    int row = tankY/TILE_SIZE;
    if(row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS){
        if(tileMap[row][col] == WATER){
            handleTankHit();
        }
    }
}

void resetPlayerPosition() {
    player.x = 384;
    player.y = 1184;
    player.dx = 0;
    player.dy = 0;
    player.setDirection(NORTH);
    player.immotank = true;
    player.immotankTimer = 120;
    camera.y = player.y - SCREEN_HEIGHT / 2 + 16;
    camera.y = std::max(0, std::min(camera.y, MAP_HEIGHT - SCREEN_HEIGHT));
}



LivesDisplay livesDisplay;
void LivesDisplay::init(SDL_Texture* tex){
    texture = tex;
    sprite.texture = texture;

    SDL_Rect clip;
    for(int i = 0; i < LIVE_FRAMES; i++){
        clip.x = LIVE_CLIPS[i][0];
        clip.y = LIVE_CLIPS[i][1];
        clip.w = LIVE_CLIPS[i][2];
        clip.h = LIVE_CLIPS[i][3];
        sprite.clips.push_back(clip);
    }
    sprite.currentFrame = 0;
}

void LivesDisplay::updateLives(int lives){
    sprite.currentFrame = 3 - lives;
    if(sprite.currentFrame < 0) sprite.currentFrame = 0;
    if(sprite.currentFrame >= LIVE_FRAMES) sprite.currentFrame = LIVE_FRAMES - 1;
}

void LivesDisplay::render(Graphics& gfx){
    gfx.render(SCREEN_WIDTH - 120, 10, sprite);
}

void handleTankHit(){
    if (player.immotank) return;
    player.lives--;
    livesDisplay.updateLives(player.lives);

    if (player.lives <= 0) {
        currentState = GAME_OVER;
    } else {
        resetPlayerPosition();
    }
}


bool gameOver(const Tank& player){
    // dieu kien cho xe dung sau nay
    return player.lives <= 0;
}
