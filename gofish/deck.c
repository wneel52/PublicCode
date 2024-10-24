#include "deck.h"
/*
 * Variable: deck_instance
 * -----------------------
 *  
 * Go Fish uses a single deck
 */

struct deck deck_instance;



int deck_size(){
   return deck_instance.size;
}



int shuffle(){
    printf("Shuffling deck... \n");
    if(deck_instance.size != 0){
        deck_instance.size = 0;
    }

    // Initialize the deck
    for(int i = 0; i < 52; i++){        
        deck_instance.list[i].rank = (i % 13) + 2;
        deck_instance.list[i].suit = i % 4; 
        deck_instance.size++;
    } 
 
    // Shuffle the deck using fisher-yates algorithm
    for(int i = 0; i < 52; i++){
        int j = rand() % 52;
        struct card temp = deck_instance.list[i];
        deck_instance.list[i] = deck_instance.list[j];
        deck_instance.list[j] = temp;
    }

    return 0;
}


int deal_player_cards(struct player* target){

    // Deal 7 random cards to the player
    for(int i=0; i<7; i++){
        int j = rand() % deck_instance.size;
        struct card dealCard = deck_instance.list[j];

        // add the card to the player's hand 
        //appendToHand(dealCard, target->playerHand);
        add_card(target, dealCard);
       
       
        // remove the card from the deck
        for(int k = j; k < deck_instance.size - 1; k++){
            deck_instance.list[k] = deck_instance.list[k + 1];
        }
        deck_instance.size--;

    }

    return 0;

}


struct card drawCard(struct player* user, struct player* computer){
    
    if(deckState(user, computer) == 0){
        int j = rand() % deck_instance.size;
        struct card dealCard = deck_instance.list[j];
        // Remove the drawn card from the deck
        for (int i = j; i < deck_instance.size - 1; i++) {
            deck_instance.list[i] = deck_instance.list[i + 1];
        }
        deck_instance.size--;
        return dealCard;
    }
    exit(EXIT_FAILURE);
}


int isEmpty(){
    if(deck_instance.size <= 7){
        return 1;
    }
    return 0;
}


int deckState(struct player* user, struct player* computer){

    if(isEmpty()){
        /**/
        printf("\n");
        printf("Game Over: deck is empty\n");
        if (user->bookCount > computer->bookCount){
            printf("User wins: %d - %d \n", user->bookCount, computer->bookCount);
            return 1;
        }
        else if (user->bookCount < computer->bookCount){
            printf("Computer wins: %d - %d \n", computer->bookCount, user->bookCount);
            return 1;
        }
        else{
            printf("It's a tie: %d - %d\n",computer->bookCount, user->bookCount);
            return 1;
        }
    }
    return 0;

}


// EOF