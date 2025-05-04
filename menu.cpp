#include "includes.h"

GameFunc currentState = MENU;
Menu gameMenu;

void Menu::init(Graphics& gfx){
    startButton.texture = gfx.loadTexture("startbutton.png");
    startButton.rect = {608,320,160, 64};
    startButton.isHovered = false;

    backgroundTexture = gfx.loadTexture("TANK _1975.png");
}

void Menu::render(Graphics& gfx){
    if(backgroundTexture){
        gfx.renderTexture(backgroundTexture, 0, 0);
    }else{
        SDL_SetRenderDrawColor(gfx.renderer,0,0,0,255);
        SDL_RenderClear(gfx.renderer);
    }


    if(startButton.texture){
        SDL_Rect Rect_render = startButton.rect;
        if(startButton.isHovered){
            Rect_render.x -= 5;
            Rect_render.y -= 5;
            Rect_render.w += 10;
            Rect_render.h += 10;
        }
        SDL_RenderCopy(gfx.renderer, startButton.texture, NULL, &Rect_render);
    }
}

void Menu::update(int mouseX, int mouseY){
    startButton.isHovered = startButton.isMouseOver(mouseX, mouseY);
}
bool Menu::handleClick(int mouseX, int mouseY){
    if(startButton.isMouseOver(mouseX, mouseY)) return true;
    return false;
}




void Menu::cleanup() {
    SDL_DestroyTexture(startButton.texture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(titleTexture);

    startButton.texture = nullptr;
    backgroundTexture = nullptr;
    titleTexture = nullptr;
}
