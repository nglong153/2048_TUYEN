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
    SDL_Rect clip = {mPosition.x, mPosition.y,buttonWidth, buttonHeight};
    fillRectangleBackground(clip,BACKGROUND_BUTTON);
    // draw button
    // draw button content.
    gText.normalRender(mPosition.x, mPosition.y);
}
// Handle event E
void LButton::handleEvent(SDL_Event* e,void (*restart)()){
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP){
        int x,y;
        SDL_GetMouseState(&x, &y);
        if (e->type == SDL_MOUSEMOTION){
            if (insideButton(x,y)) { // check if the mouse is inside the button
                setFont(20);
                drawButton(BACKGROUND_BUTTON, SWHITE); // change button color.
                setFont(30);
                PresentRender();
            }
            else {
                setFont(20);
                drawButton(WHITE, BACKGROUND_BUTTON); // change back
                setFont(30);
                PresentRender();
            } 
        }
        if (insideButton(x,y) && e->type == SDL_MOUSEBUTTONUP)  restart(); // implement button content when click.
    }
}