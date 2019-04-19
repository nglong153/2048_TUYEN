#include "DrawColor.h"
#include "display.h"
SDL_Color randomColor(){
    return {rand()%256, rand()%256, rand()%256};
}
int colorDistance(SDL_Color colorA, SDL_Color colorB){
    return (abs(colorA.r - colorB.r) + abs(colorA.g - colorB.g) + abs(colorA.b - colorB.b));
}
SDL_Color getTextColor(int score){
    if (score <=4) return {119,110,101};
    return {255,255,255};
    //return (colorDistance(A, BLACK) > colorDistance(A, WHITE) ? BLACK : WHITE);
}
SDL_Color reverseColor(SDL_Color a){
    return {255 - a.r, 255 - a.g, 255 - a.b};
}
SDL_Color colorMix(SDL_Color A, SDL_Color B){
    return {(A.r + B.r)/2, (A.g + B.g)/2 , (A.b + B.b)/2};
}
