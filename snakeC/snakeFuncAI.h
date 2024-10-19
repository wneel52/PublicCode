/*

    Title: snakeFunc.h
    
    Author: William Neel
    based on code from Datacore Linux on YouTube

    Date: 07/31/24
    
    Description: This file contains the function prototypes for the snake game.

    Version: 1.0

    Change log: 
                07/31/24 - initial version
*/

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// constants for window and grid
#define WINx 0
#define WINy 0
#define WINw 1920
#define WINh 1080
#define GridSize 20
#define GridDim 400

// enum for snake direction
enum{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


// constants for AI
#define BigMistake 0
#define SmallMistake 10
#define GoodMove 20
#define GreatMove 100

enum{
    TF,
    TL,
    TR
};

// struct for snake
struct snake{

    int x, y, dir;
    struct snake *next;

};

// struct for food
typedef struct{
    int x, y;
} Food;

// function prototypes

// game functions

bool initSnake();
void appendToSnake();
void moveSnake();
void generateFood();
void detectCollision();
void detectCrash();
void resetSnake();
void gameLoopAI(bool gameQuit, SDL_Event event, SDL_Renderer *renderer, int gridX, int gridY, bool test);
void debugging(int x, int y, SDL_Renderer *renderer);
void runSnakeAI();
void turnLeft();
void turnRight();
int gameState(int action);



// rendering functions

void renderGrid(bool test, SDL_Renderer *renderer, int x, int y);
void renderSnake(SDL_Renderer *renderer, int x, int y);
void renderFood(SDL_Renderer *renderer, int x, int y);

// SDL functions

bool initSDL(SDL_Window **window, SDL_Renderer **renderer);
void freeSDL(SDL_Window *window, SDL_Renderer *renderer);

//EOF snakeFunc.h