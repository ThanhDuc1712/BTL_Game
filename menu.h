#ifndef MENU_H
#define MENU_H

#include "includes.h"

enum GameFunc {MENU, PLAYING, GAME_OVER, VICTORY};

struct Button{
    SDL_Rect rect;
    SDL_Texture* texture;
    bool isHovered;
    bool isMouseOver(int mouseX, int mouseY) {
        return (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }
};

struct Menu{
    Button startButton;
    SDL_Texture* backgroundTexture;
    SDL_Texture* titleTexture;
    SDL_Texture* highScoreTexture;
    void init(Graphics& gfx);
    void render(Graphics& gfx);
    void update(int mouseX, int mouseY);
    bool handleClick(int mouseX, int mouseY);
    void loadHighScore();
    void saveHighScore();
    void cleanup();

};

extern GameFunc currentState;
extern Menu gameMenu;

struct GameoverScreen{
    SDL_Texture* backgroundTexture;
    Button returnButton;
    Graphics* gfx;
    void init(Graphics& gfx);
    void render(Graphics& gfx);
    void update(int mouseX, int mouseY);
    bool handleClick(int mouseX, int mouseY);
    void cleanup();

};
extern GameoverScreen GOverScreen;

struct VictoryScreen {
    SDL_Texture* backgroundTexture;
    Button returnButton;
    Graphics* gfx;
    bool MusicPlaying;
    void init(Graphics& gfx);
    void render(Graphics& gfx);
    void update(int mouseX, int mouseY);
    bool handleClick(int mouseX, int mouseY);
    void cleanup();

    SDL_Texture* scoreTexture;
    void updateScoreDisplay(Graphics& gfx);
    void playVictoryMusic();
    void stopVictoryMusic();
};
extern VictoryScreen victoryScreen;


#endif
