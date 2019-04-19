#include "grid.h"
#include "DrawColor.h"
#include "Display.h"
#include "main.h"
#include "button.h"
using namespace std;
int score; 
int maxScore = 0; 
bool firstMove = true;
LButton newGame;
#define DPAD_UP 11
#define DPAD_DOWN 12
#define DPAD_LEFT 13
#define DPAD_RIGHT 14
#define L1 9
#define R1 10
#define START 6

SDL_GameController* gGameController = NULL;
SDL_Haptic* gControllerHaptic = NULL;
bool controllerConnected = false;
void initController(){
	if( SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return;
	}
	if( SDL_NumJoysticks() < 1 ){
			printf( "Warning: No joysticks connected!\n" );
		}
		else{
			for(int i = 0 ; i < SDL_NumJoysticks() ; i++){
				if(SDL_IsGameController(i)){
					gGameController = SDL_GameControllerOpen(i);
					gControllerHaptic = SDL_HapticOpenFromJoystick( SDL_GameControllerGetJoystick(gGameController) );
					if( gControllerHaptic == NULL ){
						printf( "Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError() );
					}
					else{
						if( SDL_HapticRumbleInit( gControllerHaptic ) < 0 ){
							printf( "Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError() );
						}
					}
					break;
				}
				if( gGameController == NULL ){
					printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
				}
				else controllerConnected = true;
			}
		}
}




GRID grid[N][N], pre[N][N];
void restart(){
	score = 0; 
	for(int i=0;i<=5;++i){
		for(int j=0;j<=5;++j) {grid[i][j].score = 0, grid[i][j].color = BLANK_SQUARE_COLOR;}
	}
	firstMove = true;
}
int maxBoard(){
	int ans = 0;
	for(int i=1;i<=4;++i) for(int j=1;j<=4;++j) ans = max(ans, (int)grid[i][j].score);
	return ans;
}
void random(){
	// add a new member; 
	bool temp = false;	
	while (!temp){
		int x = rand() % 4 + 1, y = rand() % 4 + 1; 
		// rand the place
		if (!grid[x][y].score) {
			// check if that grid is available
			temp = true;	
			grid[x][y].random(); // random the value for grid[x][y]
		}
	}
}
// Moving action
void left(){
	// avoid A - 0 - B - C error
	for(int i=1;i<=4;++i){
		int cnt = 0;
		for(int j=1;j<=4;++j){
			if (grid[i][j].score == 0) {
				for(int z=j;z<=4;++z) grid[i][z] = grid[i][z+1];
				if (++cnt > 5) break;
				j--;
			}
		}
	}
	// adding element
	// ans represent for the number of implement that we have to do
	// it help you to check this move is possible.
	// Ex: 
	// 4 2 4
	// 8 4 8
	// 2 8 2
	// In this case, we can't move to the left.
	for (int i=1;i<=4;++i){
		for(int j=1;j<4;++j){
			if (grid[i][j].score && grid[i][j].score == grid[i][j+1].score) {
				grid[i][j] = grid[i][j] + grid[i][j+1]; score+= grid[i][j].score;
				for(int z = j+1;z<=4;++z) grid[i][z] = grid[i][z+1];
			}
		}
	}
}
void right(){
	// avoid A - 0 - B - C error
	for(int i=1;i<=4;++i){
		int cnt = 0; 
		for(int j=4;j>=1;--j){
			if (grid[i][j].score == 0){
				for(int z=j;z>=1;--z) grid[i][z] = grid[i][z-1];
				if (++cnt > 5) break;					
				j++;
			}
		}
	}
	for(int i=1;i<=4;i++){
		for(int j=4;j>1;--j){
			if (grid[i][j].score && grid[i][j].score == grid[i][j-1].score){
				grid[i][j] = grid[i][j] + grid[i][j-1]; score+= grid[i][j].score;
				for(int z=j-1;z>=1;--z) grid[i][z] = grid[i][z-1]; 
			}
		}
	}
}
void up(){
	// avoid A - 0 - B - C error
	for(int j=1;j<=4;++j){
		int cnt = 0;
		for(int i=1;i<=4;++i){
			if (grid[i][j].score == 0) {
				for(int z=i;z<=4;++z) grid[z][j] = grid[z+1][j];
				if (++cnt > 5) break;
				i--; 
			}
		}
	}
	for(int j=1;j<=4;++j){
		for(int i=1;i<4;++i){
			if (grid[i][j].score && grid[i][j].score == grid[i+1][j].score){
				grid[i][j] = grid[i][j] + grid[i+1][j]; score+= grid[i][j].score;
				for(int z = i+1;z<=4;++z) grid[z][j] = grid[z+1][j];
			}
		}
	}
}
void down(){
	for(int j=1;j<=4;++j){
		int cnt = 0;
		for(int i=4;i>=1;--i){
			if (grid[i][j].score == 0) {
				for(int z = i; z>= 1; --z) grid[z][j] = grid[z-1][j];
				if (++cnt > 5) break;					
				i++; 
			}
		}
	}
	for(int j=1;j<=4;++j){
		for(int i=4;i>1;--i){
			if (grid[i][j].score && grid[i][j].score == grid[i-1][j].score){
				grid[i][j] = grid[i][j] + grid[i-1][j]; score+= grid[i][j].score;
				for(int z = i-1; z>= 1; --z) grid[z][j] = grid[z-1][j];
			}
		}
	}
}
/*void display(){
	//system("cls");
	cout << "Score:" << score << '\n';
	for(int i=1;i<=4;++i){
		for(int j=1;j<=4;++j){
			cout << setw(6) << grid[i][j];
		}
		cout << '\n'; 
	}
}*/
bool ok(){
	for(int i=1;i<=4;++i){
		for(int j=1;j<=4;++j) if (!grid[i][j].score) return 1;
	}
	return 0; 
}
bool loseCondition(){
	for(int i=1;i<=4;++i){
		for(int j=1;j<=4;++j) if (!grid[i][j].score) return false; 
	}
	for(int i=1;i<=4;++i) for(int j=1;j<4;++j) if (grid[i][j].score == grid[i][j+1].score) return false;
	for(int j=1;j<=4;++j) for(int i=1;i<4;++i) if (grid[i][j].score == grid[i+1][j].score) return false;
	return true;
}
// compare between two matrix [pre and grid]
// pre is the previous state of grid (before implementing an action)
// the goal for this function is checking if the matrix "grid" have changed through 
// an implementation.
bool compare(){
	for(int i=1;i<=4;++i) for(int j=1;j<=4;++j) if (grid[i][j].score != pre[i][j].score) return true;
	return false;	
}
void implement(SDL_Event e){
	switch( e.key.keysym.sym )
		{
			case SDLK_UP:
			up();
			break;

			case SDLK_DOWN:
			down();
			break;
						

			case SDLK_LEFT:
			left();
			break;

			case SDLK_RIGHT:
			right();
			break;

			default: 
			break;
	}
}
void implementPS4(SDL_Event e){
	printf("%d\n", e.cbutton.button);
	switch(e.cbutton.button) {
	case DPAD_UP:
		up();
		break;
	case DPAD_RIGHT:
		right();
		break;
	case DPAD_DOWN:
		down();
		break;
	case DPAD_LEFT:
		left();
		break;
	case START:
		//	g.restart();
		break;
	}
}
void game(){
	init();
	loadMedia();
	initController();
	bool quit = false;
	SDL_Event e;
	newGame.initButton(0,50, "New game");
	while (ok() && !quit){
		random();
		if (firstMove) {random(); firstMove = false;}
		// at the first move, we have two numbers in our screen. 
		display(score, grid, &newGame);
		if (loseCondition()) break;
		for(int i=1;i<=4;++i) for(int j=1;j<=4;++j) pre[i][j] = grid[i][j];
		while(!quit){
			while (SDL_PollEvent(&e)){
				if (e.type == SDL_QUIT) {quit = true; }
				else {
					if (e.type == SDL_KEYDOWN) implement(e);
					else if(e.type == SDL_CONTROLLERBUTTONDOWN)	implementPS4(e);
					else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONUP) newGame.handleEvent(&e);
					if(maxScore < maxBoard()){
						int msec = maxBoard() - maxScore;
						if(msec < 200) msec = 200;
						if( SDL_HapticRumblePlay( gControllerHaptic, 0.4 + maxScore / 2048.0, msec ) != 0 ){
							printf( "Warning: Unable to play rumble! %s\n", SDL_GetError() );
						}
						maxScore = maxBoard();
					}
				}
			}
			if (compare()) break;
		}
	//	
	}
	ScreenForLoser(score);
	close();
	//system("cls"); 
//	cout << "You lost.\n";
//	cout << "Your score is: " << score << '\n';
//	Sleep(2000);
}
int main(int argc, char *argv[]){
	srand(time(NULL));
	game();
	//init();
	//loadMedia();
	//ScreenForLoser(score);
	// random();
	//display(1000000, grid, &newGame);
	//ScreenForLoser(score);
	//SDL_Delay(1000);
	//debug();
} 