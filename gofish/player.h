#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"


/*
 * Structure: player
 * -----------------
 *  Each player holds some number of cards in their hand
 *  Each player can have at most 7 "books" before winning 
 */


struct player {
  struct hand* playerHand;
  char* book[7];
  int bookCount;
};


/* 
 * Instance Variables: user, computer
 * ----------------------------------
 *
 *  We only support 2 users: a human and a computer
 */
extern struct player user;
extern struct player computer;

/*
add_card: Add a new card to the player's hand.
Prams: struct player* target, struct card new_card
return: int : return 0 if no error, non-zero otherwise
*/
int add_card(struct player* target, struct card new_card);



/*
removeCard: Remove a card from a player's hand
Prams: struct player* target, struct card* card2destroy
Return: int : return 0 if card is removed, -1 if card is not found
*/
int removeCard(struct player* target, struct card* card2destroy);


/*
returnCard: Return a card to the deck
Prams: struct player* target, struct card* card2Return
Return: struct card : return the card that was returned
*/
struct card returnCard(struct player* target, struct card* card2destroy);

/*
printHand: Print the cards in a player's hand
Prams: struct player* target
Return: void
*/
void printHand(struct player* target);


/*
check_add_book: Check if a player has all 4 cards of the same rank.
If so, remove those cards from the hand, and add the rank to the book.
Returns after finding one matching set of 4, so should be called after adding each a new card.
Prams: struct player* target
Return: int : a int that indicates the book that was added; return 0 if no book added.
*/
int check_add_book(struct player* target);


/*
search: Search a player's hand for a requested rank.
Prams: struct player* target, char rank
Return: int : If the player has a card of that rank, return the count of that rank, else return 0
*/
int search(struct player* target, char rank);


/*
transferCards: Transfer cards of a given rank from the source player's hand to the destination player's hand.
Remove transferred cards from the source player's hand. Add transferred cards to the destination player's hand.
Prams: struct player* src, struct player* dest, char rank
Return: int : return 0 if no cards found/transferred, <0 if error, otherwise return value indicates number of cards transferred
*/ 
int transferCards(struct player* src, struct player* dest, char rank); 


/*
gameOver: Check if a player has 7 books yet and the game is over
Prams: struct player* target
Return: int : return 1 if game is over, 0 if game is not over
*/
int gameOver(struct player* target);


void reset_player(struct player* target);


/*
computerPlay: Select a rank randomly to play this turn. The player must have at least
Prams: struct player* target
Return: int rank : return a valid selected rank
*/
int computerPlay(struct player* target);


/*
userPlay: Prompt the user to enter a rank to play this turn. 
The player must have at least one card of the selected rank in their hand.
Prams: struct player* target
Return: int rank : return a valid selected rank
*/
int userPlay(struct player* target);


#endif
//EOF