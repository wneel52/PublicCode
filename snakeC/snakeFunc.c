/*

    Title: snakeFunc.c

    Author: William Neel
    based on code from Datacore Linux on YouTube
    
    Date: 07/31/24
    
    Description: This file contains the function definitions for the snake game.

    Version: 1.0

    Change log: 
                07/31/24 - initial version
*/

#include "snakeFunc.h"

#define delay 80

// global variables
typedef struct snake Snake;
Snake *head;
Snake *tail;
int score = 0;
Food food;

// function definitions


// initialize SDL window and renderer
bool initSDL(SDL_Window **window, SDL_Renderer **renderer){

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error: SDL_INIT_VIDEO - %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Snake", WINx, WINy, WINw, WINh, SDL_WINDOW_BORDERLESS);

    if (!*window) {
        fprintf(stderr, "Error: SDL_CreateWindow - %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (!*renderer) {
        fprintf(stderr, "Error: SDL_CreateRenderer - %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

    return true;
}

// free SDL window and renderer
void freeSDL(SDL_Window *window, SDL_Renderer *renderer) {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
}

// render grid
void renderGrid(bool test ,SDL_Renderer *renderer, int x, int y){
    
    if (test) {
        SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0xff, 255);         

        // Calculate the size of each cell
        int cellSize = GridDim / GridSize;

        SDL_Rect cell;
        cell.w = cellSize;
        cell.h = cellSize;

        // Render the grid cells
        for (int i = 0; i < GridSize; i++) {
            for (int j = 0; j < GridSize; j++) {
                cell.x = x + (i * cellSize);
                cell.y = y + (j * cellSize);
                SDL_RenderDrawRect(renderer, &cell);
            }
        }
    }  
    else {
        // Render the play area

        for(int i = 0; i < 255; i++){

            int r = rand() % 255;

            SDL_SetRenderDrawColor(renderer, r, 0, 255-i, 255-i);

            SDL_Rect playArea = {
                .x = x-i,
                .y = y-i,
                .w = GridDim+i+i,
                .h = GridDim+i+i
            };

            
            SDL_RenderDrawRect(renderer, &playArea);
        }
    
    }



}

// initialize snake
bool initSnake(){
    
    int spawnArea = (GridSize/2)+(GridSize/4);

    Snake *new = malloc(sizeof(Snake));
    new->x = rand() % spawnArea;
    new->y = rand() % spawnArea;
    new->dir = rand() % 4;
    new->next = NULL;

    head = new;
    tail = new;

    return true;

}

// add a new segment to the snake
void appendToSnake(){
    
    Snake *new = malloc(sizeof(Snake));
    
    switch(tail->dir){
        case UP:
            new->x = tail->x;
            new->y = tail->y + 1;
            break;
        case DOWN:
            new->x = tail->x;
            new->y = tail->y - 1;
            break;
        case LEFT:
            new->x = tail->x + 1;
            new->y = tail->y;
            break;
        case RIGHT:
            new->x = tail->x - 1;
            new->y = tail->y;
            break;
    }
    
    new->dir = tail->dir;
    new->next = NULL;
    tail->next = new;
    tail = new;
}

// render snake segments
void renderSnake(SDL_Renderer *renderer, int x, int y){

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);
    SDL_Rect seg;
    
    int segSize = GridDim / GridSize;    
    seg.w = segSize;
    seg.h = segSize;    

    Snake *curr = head;

    while(curr!=NULL){

        seg.x = x + (curr->x * segSize+1);
        seg.y = y + (curr->y * segSize+1);

        SDL_RenderFillRect(renderer, &seg);

        curr = curr->next;
        
    }

}

// move snake segments
void moveSnake(){

    int prevX = head->x;
    int prevY = head->y;
    int prevDir = head->dir;

    switch(head->dir){
        
        case UP:
            head->y--;
            break;
        case DOWN:
            head->y++;
            break;
        case LEFT:
            head->x--;
            break; 
        case RIGHT:
            head->x++;
            break;
        
    }

    Snake *curr = head->next;

    while(curr!=NULL){
        int tempX = curr->x;
        int tempY = curr->y;
        int tempDir = curr->dir;

        curr->x = prevX;
        curr->y = prevY;
        curr->dir = prevDir;

        curr=curr->next;
        
        prevX = tempX;
        prevY = tempY;
        prevDir = tempDir;

    }
    
}

// generate food on grid
void generateFood(){

    bool valid;
    
    do{
        valid = true;
        food.x = rand() % GridSize;
        food.y = rand() % GridSize;

        Snake *curr = head;

        while(curr!=NULL){
            if(curr->x == food.x && curr->y == food.y){
                valid = false;
            }
            curr = curr->next;
        }
    }while(!valid);


}

// render food on grid
void renderFood(SDL_Renderer *renderer, int x, int y){

    int foodSize = GridDim / GridSize;
    SDL_Rect foodRect = {
        .w = foodSize,
        .h = foodSize,
        .x = x + (food.x * foodSize),
        .y = y + (food.y * foodSize)

    };
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 255);
    SDL_RenderFillRect(renderer, &foodRect);



}

