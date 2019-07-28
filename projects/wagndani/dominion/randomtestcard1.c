/*
 * CS362 Software Engineering II
 * Summer 2019
 * 
 * Daniel S Wagner 
 * wagndani@oregonstate.edu
 * 
 * Assignment 4
 * July 23, 2019
 * 
 * randomtestcard1.c
 * Random Testing  the refactored baron case function
 */

// STANDARD C LIBRAIRIES
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

// SPECIFIC DOMINION HEADER FILES
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

void nb_assertUpdate(char*, int, int, int, struct gameState *, struct gameState *);
void realisticRandomInitialState(struct gameState *);
int countEstatesInHand(struct gameState *);
int countEstateInDiscard(struct gameState *);

int main(){
    
    struct gameState baseGame, testGame;
   
    // Variable to pass to card functions
    int choice1 = 0,
        handPos = 0;
        
    int run;

    for(run = 0; run < 10000; run++){
        realisticRandomInitialState(&baseGame);
        memcpy(&testGame, &baseGame, sizeof(struct gameState));
        choice1 = floor(Random() * INT_MAX) - floor(Random() * INT_MAX);
        handPos = floor(Random() * baseGame.handCount[baseGame.whoseTurn]);
        
        baronCase(choice1, &testGame, handPos, testGame.whoseTurn);
        
        int estatesInHand_base,
        estatesInDiscard_base,
        estatesInHand_test,
        estatesInDiscard_test;

        estatesInHand_base = countEstatesInHand(&baseGame);
        estatesInDiscard_base = countEstateInDiscard(&baseGame);
        estatesInHand_test = countEstatesInHand(&testGame);
        estatesInDiscard_test = countEstateInDiscard(&testGame);

        //If player elects to discard estate card
        if(choice1 > 0 && estatesInHand_base){
            nb_assertUpdate("Checking number of buys", testGame.numBuys, baseGame.numBuys + 1, choice1, &testGame, &baseGame);
            nb_assertUpdate("Checking number of coins", testGame.coins, baseGame.coins + 4, choice1, &testGame, &baseGame);
            nb_assertUpdate("Checking number of estate cards in hand", estatesInHand_test, estatesInHand_base - 1, choice1, &testGame, &baseGame);
            nb_assertUpdate("Checking estate cards in discard", estatesInDiscard_test, estatesInDiscard_base + 1, choice1, &testGame, &baseGame);
            nb_assertUpdate("Checking that card in position handPos, is on top of discard",
                            testGame.discard[testGame.whoseTurn][testGame.discardCount[testGame.whoseTurn] - 1], 
                            baseGame.hand[baseGame.whoseTurn][handPos], choice1, &testGame, &baseGame);
        }
        
        else if((choice1 > 0 && !estatesInDiscard_base) || choice1 == 0){
            nb_assertUpdate("Checking number of buys", testGame.numBuys, baseGame.numBuys + 1, choice1, &testGame, &baseGame);
            nb_assertUpdate("Checking number of coins", testGame.coins, baseGame.coins, choice1, &testGame, &baseGame);
            nb_assertUpdate("Checking number of estate cards in hand", estatesInHand_test, estatesInHand_base, choice1, &testGame, &baseGame);
            nb_assertUpdate("Checking number of estate card in discard",estatesInDiscard_test, estatesInDiscard_base + 1, choice1, &testGame, &baseGame);
        }
    }
    
    return 0;
}

void nb_assertUpdate(char* ParameterTest, int test, int base, int choice1, struct gameState * T, struct gameState *B){
    
    if(base != test){
        printf("\nTEST FAILED: %s. Result: %d; Expected: %d\n"
                "Input Variables --> "
                "Player: %d; choice1: %d; Player estates in hand: %d; Estate Supply Count: %d\n",
                ParameterTest, test, base, B->whoseTurn, choice1, countEstatesInHand(B), B->supplyCount[estate]);
    }
    else{
        printf("%s == TEST PASSED!\n", ParameterTest);
    }
    
    return;
}


void realisticRandomInitialState(struct gameState *B){

    B->numPlayers = floor(Random() * MAX_PLAYERS);
    B->whoseTurn = floor(Random() * B->numPlayers);
    B->playedCardCount = floor(Random() * MAX_DECK);

    int p, cc, kc;

    //Initialize Player variables
    for(p = 0; p < B->numPlayers; p++){
        //Randomize player p handCount
        B->handCount[p] = floor(Random() * MAX_HAND);

        //Randomize cards in players handCount
        for(cc = 0; cc < B->handCount[p]; cc++){
            B->hand[p][cc] = floor(Random() * (treasure_map +1));
        }

        //Randomize player p deckCount
        B->deckCount[p] = floor(Random() * MAX_DECK);

        //Randomize cards in players deck
        for(cc = 0; cc < B->deckCount[p]; cc++){
            B->deck[p][cc] = floor(Random() * (treasure_map +1));
        }

        //Randomize player p deckCount
        B->discardCount[p] = floor(Random() * MAX_DECK);

        //Randomize cards in players discard
        for(cc = 0; cc < B->discardCount[p]; cc++){
            B->discard[p][cc] = floor(Random() * (treasure_map +1));
        }
    }

    //Initialize Random Supply Counts
    for(kc = 0; kc < treasure_map + 1; kc++){
        B->supplyCount[kc] = floor(Random() * MAX_DECK);
    }

    B->numActions = floor(Random() * INT_MAX);
    B->numBuys = floor(Random() * INT_MAX);
    B->coins = floor(Random() * INT_MAX);

    return;
}

int countEstatesInHand(struct gameState * B){
    int currentPlayer = B->whoseTurn;
    int estateCount = 0,
        i;
    
    for(i = 0; i < B->handCount[currentPlayer]; i++){
        if(B->hand[currentPlayer][i] == estate){
            estateCount++;
        }
    }
    
    return estateCount;
}

int countEstateInDiscard(struct gameState * B){
    int currentPlayer = B->whoseTurn;
    int estateDiscardCount = 0,
        i;
    
    for(i = 0; i < B->handCount[currentPlayer]; i++){
        if(B->hand[currentPlayer][i] == estate){
            estateDiscardCount++;
        }
    }
    
    return estateDiscardCount;
}