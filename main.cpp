#include "grid.h"
#include "DrawColor.h"
#include "Display.h"
#include "main.h"
#include "button.h"
#include "stdio.h"
#include "iostream"
#include "fstream"

using namespace std;
#define DPAD_UP 11
#define DPAD_DOWN 12
#define DPAD_LEFT 13
#define DPAD_RIGHT 14
#define L1 9
#define R1 10
#define START 4
#define SELECT 6
int score;
int maxScore = 0;
int mode = 0;
bool firstMove = true; // to check if it is the first move in the game, then we implement two random functions.
LButton newGame;
LButton newMode;
LButton Save;
LButton Load;
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

void save(){
    int data[17];
    ofstream outfile;
    outfile.open("dat.txt");
    for (int i = 1; i <= 4; i++){
        for (int j = 1; j <= 4; j++){
            data[(i-1)*4 + j] = grid[i][j].score;
        }
    }
    if (outfile.is_open()){
        for (int i = 1; i <= 17; i++){
            if (i <= 16 ) outfile << data[i] <<" ";
            else outfile << score <<" ";
        }
        outfile.close();
    }
    close();
}
bool checkLoad = true;
void load(){
    checkLoad = false;
    for(int i=1;i<=4;++i){
		for(int j=1;j<=4;++j) {
		    grid[i][j].score = 0;
            grid[i][j].color = BLANK_SQUARE_COLOR;
        }
	}

    int data[17];
    ifstream infile;
    infile.open("dat.txt");
    int n;
    if (infile.is_open()){
        for (int i = 1; i <= 17; i++){
            if (i <= 16 )
                {
                infile >> n;
                data[i] = n;
                }
            else {
                infile >> n;
                score = n;
            }
        }
        infile.close();
    }
    for (int i = 1; i <= 4; i++){
        for (int j = 1; j <= 4; j++){
            grid[i][j].score = data[(i-1)*4+j];
            if (grid[i][j].score) grid[i][j].color = iColor[(int) log2(grid[i][j].score)];
        }
    }
    display(score, grid, &newGame, &newMode, &Save, &Load);
}

void DISPLAY(){
	display(score, grid, &newGame, &newMode, &Save, &Load);
}
int getMode(){return mode;};
// button newMode
void reverse(){
	mode = 1 - mode;
	string text = "Mode " + intoString(mode+1);
	newMode.initButton(5,145,text);
	if (mode == 0){
		for(int i=1;i<=4;++i) for(int j=1;j<=4;++j)
			if (grid[i][j].score){
				grid[i][j].color = iColor[(int)log2(grid[i][j].score)];
			}
	}
	else {
		for(int i=1;i<=4;++i) for(int j=1;j<=4;++j)
			if (grid[i][j].score){
				grid[i][j].color = randomColor();
			}
	}
	display(score, grid, &newGame, &newMode, &Save, &Load);
}
// button newGame
void restart(){
	score = 0;
	for(int i=0;i<=5;++i){
		for(int j=0;j<=5;++j) {
		    grid[i][j].score = 0;
            grid[i][j].color = BLANK_SQUARE_COLOR;
        }
	}
	maxScore = 0;
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
			if (mode == 1) grid[x][y].color = randomColor();
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
				grid[i][j] = grid[i][j] + grid[i][j+1];
				if (mode == 1) grid[i][j].color = colorMix(grid[i][j].color, grid[i][j+1].color);
				score+= grid[i][j].score;
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
				grid[i][j] = grid[i][j] + grid[i][j-1];
				score+= grid[i][j].score;
				if (mode == 1) grid[i][j].color = colorMix(grid[i][j].color, grid[i][j-1].color);
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
				grid[i][j] = grid[i][j] + grid[i+1][j];
				score += grid[i][j].score;
				if (mode == 1) grid[i][j].color = colorMix ( grid[i][j].color, grid[i+1][j].color);
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
				if (mode == 1) grid[i][j].color = colorMix(grid[i][j].color, grid[i-1][j].color);
				for(int z = i-1; z>= 1; --z) grid[z][j] = grid[z-1][j];
			}
		}
	}
}
// check if the grid could generate new number
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
		restart();
		break;
	case SELECT:
		reverse();
		break;
	}

}
void game(){
	bool quit = false;
	newGame.initButton(5,85, "New game");
	string text = "Mode " + intoString(mode+1);
    newMode.initButton(5,145,text);
	Save.initButton(5,205,"Save");
	Load.initButton(5,265,"Load");
	while (ok() && !quit){
		if (checkLoad)random();
		if (checkLoad && firstMove) {random(); firstMove = false;}
		checkLoad = true;
		// at the first move, we have two numbers in our screen.
		display(score, grid, &newGame, &newMode,&Save,&Load);
		if (loseCondition()) {
                break;
		}
		for(int i=1;i<=4;++i) for(int j=1;j<=4;++j) pre[i][j] = grid[i][j];
		while(!quit){
			SDL_Delay(10);
			SDL_Event e;
			while (SDL_PollEvent(&e)){
				if (e.type == SDL_QUIT) {quit = true; }
				else {
					if (e.type == SDL_KEYDOWN) implement(e);
					else if(e.type == SDL_CONTROLLERBUTTONDOWN)	implementPS4(e);
					else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONUP) {
						newGame.handleEvent(&e,restart);
						newMode.handleEvent(&e,reverse);
						Save.handleEvent(&e,save);
						Load.handleEvent(&e,load);
						}
					if(maxScore < maxBoard()){
						int msec = maxBoard() - maxScore;
						if(msec < 200) msec = 200;
						if( SDL_HapticRumblePlay( gControllerHaptic, 0.4 + maxScore / 2048.0, msec ) != 0 ){
							printf( "Warning: Unable to play rumble! %s\n", SDL_GetError() );
						}
						maxScore = maxBoard();}
				}
			}
			newGame.drawButton();
			newMode.drawButton();
			Save.drawButton();
			Load.drawButton();
			PresentRender();
			if (compare()) break;
		}
	// If the Player Lose the Game
	}
	ScreenForLoser(score);
    while (!quit){
		SDL_Delay(10);
		SDL_Event e;
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) quit = true;
			else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONUP) {
				newGame.handleEvent(&e,restart);
				newGame.drawButton();
				PresentRender();
				// check if restart again.
				if (firstMove) game();
			}
			else if (e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == START) restart();
        }
    }
	close();
	//system("cls");
//	cout << "You lost.\n";
//	cout << "Your score is: " << score << '\n';
//	Sleep(2000);
}
int main(int argc, char *argv[]){
	srand(time(NULL));
	//display(score, grid, &newGame, &newMode);
	init();
	loadMedia();
	initController();
	game();
	//SDL_Delay(5000);
	//init();
	//loadMedia();
	//ScreenForLoser(score);
	// random();
	//display(1000000, grid, &newGame);
	//ScreenForLoser(score);
	//SDL_Delay(1000);
	//debug();
}
