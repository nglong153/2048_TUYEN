#ifndef button_h
#define button_h
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;
class LButton
{
    public:
        LButton();
        void initButton(int x,int y,string s);
        void set(int x,int y);
        void handleEvent(SDL_Event* e);
        int insideButton(int x,int y);
        int getButtonWidth();
        int getButtonHeight();
        string getButtonText();
        SDL_Point getPosition();
        void drawButton(SDL_Color textColor, SDL_Color backgroundColor);
    private:
        SDL_Point mPosition;
        int buttonHeight, buttonWidth;
        string text;
};

#endif