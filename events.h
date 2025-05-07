#ifndef EVENTS_H
#define EVENTS_H

#include "includes.h"


struct Tank;
struct Bullet;

void handleKeyEvents(Tank& player, Bullet& bullet, SDL_Texture* bulletTexture, SDL_Texture* bulletTexture1);
void handleMouseEvents(SDL_Event &event, int mouseX, int mouseY);
void handleEvents(SDL_Event &event, bool &running, Tank &player, Bullet& bullet, SDL_Texture* bulletTexture, SDL_Texture* bulletTexture1);




#endif
