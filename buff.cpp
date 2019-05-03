#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
using namespace std;
#undef main
#define undefine SDL_WINDOWPOS_UNDEFINED
//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
TTF_Font* gFont = NULL;
SDL_Color gColor;

SDL_Texture* loadText( string text, TTF_Font* gFont, SDL_Color gColor, SDL_Renderer* gRenderer){

	SDL_Surface* tempSurface = TTF_RenderText_Solid( gFont, text.c_str(), gColor);

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface( gRenderer, tempSurface);

	SDL_FreeSurface(tempSurface);

	return newTexture;
}

void close(){
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	TTF_Quit();
	SDL_Quit();
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

int main(){

	// init everything
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	//Create Window
	gWindow = SDL_CreateWindow("Scoreboard",undefine, undefine, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	//Render Window
	gRenderer = SDL_CreateRenderer(gWindow,-1, SDL_RENDERER_ACCELERATED);
    // Font
    gFont = TTF_OpenFont("VeraMobd.ttf", 20);
	// Color of the board
	gColor = {0, 0, 0};
	string text = intoString(100);
	gTexture = loadText(text, gFont, gColor, gRenderer);
	SDL_Rect srcRect, desRect;
	TTF_SizeText(gFont, text.c_str(), &srcRect.w, &srcRect.h);
	srcRect.x = 0;
	srcRect.y = 0;
	desRect.x = 200;
	desRect.y = 270;
	desRect.w = srcRect.w;
	desRect.h = srcRect.h;
	bool quit = false;
	SDL_Event e;

	while (!quit){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_KEYDOWN) quit = true;
		}
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer,0xFF, 0, 0, 0xFF);
		SDL_RenderFillRect(gRenderer, &desRect);
		SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
		SDL_RenderCopy(gRenderer, gTexture, &srcRect, &desRect);
		SDL_RenderPresent(gRenderer);
	}
	close();
}
