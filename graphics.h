#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include "includes.h"

struct Sprite {
    int startFrame = 0;
    int endFrame = 0;
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;
    void init(SDL_Texture* _texture, int frames, const int _clips [][4]);
    void tick();
    const SDL_Rect* getCurrentClip() const;
    void setFramerange(int start, int e_nd);
};


struct Graphics {
    SDL_Renderer *renderer;
    SDL_Window *window;

    void logErrorAndExit(const char* msg, const char* error);
    void init();
    void prepareScene(SDL_Texture* background);
    void presentScene();
    SDL_Texture *loadTexture(const char *filename);
    void renderTexture(SDL_Texture *texture, int x, int y);
    void render(int x, int y, const Sprite& sprite);
    SDL_Texture* renderText(const std::string& text, TTF_Font* font, SDL_Color color);

    void renderTextureCamera(SDL_Texture* texture, int x, int y);
    void renderSpriteCamera(int x, int y, const Sprite& sprite);
    void quit();
};




#endif
