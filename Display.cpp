#include "Display.h"
#include "DrawColor.h"
#include "button.h"
#include <time.h>
#include <math.h>
#include <stdio.h>
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
SDL_Color gColor;
LTexture gTextTexture;

using namespace std;
void setFont(int v){
    // Deallocate close previous font
    TTF_CloseFont(gFont);gFont = NULL;
    gFont = TTF_OpenFont("ClearSans-Bold.ttf", v);
}

void fillRectangle(int x,int y, int width, int height, SDL_Color gColor){
    SDL_Rect fillRect = {x,y, width, height};
    // SDL_SetRenderDrawColor(gRenderer,rand()%256, rand()%256, rand() % 256, 0xFF);
    SDL_SetRenderDrawColor(gRenderer,gColor.r, gColor.g, gColor.b, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}
void fillRectangleBackground(SDL_Rect clip, SDL_Color gColor){
    SDL_SetRenderDrawColor(gRenderer, gColor.r , gColor.g, gColor.b, 0XFF);
    SDL_RenderDrawLine(gRenderer, clip.x, clip.y, clip.x + clip.w, clip.y);
    SDL_RenderDrawLine(gRenderer, clip.x, clip.y + clip.h, clip.x + clip.w, clip.y + clip.h);
    SDL_RenderDrawLine(gRenderer, clip.x, clip.y, clip.x, clip.y + clip.h);
    SDL_RenderDrawLine(gRenderer, clip.x + clip.w, clip.y, clip.x + clip.w, clip.y + clip.h);
}
void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    gWindow = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    // Assign for print
};
void loadMedia(){
    gFont = TTF_OpenFont("ClearSans-Bold.ttf", 30);
    SDL_Color gColor = {255, 0, 0};
}
void PresentRender(){ SDL_RenderPresent(gRenderer);}
void close(){
    SDL_DestroyRenderer(gRenderer); gRenderer = NULL;
    SDL_DestroyWindow( gWindow ); gWindow = NULL;
    TTF_CloseFont(gFont);   gFont = NULL;
    gTextTexture.free();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

//void DrawShape(){
    // Clear Screen
//}
LTexture::LTexture(){
    pTexture = NULL;
    pWidth = 0;
    pHeight = 0;
}
LTexture::~LTexture(){
    free();
}
void LTexture::free(){
    if (pTexture != NULL){
        SDL_DestroyTexture(pTexture); pTexture = NULL;
        pWidth = 0;
        pHeight = 0;
    }
}
int LTexture::getWidth(){
    return pWidth;
}
int LTexture::getHeight(){
    return pHeight;
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod(pTexture, red, green, blue);
}
/*void LTexture::render(string text,int x,int y){
    SDL_Rect scrRect, desRect;
    TTF_SizeText(gFont, text.c_str(), &scrRect.w, &scrRect.h);
    scrRect.x = 0;
    scrRect.y = 0;
    desRect.x = x;
    desRect.y = y;
    desRect.w = desRect.h = squareWidth;
    SDL_RenderCopy(gRenderer, pTexture, &scrRect, &desRect);
}*/
void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
    int newX = x + (squareWidth-pWidth)/2, newY = y + (squareWidth - pHeight)/2;
	SDL_Rect renderQuad = { newX,newY, pWidth, pHeight};

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = pWidth;
		renderQuad.h = pHeight;
        renderQuad.x = x + (clip->w - pWidth)/2;
        renderQuad.y = y + (clip->h - pHeight)/2;
        //fillRectangle(renderQuad.x, renderQuad.y, pWidth, pHeight, BLANK_SQUARE_COLOR);
        printf("x:%d width:%d y:%d height:%d\n", renderQuad.x,pWidth,renderQuad.y, pHeight);
	}
	//Render to screen
	SDL_RenderCopyEx( gRenderer, pTexture, NULL, &renderQuad, angle, center, flip );
}
// Render without caring about the position in desRect.
void LTexture::normalRender(int x,int y,  SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ){
    SDL_Rect renderQuad = {x, y, pWidth, pHeight};
    if( clip != NULL ){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx( gRenderer, pTexture, clip, &renderQuad, angle, center, flip );
}

bool LTexture::loadText(string text, SDL_Color gColor){
    free();
    SDL_Surface* tempSurface = TTF_RenderText_Blended(gFont, text.c_str(), gColor);
    pTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
    pWidth = tempSurface->w; pHeight = tempSurface->h;
    SDL_FreeSurface(tempSurface);
    return pTexture != NULL;
}
string intoString(int score){
	string ans;
	while (score >= 10) {
		ans = (char)(score%10+'0') + ans;
		score/=10;
	}
	ans = (char) (score + '0') + ans;
	return ans;
}
// void Background(){

//     for(int i=1;i<=4;++i) for(int j=1;j<=4;++j) {
//         int x = xl + (j-1) * squareWidth;
//         int y = yl + (i-1) * squareWidth;
//     }
// }
void display(int score, GRID grid[N][N], LButton* newGame,LButton* newMode, LButton* Save, LButton*Load){
    //DrawShape();
    // Clear
    SDL_SetRenderDrawColor(gRenderer,255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    gTextTexture.free();
    //fillRectangle(xl-5, yl-5, squareWidth*4+10, squareWidth*4+10, OUTLINE_COLOR);
    setFont(30);
    // Draw background for the board.
    gColor = {0,0,0};
    // Draw scoreline
    string text = "Score:" + intoString(score);
    gTextTexture.loadText(text, {255,255,255});
    fillRectangle(209, 0, 201, 50, {187,173,160}); // the Font for scoreline.

    SDL_Rect clip = {0,0,SCREEN_WIDTH,gTextTexture.getHeight() + 4};
    gTextTexture.render(0,0,&clip);
    // Draw nemGame button
	newGame->drawButton();
    // Draw newMode button
	newMode->drawButton();
    //Draw Save button
    Save->drawButton();
    //Draw Load button
    Load->drawButton();
    for(int i=1;i<=4;++i){
        for(int j=1;j<=4;++j){
            int x = xl + (j-1) * squareWidth;
            int y = yl + (i-1) * squareWidth;
            fillRectangle(x + 5, y + 5, squareWidth-10, squareWidth-10, grid[i][j].color);
            if (grid[i][j].score) {
                string text = intoString(grid[i][j].score);
                //gColor = {log2(grid[i][j])*30,log2(grid[i][j])*30,log2(grid[i][j])*30};
                gTextTexture.loadText(text, getTextColor(grid[i][j]));
                gTextTexture.render(x,y);
            }
        }
    }
    //SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderPresent(gRenderer);
}
void ScreenForLoser(int score){
    //fillRectangle(xl,yl,squareWidth*4,{212,200,189});
    //SDL_RenderClear(gRenderer);
    // SDL_SetRenderDrawColor(gRenderer,rand()%256, rand()%256, rand() % 256, 0xFF);
    setFont(30);
    gTextTexture.loadText("Game Over", {150,150,0});
    gTextTexture.normalRender(250, 250);
    // string text = "Your score is:" + intoString(score);
    // gTextTexture.loadText(text, {150,150,0});
    // gTextTexture.normalRender(200, 240);
    SDL_RenderPresent(gRenderer);
}
// int main(int argc, char *argv[]){
//     init();
//     loadMedia();
//     SDL_RenderClear(gRenderer);
//     SDL_SetRenderDrawColor(gRenderer,255,255,255,0);
//     setFont(20);
//     gTextTexture.loadText("1000000", {0,0,0});
//     fillRectangle(0,0,50,50,{150,150,0});
//     cout << gTextTexture.getWidth() << ' ' << gTextTexture.getHeight() << '\n';
//     gTextTexture.loadText("Score", {0,0,0});
//     cout << gTextTexture.getWidth() << ' ' << gTextTexture.getHeight();
//     SDL_RenderPresent(gRenderer);
//     SDL_Delay(1000);
//     close();
// }
