#include "includes.h"

void handleKeyEvents(Tank& player, Bullet& bullet, SDL_Texture* bulletTexture, SDL_Texture* bulletTexture1){
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    bool moving = false;

    Direction newdir;
    if (currentKeyStates[SDL_SCANCODE_UP]){
        newdir = NORTH;
        moving = true;
    }
    else if (currentKeyStates[SDL_SCANCODE_DOWN]){
        newdir = SOUTH;
        moving = true;
    }
    else if (currentKeyStates[SDL_SCANCODE_LEFT]){
        newdir = WEST;
        moving = true;
    }
    else if (currentKeyStates[SDL_SCANCODE_RIGHT]){
        newdir = EAST;
        moving = true;
    }

    if(moving){
        if(player.direction != newdir){
            player.setDirection(newdir);
        }

        player.updateVelocity(newdir);
    }
    else{
        player.stop();
    }

    if(currentKeyStates[SDL_SCANCODE_SPACE]){
        if(!bullet.active){
            if(player.direction == NORTH  || player.direction == SOUTH){
                bullet.fire(player.x, player.y, player.direction, bulletTexture, bulletTexture1);
            }else{
                bullet.fire(player.x, player.y, player.direction, bulletTexture, bulletTexture1);
            }
        }

    }
}
void handleMouseEvents(SDL_Event &event, Tank &player) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    // Xử lý chọn menu
                    break;
                case SDL_BUTTON_RIGHT:
                    // Xử lý tùy chỉnh âm lượng
                    break;
            }
            break;
    }
}

void handleEvents(SDL_Event &event, bool &running, Tank &player, Bullet& bullet, SDL_Texture* bulletTexture, SDL_Texture* bulletTexture1) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        handleMouseEvents(event, player);
    }
    handleKeyEvents(player, bullet, bulletTexture, bulletTexture1);
}
