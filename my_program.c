#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#define DECK_SIZE 52
#define SUIT_COUNT 4
#define RANK_COUNT 13
#define HAND_SIZE 5
#define COMBINATIONS 21 //combinations calculated by formula 7*6*5*4*3/5*4*3*2*1



typedef enum {
    DECK_NEW,
    DECK_SHUFFLED,
    DECK_EMPTY,
    DECK_PARTIAL
} DeckState;

typedef struct {
    int value;
    const char *display;
} Rank;

typedef struct {
    const char *symbol;
    const char *name;
} Suit;

typedef struct {
    Rank rank;
    Suit suit;
} Card;

typedef struct {
    Card cards[DECK_SIZE];
    unsigned int cards_remaining; 
    DeckState state;
} Deck;

typedef struct {
    Card cards[HAND_SIZE];
    unsigned int cards_in_hand; 
} Hand;

const Rank RANKS[] = {
    {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"}, {6, "6"}, {7, "7"}, 
    {8, "8"}, {9, "9"}, {10, "10"}, {11, "J"}, {12, "Q"}, {13, "K"}, {14, "A"}
};

const Suit SUITS[] = {
    {"♠", "Spades"}, {"♥", "Hearts"}, {"♦", "Diamonds"}, {"♣", "Clubs"}
};

const Rank JOKER_RANK = {0, "Joker"};
const Suit JOKER_SUIT = {"★", "Joker"};

void init_deck(Deck *deck) {
    int card_index = 0;
    for (int i = 0; i < SUIT_COUNT; i++) {
        for (int j = 0; j < RANK_COUNT; j++) {
            deck->cards[card_index].suit = SUITS[i];
            deck->cards[card_index].rank = RANKS[j];
            card_index++;
        }
    }
    deck->cards_remaining=DECK_SIZE;
    deck->state = DECK_NEW;
}

const char* deck_state_to_string(DeckState state) {
    switch (state) {
        case DECK_NEW: return "NEW";
        case DECK_SHUFFLED: return "SHUFFLED";
        case DECK_EMPTY: return "EMPTY";
        case DECK_PARTIAL: return "PARTIAL";
        default: return "UNKNOWN";
    }
}

void shuffle_deck(Deck *deck) {
    if (deck->cards_remaining < DECK_SIZE) {
        printf("Warning: Shuffling partial deck (%d cards remaining)\n", 
               deck->cards_remaining);
    }
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
    deck->state = DECK_SHUFFLED;
}

void update_deck_state(Deck *deck) {
    if (deck->cards_remaining == 0) {
        deck->state = DECK_EMPTY;
    } else if (deck->cards_remaining == DECK_SIZE) {
        deck->state = (deck->state == DECK_SHUFFLED) ? DECK_SHUFFLED : DECK_NEW;
    } else {
        deck->state = DECK_PARTIAL;
    }
}

Card draw_card(Deck *deck) {
    if (deck->cards_remaining == 0) {
        printf("Deck is empty! Returning joker.\n");
        deck->state = DECK_EMPTY;
        return (Card){JOKER_RANK, JOKER_SUIT};
    }
    
    Card drawn = deck->cards[--deck->cards_remaining];
    
    if (deck->cards_remaining == 0) {
        deck->state = DECK_EMPTY;
    } else if (deck->cards_remaining < DECK_SIZE) {
        deck->state = DECK_PARTIAL;
    }
    return drawn;
}

void add_card(Hand *hand, Deck *deck) {
    if (hand->cards_in_hand == HAND_SIZE) {
        printf("Your hand is full.\n");
    } else {
        hand->cards[hand->cards_in_hand++] = draw_card(deck);
    }
}

void sort_hand_ascending(Hand *hand) {
    for (int i = 0; i< HAND_SIZE-1; i++) {
        for (int j = i+1; j< HAND_SIZE; j++) {
            if (hand->cards[j].rank.value < hand->cards[i].rank.value) {
                Card temp = hand->cards[i];
                hand->cards[i] = hand->cards[j];
                hand->cards[j] = temp;
            } 
        }
    }
}

int sum_ranks(Hand *hand) {
    int sum_of_ranks = 0;
    for (int i = 0; i < HAND_SIZE; i++) {
        sum_of_ranks += hand->cards[i].rank.value;
    }
    return sum_of_ranks;
}

bool check_for_straight(Hand *hand) {
    sort_hand_ascending(hand);
    int sum_of_differences = 0;
    for (int i = 0; i< HAND_SIZE-1; i++) {
        int j = i+1;
        sum_of_differences += (hand->cards[j].rank.value - hand->cards[i].rank.value);
        printf("difference = %d\n", (hand->cards[j].rank.value - hand->cards[i].rank.value));
        printf("tally = %d\n", sum_of_differences);
    }
    if (sum_of_differences == 4 | sum_of_differences == -4) {
        return true;
    } else {
        return false;
    }
}

bool check_for_flush(Hand *hand) {
    for (int i = 0; i< HAND_SIZE-1; i++) {
        int j = i+1;
        int comparison = strcmp(hand->cards[i].suit.symbol, hand->cards[j].suit.symbol);
        if (comparison != 0) {
            return false;
        }
    } return true;
}

int sum_of_duplicates(Hand *hand) {
    int comparison_tally = 0;
        for (int i = 0; i< HAND_SIZE-1; i++) {
            int tmp = 1;
            for (int j = 0; j< HAND_SIZE-1; j++) {
                if (j == i) {
                    j++;
                }
                if (hand->cards[i].rank.value == hand->cards[j].rank.value) {
                    tmp ++;
                };
            }
        if (tmp > comparison_tally) {
            comparison_tally = tmp;
        }
    } return comparison_tally;
}

// char *poker_hand(Hand *hand) {
// Royal Flush: Ace, King, Queen, Jack, and 10, all of the same suit. 
// Straight Flush: Five cards in sequence, all of the same suit. 
// Four-of-a-Kind: Four cards of the same rank, plus one other card. 
// Full House: Three cards of the same rank and two cards of another rank. 
// Flush: Five cards of the same suit that are not in sequence. 
// Straight: Five cards in sequence, not necessarily of the same suit. 
// Three-of-a-Kind: Three cards of the same rank, plus two other cards. 
// Two Pair: Two sets of two cards of the same rank, plus one other card. 
// One Pair: Two cards of the same rank, plus three other cards. 
// High Card: When no other combinations are present, the highest card in your hand determines the strength. 
// }


// error checks
void print_card(const Card *card) {
    printf("%s%s", card->rank.display, card->suit.symbol);
}

void print_deck_status(const Deck *deck) {
    printf("Cards remaining: %u/%d, State: %s\n",
           deck->cards_remaining, DECK_SIZE, 
           deck_state_to_string(deck->state));
}


int main() {
    srand(time(NULL));  // Seed only once!
    
    Deck my_deck;
    Hand my_hand;
    init_deck(&my_deck);
    
    printf("Initial deck:\n");
    print_deck_status(&my_deck);
    
    shuffle_deck(&my_deck);
    printf("\nAfter shuffling:\n");
    print_deck_status(&my_deck);
    
    printf("\nDrawing 5 cards:\n");
    for (int i = 0; i < 5; i++) {
        Card drawn = draw_card(&my_deck);
        printf("Drew: ");
        print_card(&drawn);
        printf("\n");
    }
    
    print_deck_status(&my_deck);
    

    printf("\nDrawing 5 cards:\n");
    for (int i = 0; i < HAND_SIZE; i++) {
        add_card(&my_hand, &my_deck);
        printf("Drew: ");
        print_card(&my_hand.cards[i]);
        printf("\n");
        printf("Cards in my hand = %d\n", my_hand.cards_in_hand);
    }
    
    printf("\nSum of cards: %d\n", sum_ranks(&my_hand));
    // printf("\nDrawing until empty:\n");
    // while (my_deck.cards_remaining > 0) {
    //     Card drawn = draw_card(&my_deck);
    //     print_card(&drawn);
    //     printf(" ");
    // }
    // printf("\n");
    
    // print_deck_status(&my_deck);
    
    // // Try to reshuffle and draw again
    // printf("\nReshuffling empty deck...\n");
    // // init_deck(&my_deck);  
    // shuffle_deck(&my_deck);
    // draw_card(&my_deck);
    
    // printf("After reshuffling:\n");
    // print_deck_status(&my_deck);
    
    // printf("Drawing 3 more cards:\n");
    // for (int i = 0; i < 3; i++) {
    //     Card drawn = draw_card(&my_deck);
    //     printf("Drew: ");
    //     print_card(&drawn);
    //     printf("\n");
    // }
    
    print_deck_status(&my_deck);
    
    return 0;
}
