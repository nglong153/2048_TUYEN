#ifndef Display_h
#define Display_h

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "grid.h"
#include "button.h"
#include <string>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 550;
const int xl = 120;
const int xr = 520;
const int yl = 80;
const int yr = 480;
const int squareWidth = 100;
const int N = 7;
void setFont(int v);
void init();
void PresentRender();
void loadMedia();
void close();
void ScreenForLoser(int score);
void fillRectangle(int x,int y, int width, int height, SDL_Color gColor);
void fillRectangleBackground(SDL_Rect clip, SDL_Color gColor);
void drawButton(LButton e,SDL_Color textColor, SDL_Color backgroundColor);
string intoString(int score);
void display(int score,GRID grid[N][N],LButton* newGame,LButton* newMode,LButton* Save, LButton* Load);
class LTexture{
    public:
        // Init
        LTexture();
        // Deallocates memory
        ~LTexture();
        bool loadText(string text,SDL_Color gColor);
        // Deallocates Texture;
        void free();
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        //void render(string text, int x,int y);
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        void normalRender( int x,int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        int getWidth();
        int getHeight();
    private:
        int pWidth, pHeight;
        SDL_Texture* pTexture;
};

#endif
