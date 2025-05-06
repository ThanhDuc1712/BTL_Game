#include "includes.h"

void Graphics::logErrorAndExit(const char* msg,const char* error){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
}

void Graphics::init(){
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == nullptr)
            logErrorAndExit("CreateWindow", SDL_GetError());

        if(!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit("SDL_image error:", IMG_GetError());
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            logErrorAndExit("SDL_mixer error:", Mix_GetError());
        if(TTF_Init() == -1)
            logErrorAndExit("SDL_ttf error:", TTF_GetError());

        renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(renderer == nullptr)
            logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Graphics::prepareScene(SDL_Texture* background){
        SDL_Rect tang = camera;
        SDL_RenderClear(renderer);
        SDL_Rect dest = {0, 0, 800, 608};
        SDL_RenderCopy(renderer, background, &tang, &dest);
}

void Graphics::presentScene(){
        SDL_RenderPresent(renderer);
}

SDL_Texture* Graphics::loadTexture(const char *filename){
    std::cout << "Loading texture: " << filename << std::endl;
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL) {
        std::cerr << "Error loading texture: " << filename << " - " << IMG_GetError() << std::endl;
    }
    return texture;
}

void Graphics::renderTexture(SDL_Texture *texture, int x, int y){
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
}
void Graphics::render(int x, int y, const Sprite& sprite) {
        const SDL_Rect* clip = sprite.getCurrentClip();
        SDL_Rect renderQuad = {x, y, clip->w, clip->h};
        SDL_RenderCopy(renderer, sprite.texture, clip, &renderQuad);
}

SDL_Texture* Graphics::renderText(const string& text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}


void Graphics::quit(){
        if(victoryMusic != nullptr){
            Mix_FreeMusic(victoryMusic);
            victoryMusic = nullptr;
        }
        if(scoreFont != nullptr){
            TTF_CloseFont(scoreFont);
            scoreFont = nullptr;
        }
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
}


void Graphics::renderTextureCamera(SDL_Texture* texture, int x, int y){
    SDL_Rect dest;
    dest.x = x;
    dest.y = y - camera.y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Graphics::renderSpriteCamera(int x, int y, const Sprite& sprite){
    const SDL_Rect* clip = sprite.getCurrentClip();
    SDL_Rect renderQuad = {x, y - camera.y, clip->w, clip->h};
    SDL_RenderCopy(renderer, sprite.texture, clip, &renderQuad);
}

void Sprite::init(SDL_Texture* _texture, int frames, const int _clips [][4]) {
        texture = _texture;
        SDL_Rect clip;
        for (int i = 0; i < frames; i++) {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.push_back(clip);
        }
}

void Sprite::tick() {
    if (startFrame == endFrame) return;

    currentFrame++;
    if (currentFrame > endFrame) {
        currentFrame = startFrame;
    }
}

const SDL_Rect* Sprite::getCurrentClip() const {
    return &(clips[currentFrame]);
}

void Sprite::setFramerange(int start, int end){
    startFrame = start;
    endFrame = end;
    currentFrame = start;

}