// detect collision with food
void detectCollision(){

    if(head->x == food.x && head->y == food.y){
        generateFood();
        appendToSnake();
        score++;
    }
    
}

// detect collision with walls or self
void detectCrash() {
    // Check for collision with walls
    if (head->x < 0 || head->x >= GridSize || head->y < 0 || head->y >= GridSize){
        printf("Game Over! Score: %d\n", score);
        printf("You hit a wall\n @ %d, %d\n", head->x, head->y);
        resetSnake();
        // Exit the function after resetting the snake
    }

    // Check for collision with self
    Snake *curr = head->next;
    while (curr != NULL) {
        if (head->x == curr->x && head->y == curr->y) {
            printf("Game Over! Score: %d\n", score);
            printf("You hit yourself\n @ %d, %d\n", head->x, head->y);
            resetSnake();
            // Exit the function after resetting the snake
        }
        curr = curr->next;
    }
}

// reset snake to new state
void resetSnake(){

    Snake *curr = head;
    Snake *temp;

    while(curr!=NULL){
        temp=curr;
        curr=curr->next;
        free(temp);
    }

    score = 0;

    if(initSnake()){
        for(int x = 0; x < 3; x++){
            appendToSnake();
        }
    }

    generateFood();
    SDL_Delay(2000);
}

// game loop function 
void gameLoop(bool gameQuit, SDL_Event event, SDL_Renderer *renderer, int gridX, int gridY, bool test){
    
    while (!gameQuit) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameQuit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        gameQuit = true;
                        break;
                    case SDLK_UP:
                    head->dir = UP;
                    break;
                case SDLK_DOWN:
                    head->dir = DOWN;
                    break;
                case SDLK_LEFT:
                    head->dir = LEFT;
                    break;
                case SDLK_RIGHT:
                    head->dir = RIGHT;
                    break;
            }
        }
    }
        
        // clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        
        // move first then render 
        moveSnake();
        SDL_Delay(10);    
        detectCrash();
        SDL_Delay(10);
        detectCollision();
        SDL_Delay(10);
        debugging(gridX, gridY, renderer);
               
        // render loop
        renderGrid(test, renderer, gridX, gridY);
        renderFood(renderer, gridX, gridY);
        renderSnake(renderer, gridX, gridY);

        // present renderer
        SDL_RenderPresent(renderer);

        // delay to control speed of game
        SDL_Delay(delay);
    }

}

void debugging(int windowWidth, int windowHeight, SDL_Renderer *renderer) {
    // Set the size of the block
    int cellSize = GridSize;

    // Calculate the center position
    int centerX = (windowWidth - cellSize) / 2;
    int centerY = (windowHeight - cellSize) / 2;

    // Set render color to yellow
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // Create a rectangle at the center
    SDL_Rect centerRect = {centerX, centerY, cellSize, cellSize};

    // Render the rectangle
    SDL_RenderFillRect(renderer, &centerRect);
}

//EOF snakeFunc.c