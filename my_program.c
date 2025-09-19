#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52
#define SUIT_COUNT 4
#define RANK_COUNT 13

typedef struct {
    int rank;
    const char *suit;
} Card;

typedef struct {
    Card *cards;
    int size;
} Deck;

void initializeDeck(Deck *deck) {
    int rank[] = {2,3,4,5,6,7,8,9,10,11,12,13,14};
    const char *suit[] = {"♠","♥","♦","♣"};
    
    deck->cards = malloc(sizeof(Card) * DECK_SIZE);
    if (!deck->cards) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    
    deck->size = DECK_SIZE;
    
    int card_index = 0;
    for (int i = 0; i < SUIT_COUNT; i++) {
        for (int j = 0; j < RANK_COUNT; j++) {
            deck->cards[card_index].suit = suit[i];
            deck->cards[card_index].rank = rank[j];
            card_index++;
        }
    }
}

void printDeck(const Deck *deck) {
    for (int i = 0; i < deck->size; i++) {
        // Convert face cards to letters for better readability
        switch(deck->cards[i].rank) {
            case 11: printf(" J%s", deck->cards[i].suit); break;
            case 12: printf(" Q%s", deck->cards[i].suit); break;
            case 13: printf(" K%s", deck->cards[i].suit); break;
            case 14: printf(" A%s", deck->cards[i].suit); break;
            default: printf("%2d%s", deck->cards[i].rank, deck->cards[i].suit);
        }
        
        // Format output in columns
        if ((i + 1) % 4 == 0) printf("\n");
        else printf("\t");
    }
}

void freeDeck(Deck *deck) {
    free(deck->cards);
    deck->cards = NULL;
    deck->size = 0;
}

void shuffleDeck(Deck *deck) {
    srand(time(NULL));
    for (int i = deck->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap cards
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

int main() {
    Deck myDeck;
    
    initializeDeck(&myDeck);
    
    printf("Original deck:\n");
    printDeck(&myDeck);
    
    printf("\nShuffled deck:\n");
    shuffleDeck(&myDeck);
    printDeck(&myDeck);
    
    freeDeck(&myDeck);
    
    return 0;
}