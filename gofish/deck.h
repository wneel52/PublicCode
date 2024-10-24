#ifndef DECK_H
#define DECK_H

#include "player.h"

/*
 * Structure: deck
 * ---------------
 *  An array of 52 cards.
 */
struct deck {
  struct card list[104];
  int size;
};


/*
shuffle: shuffles the deck
Params: none
Return: int, 0 if no error, non-zero on error
*/
int shuffle();


/*
deal_player_cards: deals 7 random cards to the player
Params: struct player* target
Return: int, 0 if no error, non-zero on error
*/
int deal_player_cards(struct player* target);


/*
deck_size: returns the number of cards left in the deck
Params: none
Return: int, the number of cards left in the deck
*/
int deck_size();


/*
isEmpty: checks if the deck is empty
Params: none
Return: int, 1 if the deck is empty, 0 if not
*/
int isEmpty();


/*
deckState: checks if the deck is empty and prints the winner
Params: struct player* user, struct player* computer
Return: int, 1 if the deck is empty, 0 if not
*/
int deckState(struct player* user, struct player* computer);



/*
drawCard: draws a card from the deck
Params: struct player* user, struct player* computer
Return: struct card
*/
struct card drawCard(struct player* user, struct player* computer);


#endif
//EOF