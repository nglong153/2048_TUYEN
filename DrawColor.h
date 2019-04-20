#ifndef DrawColor_h
#define DrawColor_h
#include "grid.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <math.h>
#include <time.h>
using namespace std;
const SDL_Color WHITE = {255, 255, 255};
const SDL_Color BLACK = {0,0,0};
const SDL_Color BLANK_SQUARE_COLOR = {205, 193, 181};
const SDL_Color OUTLINE_COLOR = {186, 172, 163};
const SDL_Color BACKGROUND_BUTTON = {143,118,96};
const SDL_Color SWHITE = {250, 247 , 238};
const SDL_Color iColor[13] = {
    BLACK,
    {238,238,218},
    {237,224,200},
    {242,177,121},
    {245,149,99},
    {246,124,95},
    {246,94,59},
    {237,207,114},
    {237,204,97},
    {237,200,80},
    {237,197,63},
    {122,0,98},    
};
SDL_Color randomColor();
int colorDistance(SDL_Color colorA, SDL_Color colorB);
SDL_Color getTextColor(GRID a);
SDL_Color reverseColor(SDL_Color a);
SDL_Color colorMix(SDL_Color A, SDL_Color B);

#endif