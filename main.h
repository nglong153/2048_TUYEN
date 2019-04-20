#ifndef main_h
#define main_h
#include "DrawColor.h"
#include "SDL.h"
#include "SDL_image.h"
void initController();
void reverse();
void restart();
int getMode();
int maxBoard();
void random();
void left();
void right();
void up();
void down();
bool ok();
bool loseCondition();
bool compare();
void implement(SDL_Event e);
void implementPS4(SDL_Event e);
void game();
#endif;