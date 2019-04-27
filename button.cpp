#include "button.h"
#include "Display.h"
#include "DrawColor.h"
#include "main.h"
#undef main

LButton::LButton(){
    mPosition.x = 0;
    mPosition.y = 0;
    currentSprite = 1; 
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

void LButton::drawButton(){
    // load text
    LTexture gText;
    SDL_Color textColor, backgroundColor;
    if (!currentSprite) textColor = WHITE, backgroundColor = BACKGROUND_BUTTON;
    else backgroundColor = WHITE, textColor = BACKGROUND_BUTTON;
    setFont(20);
    gText.loadText(text, textColor);
    setFont(30);
    // load button screen width/height
    set(gText.getWidth() + 2, gText.getHeight() + 2);
    fillRectangle(mPosition.x,mPosition.y,buttonWidth, buttonHeight, backgroundColor);
    SDL_Rect clip = {mPosition.x, mPosition.y,buttonWidth, buttonHeight};
    fillRectangleBackground(clip,BACKGROUND_BUTTON);
    // draw button
    // draw button content.
    gText.normalRender(mPosition.x, mPosition.y);
    gText.free();
}
// Handle event E
void LButton::handleEvent(SDL_Event* e,void (*restart)()){
    int x,y;
    SDL_GetMouseState(&x, &y);
        if (e->type == SDL_MOUSEMOTION){
            if (insideButton(x,y)) { // check if the mouse is inside the button
                currentSprite = 0; // change button color.
            }
            else {
                currentSprite = 1; // change back
            } 
        }
        else if (insideButton(x,y))  restart(); // implement button content when click.
}