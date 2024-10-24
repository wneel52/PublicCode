#include "deck.h"

// Function Prototypes
int computerTurn(struct player* target, struct player* opponet);
int userTurn(struct player* target, struct player* opponet);
void gameLoop(struct player* player1, struct player* player2);

 

int main(int args, char* argv[]) 
{
  srand(time(0));
    

  struct player user;
  struct player computer;

  
  computer.playerHand = NULL;
  user.playerHand = NULL;
  computer.bookCount = 0;
  user.bookCount = 0;
   
  if(shuffle() != 0){
    return -1;
  }
  
  if(deck_size() != 52){
    return -1;
  }

  if((deal_player_cards(&user) != 0)){
    return -1;
  }
  deal_player_cards(&computer);  

  gameLoop(&user, &computer); 

  return 0;
}


/*
computerPlay: returns a random rank for the computer to ask for
Params: struct player* target, struct player* opponet
Return: int
*/
int computerTurn(struct player* target, struct player* opponet){
  
  if(target->playerHand == NULL){
  
    printf("Computer hand is empty\n");
    if(isEmpty()==1){
      printf("Deck is also empty: ending game \n");
      deckState(target,opponet);
      return 1;
    }
    deal_player_cards(target); 
    
  }
  
  int rank = computerPlay(target);
  int count = search(opponet, rank);
  // need 2 process character
  printf("Computer asked for %s\n", processRank(rank));

  if(count == 0){
    
    printf("Go fish! \n");
    struct card newCard = drawCard(target, opponet);
    add_card(target, newCard);
    printf("Computer draws %s\n", processCard(newCard));
    
    
    if(check_add_book(target)==0){
     // Check if the player's hand is empty after adding a book
     if(target->playerHand == NULL){
       printf("Computer's hand is now empty.\n");}
       if(isEmpty() == 1){
         printf("Deck is also empty: ending game \n");
         deckState(target, opponet);
         return 1;
       }
      if(target->playerHand == NULL){
        deal_player_cards(target);
      }
      return 0;
    }
    return 1;  
  
  }
  transferCards(opponet, target, rank);
  check_add_book(target);
  
  return 0;
  
}


/*
userTurn: returns a random rank for the computer to ask for
Params: struct player* target, struct player* opponet
Return: int
*/
int userTurn(struct player* target, struct player* opponet){

  
  if(target->playerHand == NULL){
  
    printf("PLayer hand is empty\n");
    if(isEmpty()==1){
      printf("Deck is also empty: ending game \n");
      deckState(target,opponet);
      return 1;
    }
    deal_player_cards(target); 
    
  }


  int rank = userPlay(target);
  int count = search(opponet, rank);
  
  printf("User asked for %s\n", processRank(rank));

  if(count == 0){
    printf("Go fish! \n");
    struct card newCard = drawCard(target, opponet);
    add_card(target, newCard);
    printf("User draws %s\n", processCard(newCard));
    
    if(check_add_book(target)==0){
      if(target->playerHand == NULL){
        deal_player_cards(target);
      }   
      return 0;
    }
    return 1;  
  
  }
  else{
    transferCards(opponet, target, rank);
    check_add_book(target);
  }
  return 0;
  
}


/*
gameLoop: runs the game loop
Params: struct player* player1, struct player* player2
Return: void
*/
void gameLoop(struct player* player1, struct player* player2){
  
  while(!gameOver(player1) && !gameOver(player2)){
    if(deckState(player1, player2) == 1){
      break;
    }
    
    // Player 1's turn (Computer)
    do {
      printf("Player 1's hand - ");
      printHand(player1);
    } while(computerTurn(player1, player2) == 0); 
    printf("END P1 \n");
    printf("Player 1 has %d books\n", player1->bookCount); 
    printf("Player1 hand after turn \n");
    printHand(player1);

    // Player 2's turn (User)
    do {
      printf("Player 2's hand - ");
      printHand(player2);
    } while(userTurn(player2, player1) == 0);
    printf("END P2 \n");
    printf("player 2 has %d books\n", player2->bookCount);
    printf("Player2 hand after turn \n");
    printHand(player2);
  }
  
}


// EOF
