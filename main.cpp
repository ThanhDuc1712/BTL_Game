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

int main(int argc, char* argv[]) {
    Graphics graphics;

    graphics.init();

    SDL_Texture* background = graphics.loadTexture("huhu.png");
    SDL_Texture* tankTexture = graphics.loadTexture(TANK_SPRITE_FILE);
    SDL_Texture* bulletTexture = graphics.loadTexture("Bullet.png");
    SDL_Texture* bulletTexture1 = graphics.loadTexture("Bullet1.png");

    loadTileTextures(graphics);
    loadTileMap("tilemap1.txt");

    Bullet bullet;
    Tank player;
    Sprite tankSprite ;
    tankSprite.init(tankTexture, TANK_FRAMES, TANK_CLIPS);
    player.sprite = tankSprite;




    bool running = true;
    SDL_Event event;


    while (running) {
        handleEvents(event, running, player, bullet, bulletTexture, bulletTexture1);
        player.move();
        bullet.move();
        BulletTile(bullet);


        graphics.prepareScene(background);

        renderTileMap(graphics,false);
        graphics.renderSpriteCamera(player.x, player.y, player.sprite);
        bullet.render(graphics);
        renderTileMap(graphics,true);

        graphics.presentScene();

        SDL_Delay(1000/24);
    }

    graphics.quit();
    return 0;
}









