#include "includes.h"

GameFunc currentState = MENU;
Menu gameMenu;
VictoryScreen victoryScreen;

void Menu::init(Graphics& gfx){
    startButton.texture = gfx.loadTexture("startbutton.png");
    startButton.rect = {608,320,160, 64};
    startButton.isHovered = false;

    backgroundTexture = gfx.loadTexture("TANK _1975.png");

    loadHighScore();
    gameMenu.loadHighScore();


}

void Menu::render(Graphics& gfx){
    if(backgroundTexture) gfx.renderTexture(backgroundTexture, 0, 0);

    if(highScoreTexture) {
        int scoreWidth, scoreHeight;
        SDL_QueryTexture(highScoreTexture, NULL, NULL, &scoreWidth, &scoreHeight);
        SDL_Rect scoreRect = {544, 512, scoreWidth, scoreHeight};
        SDL_RenderCopy(gfx.renderer, highScoreTexture, NULL, &scoreRect);
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
    string highScoreText = "HIGH SCORE: " + to_string(High_Score);
    SDL_Surface* highscoreSur = TTF_RenderText_Solid(scoreFont,highScoreText.c_str() , {0,0,0,255});
    SDL_Texture* highscoreTex = SDL_CreateTextureFromSurface(gfx.renderer, highscoreSur);
    int scoreWidth, scoreHeight;
    SDL_QueryTexture(highscoreTex, NULL, NULL, &scoreWidth, &scoreHeight);
    SDL_Rect scoreRect = {544, 512, scoreWidth, scoreHeight};
    SDL_RenderCopy(gfx.renderer, highscoreTex, NULL, &scoreRect);
    SDL_FreeSurface(highscoreSur);
    SDL_DestroyTexture(highscoreTex);

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

void Menu::loadHighScore() {
    ifstream file("High Score.txt");
    if(file.is_open()) {
        file >> High_Score;
        file.close();
    } else {
        High_Score = 0;
    }
}

void Menu::saveHighScore() {
    ofstream file("High Score.txt");
    if(file.is_open()) {
        file << High_Score;
        file.close();
    }
}


void Menu::cleanup() {
    SDL_DestroyTexture(startButton.texture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(titleTexture);

     if(highScoreTexture != nullptr) {
        SDL_DestroyTexture(highScoreTexture);
        highScoreTexture = nullptr;
    }

    startButton.texture = nullptr;
    backgroundTexture = nullptr;
    titleTexture = nullptr;
}

GameoverScreen GOverScreen;
void GameoverScreen::init(Graphics& gfx){
    this->gfx = &gfx;
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
        if(playerScore > High_Score) {
           High_Score = playerScore;
           gameMenu.saveHighScore();
        }
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

void VictoryScreen::init(Graphics& gfx){
    this->gfx = &gfx;
    backgroundTexture = gfx.loadTexture("VICTORY.png");
    returnButton.texture = gfx.loadTexture("returnbutton.png");
    returnButton.rect = {736, 544, 192, 64};
    returnButton.isHovered = false;
    MusicPlaying = false;
    if(victoryMusic == nullptr) victoryMusic = Mix_LoadMUS(VICTORY_MUSIC_FILE);
    scoreTexture = nullptr;


    updateScoreDisplay(gfx);
}

void VictoryScreen::render(Graphics& gfx){
    if(backgroundTexture) gfx.renderTexture(backgroundTexture, 0, 0);

    if(scoreTexture) {
        int scoreWidth, scoreHeight;
        SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreWidth, &scoreHeight);
        SDL_Rect scoreRect = {16, 10, scoreWidth, scoreHeight};
        SDL_RenderCopy(gfx.renderer, scoreTexture, NULL, &scoreRect);
    }

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

    if(!MusicPlaying) playVictoryMusic();
}

void VictoryScreen::update(int mouseX, int mouseY){
    returnButton.isHovered = returnButton.isMouseOver(mouseX, mouseY);
}

bool VictoryScreen::handleClick(int mouseX, int mouseY){
    if(returnButton.isMouseOver(mouseX,mouseY)){
        if(playerScore > High_Score) {
            High_Score = playerScore;
            gameMenu.saveHighScore();
        }

        stopVictoryMusic();
        resetGame();
        currentState = MENU;
        return true;
    }
    return false;
}

void VictoryScreen::playVictoryMusic(){
    if(victoryMusic != nullptr && !MusicPlaying){
        Mix_PlayMusic(victoryMusic, -1);
        MusicPlaying = true;
    }
}

void VictoryScreen::stopVictoryMusic(){
    if(MusicPlaying){
        Mix_HaltMusic();
        MusicPlaying = false;
    }
}

void VictoryScreen::updateScoreDisplay(Graphics& gfx){
    if(scoreTexture != nullptr) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;
    }
    if(scoreFont != nullptr) {
        SDL_Color scoreColor = {0, 0, 0, 255};
        string scoreText = "SCORE: " + to_string(playerScore);
        scoreTexture = gfx.renderText(scoreText, scoreFont, scoreColor);
    }
}

void VictoryScreen::cleanup() {
    stopVictoryMusic();

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(returnButton.texture);
    if(scoreTexture != nullptr) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;
    }
    backgroundTexture = nullptr;
    returnButton.texture = nullptr;
}

