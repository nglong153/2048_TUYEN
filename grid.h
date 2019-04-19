#ifndef grid_h
#define grid_h

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <iomanip>
#include <conio.h>
#include <windows.h>
struct GRID{
    int score;
    SDL_Color color;
    GRID();
    void random();
};

GRID operator+(GRID a, GRID b);
bool operator==(GRID a, GRID b);
bool operator!(GRID A);
#endif