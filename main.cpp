#include "includes.h"
#include <direct.h>

void printCurrentDir() {
    char buffer[256];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        printf("Current working directory: %s\n", buffer);
    } else {
        perror("getcwd() error");
    }
}

SDL_Texture* bulletTexture = nullptr;
SDL_Texture* bulletTexture1 = nullptr;


int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();

    gameMenu.init(graphics);
    GOverScreen.init(graphics);
    SDL_Texture* livesTexture = graphics.loadTexture(LIVE_SPRITE_FILE);
    SDL_Texture* background = graphics.loadTexture("hehe.png");
    SDL_Texture* tankTexture = graphics.loadTexture(TANK_SPRITE_FILE);
    bulletTexture = graphics.loadTexture("Bullet.png");
    bulletTexture1 = graphics.loadTexture("Bullet1.png");
    SDL_Texture* enemyTexture = graphics.loadTexture(ENEMY_SPRITE_FILE);

    for (int i = 0; i < enemyMax; ++i) {
        idivEnemy(i, spawnPos[i][0], spawnPos[i][1]);
        enemy[i].sprite.init(enemyTexture, TANK_FRAMES, TANK_CLIPS);
        enemy[i].sprite.setFramerange(2, 3);
    }

    loadTileTextures(graphics);
    loadTileMap("tilemap1.txt");

    Bullet bullet;
    Sprite tankSprite ;
    tankSprite.init(tankTexture, TANK_FRAMES, TANK_CLIPS);
    player.sprite = tankSprite;
    livesDisplay.init(livesTexture);
    livesDisplay.updateLives(player.lives);

    bool running = true;
    SDL_Event event;
    srand(time(NULL));

    while (running) {
        handleEvents(event, running, player, bullet, bulletTexture, bulletTexture1);
        if(currentState == PLAYING){
            player.move();
            bullet.move();
            BulletTile(bullet);
            checkBulletEnemyCollision(bullet);
            checkEnemyBulletPlayerCollision();
            updateEnemy();
            updateBullets(enemyBullets, MaxEnemy_bullet);


            graphics.prepareScene(background);
            renderTileMap(graphics,false);
            graphics.renderSpriteCamera(player.x, player.y, player.sprite);
            bullet.render(graphics);
            renderTileMap(graphics,true);
            renderEnemy(graphics);
            renderBullets(enemyBullets, MaxEnemy_bullet, graphics);
            livesDisplay.render(graphics);
        }
        else if (currentState == MENU) {
            SDL_RenderClear(graphics.renderer);
            gameMenu.render(graphics);
        }
        else if (currentState == GAME_OVER) {
            SDL_RenderClear(graphics.renderer);
            GOverScreen.render(graphics);
        }

        graphics.presentScene();
        SDL_Delay(1000/24);
    }

    gameMenu.cleanup();
    GOverScreen.cleanup();
    graphics.quit();
    return 0;
}








