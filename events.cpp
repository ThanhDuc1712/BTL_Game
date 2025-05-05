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
void handleMouseEvents(SDL_Event &event, int mouseX, int mouseY) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    // Xử lý chọn menu
                    if(currentState == MENU){
                        if(gameMenu.handleClick(mouseX, mouseY)){
                            currentState = PLAYING;
                        }
                    }else if(currentState == GAME_OVER){
                        GOverScreen.handleClick(mouseX, mouseY);
                    }
                    break;
                case SDL_BUTTON_RIGHT:
                    // Xử lý tùy chỉnh âm lượng
                    break;
            }
            break;
                case SDL_MOUSEMOTION:
                    if(currentState == MENU){
                        gameMenu.update(mouseX, mouseY);
                    }else if(currentState == GAME_OVER){
                        GOverScreen.update(mouseX, mouseY);
                    } break;
    }
}

void handleEvents(SDL_Event &event, bool &running, Tank &player, Bullet& bullet, SDL_Texture* bulletTexture, SDL_Texture* bulletTexture1) {
    int mouseX = 0, mouseY = 0;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        handleMouseEvents(event, mouseX, mouseY);
    }
    handleKeyEvents(player, bullet, bulletTexture, bulletTexture1);
}
