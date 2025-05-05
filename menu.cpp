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
    if(backgroundTexture) gfx.renderTexture(backgroundTexture, 0, 0);
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
    if(startButton.isMouseOver(mouseX, mouseY)){
        resetGame();
        currentState = PLAYING;
        return true;
    }
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

GameoverScreen GOverScreen;
void GameoverScreen::init(Graphics& gfx){
    backgroundTexture = gfx.loadTexture("GAME OVER.png");
    returnButton.texture = gfx.loadTexture("returnbutton.png");
    returnButton.rect = {736, 544, 192, 64};
    returnButton.isHovered = false;
}

void GameoverScreen::render(Graphics& gfx){
    if(backgroundTexture) gfx.renderTexture(backgroundTexture, 0, 0);
    if(returnButton.texture){
        SDL_Rect rectRender = returnButton.rect;
        if(returnButton.isHovered){
            rectRender.x -= 5;
            rectRender.y -= 5;
            rectRender.w += 10;
            rectRender.h += 10;
        }
        SDL_RenderCopy(gfx.renderer, returnButton.texture, NULL, &rectRender);
    }
}

void GameoverScreen::update(int mouseX, int mouseY){
    returnButton.isHovered = returnButton.isMouseOver(mouseX, mouseY);
}

bool GameoverScreen::handleClick(int mouseX, int mouseY){
    if(returnButton.isMouseOver(mouseX, mouseY)){
        resetGame();
        currentState = MENU;
        return true;
    }
    return false;
}

void GameoverScreen::cleanup(){
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(returnButton.texture);
    backgroundTexture = nullptr;
    returnButton.texture = nullptr;
}
