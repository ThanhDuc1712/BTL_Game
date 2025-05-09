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
    gameMenu.loadHighScore();
    GOverScreen.init(graphics);
    victoryScreen.init(graphics);
    victoryScreen.MusicPlaying = false;
    scoreFont = TTF_OpenFont(SCORE_FONT_FILE, 35);

    SDL_Texture* livesTexture = graphics.loadTexture(LIVE_SPRITE_FILE);
    SDL_Texture* background = graphics.loadTexture("hehe.png");
    SDL_Texture* tankTexture = graphics.loadTexture(TANK_SPRITE_FILE);
    bulletTexture = graphics.loadTexture("Bullet.png");
    bulletTexture1 = graphics.loadTexture("Bullet1.png");
    SDL_Texture* enemyTexture = graphics.loadTexture(ENEMY_SPRITE_FILE);
    SDL_Texture* congTexture = graphics.loadTexture("Cong.png");

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
    dkVictory.init(congTexture);

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

            dkVictory.update(player);
            victoryScreen.updateScoreDisplay(graphics);


            graphics.prepareScene(background);
            renderTileMap(graphics,false);
            dkVictory.render(graphics);
            graphics.renderSpriteCamera(player.x, player.y, player.sprite);
            bullet.render(graphics);
            renderEnemy(graphics);
            renderBullets(enemyBullets, MaxEnemy_bullet, graphics);
            renderTileMap(graphics,true);
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
        else if(currentState == VICTORY){
            SDL_RenderClear(graphics.renderer);
            victoryScreen.render(graphics);
        }

        graphics.presentScene();
        SDL_Delay(1000/24);
    }


    gameMenu.cleanup();
    GOverScreen.cleanup();
    victoryScreen.cleanup();
    graphics.quit();
    return 0;
}








