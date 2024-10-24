#ifndef CARD_H
#define CARD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


/*
  Valid suits: C, D, H, and S
  Valid ranks: 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A
*/
struct card
{
  
  enum suit
  {
    C, D, H, S
  } suit;

  enum rank
  {
    TWO = 2, 
    THREE, 
    FOUR, 
    FIVE, 
    SIX, 
    SEVEN, 
    EIGHT, 
    NINE, 
    T, 
    J, 
    Q, 
    K, 
    A
  } rank;


};

struct hand {
  struct card Payload;
  struct hand *next;
};


/*
Prints the rank and suit of a card based off of enum values
Prams: struct card c
Return: void
*/
void print_card(struct card c);


/*
Returns the rank and suit of a card based off of enum values
Prams: struct card c
Return: char*
*/
char* processCard(struct card c);


/*
Returns the rank of a card based off of enum values
Prams: int x
Return: char*
*/
char* processRank(int x);


#endif
//EOF