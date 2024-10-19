/*

*/

#include "snakeFuncAI.h"


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
    
    //debugging(gridX, gridY, renderer);
    gameLoopAI(gameQuit, event, renderer, gridX, gridY, false);

    freeSDL(window, renderer);
    SDL_Quit();

    return 0;
}
