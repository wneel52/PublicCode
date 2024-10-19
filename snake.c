/*

    Title: snake.c

    Author: getZinked
    
    Date: 07/31/24
    
    Description: This file contains the main function for the snake game.

    Version: 1.0

    Change log: 
                07/31/24 - initial version
*/

#include "snakeFunc.h"

//DEPLOYMENT VARIABLE: determine if the game is being tested or a user is playing
bool test = true;

int main(){
    
    // seed random number generator
    srand(time(0));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!initSDL(&window, &renderer)) {
        fprintf(stderr, "Failed to initialize SDL\n");
        return -1;
    }

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate grid position (centered)
    int gridX = (windowWidth - GridDim) / 2;
    int gridY = (windowHeight - GridDim) / 2;

    // initialize snake    
    if(initSnake()){
        for(int x = 0; x < 3; x++){
            appendToSnake();
        }
    }
    generateFood();
   
    // this is where the game loop will be
    
    bool gameQuit = false;
    SDL_Event event;
    
    debugging(gridX, gridY, renderer);
    gameLoop(gameQuit, event, renderer, gridX, gridY, test);

    freeSDL(window, renderer);
    SDL_Quit();

    return 0;
}


//EOF snake.c