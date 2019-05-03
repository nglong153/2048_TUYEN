#include "grid.h"
#include "DrawColor.h"
#include "Display.h"
#include <math.h>
GRID::GRID(){score = 0; color = BLANK_SQUARE_COLOR;}

void GRID::random(){
    score = (rand() % 5 == 0 ? 4 : 2);
    color = iColor[(int)log2(score)];
}


GRID operator+(GRID a, GRID b){
    GRID ans;
    ans.score = a.score + b.score;
    ans.color = iColor[(int)log2(ans.score)];
    return ans;
}
bool operator==(GRID a, GRID b){
    return (a.score == b.score);
}
bool operator!(GRID A){
    return (A.score != 0);
}
// #undef main
// int main(){
//     init();
//     loadMedia();
//     GRID A;
//     A.score = 5;
//     A.color = BLACK;
//     mTexture.loadText(intoString(A.score), A.color);
//     SDL_Delay(1000);
//     PresentRender();
//     close();
// }
