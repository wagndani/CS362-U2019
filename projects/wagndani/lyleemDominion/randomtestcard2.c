/*
 * CS362 Software Engineering II
 * Summer 2019
 * 
 * Daniel S Wagner 
 * wagndani@oregonstate.edu
 * 
 * Assignment 4
 * July 26, 2019
 * 
 * randomtestcard2.c
 * Random Testing  the refactored minion case function
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

void nb_assertUpdate_minion(char*, int, int, int, int, int, struct gameState *, struct gameState *);
void realisticRandomInitialState(struct gameState *);
int countEstatesInHand(struct gameState *);
int countEstateInDiscard(struct gameState *);

int main(){
    
    struct gameState baseGame, testGame;
   
    // Variable to pass to card functions
    int choice1 = 0,
        handPos = 0,
        choice2 = 0,
        i;
        
    int run;

    
    realisticRandomInitialState(&baseGame);
    
    memcpy(&testGame, &baseGame, sizeof(struct gameState));
    
    for(run = 0; run < 1000; run++){
        realisticRandomInitialState(&baseGame);
        memcpy(&testGame, &baseGame, sizeof(struct gameState));
        
        if(run % 2 == 0){
            choice1 = 0;
        }
        else{
            choice1 = floor(Random() * INT_MAX) - floor(Random() * INT_MAX);
        }
        choice2 = floor(Random() * INT_MAX) - floor(Random() * INT_MAX);
        handPos = floor(Random() * baseGame.handCount[baseGame.whoseTurn]);
        
        printf("\nCalling executeMinionCard: %d, number of player: %d, handPos: %d, handCount %d, choice1: %d, choice2: %d\n", 
                run + 1, testGame.numPlayers, handPos, testGame.handCount[testGame.whoseTurn], choice1, choice2);
        executeMinionCard(choice1, choice2, &testGame, handPos, testGame.whoseTurn);
 
        //If player elects to discard estate card
        if(choice1){
            nb_assertUpdate_minion("Checking number of action", testGame.numActions, baseGame.numActions + 1, i, choice1, choice2, &testGame, &baseGame);
            nb_assertUpdate_minion("Checking number of coins", testGame.coins, baseGame.coins + 2, i, choice1, choice2, &testGame, &baseGame);
            for(i = 0; i < baseGame.numPlayers; i++){
                nb_assertUpdate_minion("Verifying number of cards in player deck did not change", 
                                       testGame.deckCount[i], baseGame.deckCount[i], i, choice1, choice2, &testGame, &baseGame);
                
                if(i == baseGame.whoseTurn){
                    nb_assertUpdate_minion("Verifying number of cards in current player hand did not change, except discard of handPos", 
                                            testGame.handCount[i], baseGame.handCount[i] -1, i, choice1, choice2, &testGame, &baseGame);
                    nb_assertUpdate_minion("Verifying number of cards in current player discard incremented by 1", 
                                            testGame.discardCount[i], baseGame.discardCount[i] + 1, i, choice1, choice2, &testGame, &baseGame);
                }
                else{
                    nb_assertUpdate_minion("Verifying number of cards in other player hand did not change", 
                                            testGame.handCount[i], baseGame.handCount[i], i, choice1, choice2, &testGame, &baseGame);
                    nb_assertUpdate_minion("Verifying number of cards in player discard did not change", 
                                            testGame.discardCount[i], baseGame.discardCount[i], i, choice1, choice2, &testGame, &baseGame);
                }
            }
        }
        
        else if(choice2){
            nb_assertUpdate_minion("Checking number of action", testGame.numActions, baseGame.numActions + 1, baseGame.whoseTurn, choice1, choice2, &testGame, &baseGame);
            nb_assertUpdate_minion("Checking number of coins", testGame.coins, baseGame.coins, baseGame.whoseTurn, choice1, choice2, &testGame, &baseGame);
            for(i = 0; i < baseGame.numPlayers; i++){
                if(baseGame.handCount[i] > 4 || i == baseGame.whoseTurn){
                    if(baseGame.deckCount[i] > 4){
                        nb_assertUpdate_minion("Checking number of cards in players hand", testGame.handCount[i], 4, i, choice1, choice2, &testGame, &baseGame);
                        nb_assertUpdate_minion("Checking number of cards in players deck", testGame.deckCount[i], baseGame.deckCount[i] - 4, i, choice1, choice2, &testGame, &baseGame);
                        nb_assertUpdate_minion("Checking number of card in players discard", testGame.discardCount[i], baseGame.discardCount[i] + baseGame.handCount[i],
                                                i, choice1, choice2, &testGame, &baseGame);
                    }
                    else if((baseGame.handCount[i] + baseGame.deckCount[i] + baseGame.discardCount[i]) > 4){
                        nb_assertUpdate_minion("Checking number of cards in players hand", testGame.handCount[i], 4, i, choice1, choice2, &testGame, &baseGame);
                        nb_assertUpdate_minion("Checking number of cards in players deck", testGame.deckCount[i], 
                                                baseGame.handCount[i] + baseGame.discardCount[i] + baseGame.deckCount[i] - 4, i, choice1, choice2, &testGame, &baseGame);
                        nb_assertUpdate_minion("Checking number of cards in players discard", testGame.discardCount[i], 0, i, choice1, choice2, &testGame, &baseGame); 
                    }
                    else{
                        nb_assertUpdate_minion("Checking number of cards in players hand", testGame.handCount[i],
                                                baseGame.handCount[i] + baseGame.deckCount[i] + baseGame.discardCount[i], i, choice1, choice2, &testGame, &baseGame);
                        nb_assertUpdate_minion("Checking number of cards in players deck", testGame.deckCount[i], 0, i, choice1, choice2, &testGame, &baseGame);
                        nb_assertUpdate_minion("Checking number of card in players discard", testGame.discardCount[i], 0, i, choice1, choice2, &testGame, &baseGame);
                    }   
                } // End of 'if player has at least 5 cards in hand or player[i] is current player
            } // End of for loop that iterates though number of players 
        } // End of else if choice2
    } // End of random generation iteration loop
    
    return 0;
}

void nb_assertUpdate_minion(char* ParameterTest, int test, int base, int playerNum, int choice1, int choice2, struct gameState * T, struct gameState *B){
    
    if(base != test){
        printf("\nTEST FAILED: %s. Result: %d; Expected: %d\n"
                "Input Variables --> "
                "Player: %d; choice1: %d; choice2: %d\n",
                ParameterTest, test, base, playerNum, choice1, choice2);
    }
    else{
        printf("%s == TEST PASSED!\n", ParameterTest);
    }
    
    return;
}


void realisticRandomInitialState(struct gameState *B){

    B->numPlayers = floor(Random() * MAX_PLAYERS);
    B->whoseTurn = floor(Random() * B->numPlayers);
    B->numActions = floor(Random() * INT_MAX);
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