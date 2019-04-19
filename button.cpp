#include "button.h"
#include "Display.h"
#include "DrawColor.h"
#include "main.h"
#undef main

LButton::LButton(){
    mPosition.x = 0;
    mPosition.y = 0;
    buttonHeight = buttonWidth = 0;
    text = "";
}
int LButton::getButtonWidth(){return buttonWidth;}
int LButton::getButtonHeight(){return buttonHeight;}
string LButton::getButtonText(){return text;}
void LButton::set(int x,int y){buttonWidth = x; buttonHeight = y; }
SDL_Point LButton::getPosition() { return mPosition; }
void LButton::initButton(int x,int y, string s){
    mPosition.x = x;
    mPosition.y = y;
    text = s;
}
int LButton::insideButton(int x,int y){
    int xl = mPosition.x, yl = mPosition.y;
    return (x >= xl && x <= xl + buttonWidth && y >= yl && y <= yl + buttonHeight);
}
LTexture gText;
void LButton::drawButton(SDL_Color textColor, SDL_Color backgroundColor){
    // load text
    gText.loadText(text, textColor);
    // load button screen width/height
    set(gText.getWidth() + 2, gText.getHeight() + 2);
    fillRectangle(mPosition.x,mPosition.y,buttonWidth, buttonHeight, backgroundColor);
    // draw button
    // draw button content.
    gText.normalRender(mPosition.x, mPosition.y);
}
void LButton::handleEvent(SDL_Event* e){
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP){
        int x,y;
        SDL_GetMouseState(&x, &y);
        if (e->type == SDL_MOUSEMOTION){
            if (insideButton(x,y)) {
                setFont(20);
                drawButton(BACKGROUND_BUTTON, SWHITE);
                setFont(30);
                PresentRender();
            }
            else {
                setFont(20);
                drawButton(WHITE, BACKGROUND_BUTTON);
                setFont(30);
                PresentRender();
            } 
        }
        if (insideButton(x,y) && e->type == SDL_MOUSEBUTTONUP)  restart();
    }
}