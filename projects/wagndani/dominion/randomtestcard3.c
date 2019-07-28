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
 * Random Testing  the refactored tribute case function
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

void nb_assertUpdate_tribute(char*, int, int, int, int, int, int, struct gameState *, struct gameState *);
void realisticRandomInitialState(struct gameState *);
int countEstatesInHand(struct gameState *);
int countEstateInDiscard(struct gameState *);

int main(){
    
    struct gameState baseGame, testGame;
   
    // Variable to pass to card functions
    int currentPlayer,
        nextPlayer;
    
    currentPlayer = floor(Random() * baseGame.numPlayers);
    if(currentPlayer == baseGame.numPlayers - 1){
        nextPlayer = 0;
    }
    else{
        nextPlayer = currentPlayer + 1;
    }
        
    realisticRandomInitialState(&baseGame);
    
    memcpy(&testGame, &baseGame, sizeof(struct gameState));
    
    int run, rc1, rc2;
    int tribute[2] = {-1, -1};
    int foundCards = 0;
    
    int addActions,
        addCoins,
        addDraws;
    
    for(run = 0; run < 1000; run++){
        
        addActions = addCoins = addDraws = 0;

        realisticRandomInitialState(&baseGame);
        memcpy(&testGame, &baseGame, sizeof(struct gameState));
        
        printf("\nCalling tributeCase: %d, number of player: %d, current player: %d,\n"
                "number of cards not currently in hand for current player: %d,\n"
                "number of cards not currently in hand for next player %d\n", 
                run + 1, baseGame.numPlayers, currentPlayer, 
                baseGame.deckCount[currentPlayer] + baseGame.discardCount[currentPlayer],
                baseGame.deckCount[nextPlayer] + baseGame.discardCount[nextPlayer]);
        
        tributeCase(&testGame, currentPlayer, nextPlayer);
        
        if(baseGame.deckCount[nextPlayer] >= 2){
            rc1 = baseGame.deck[nextPlayer][baseGame.deckCount[nextPlayer] -1];
            rc2 = baseGame.deck[nextPlayer][baseGame.deckCount[nextPlayer] -2];
            foundCards = 2;
            nb_assertUpdate_tribute("Checking number of cards in nextPlayers Deck", testGame.deckCount[nextPlayer], baseGame.deckCount[nextPlayer] - 2, 
                                    currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
            nb_assertUpdate_tribute("Checking number of cards in next players discard pile", testGame.discardCount[nextPlayer], baseGame.discardCount[nextPlayer] + 2, 
                                    currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
        }
        else if(baseGame.deckCount[nextPlayer] < 2 && baseGame.discardCount[nextPlayer] > 0){
            if(baseGame.deckCount[nextPlayer] == 1){
                rc1 = baseGame.deck[nextPlayer][baseGame.deckCount[nextPlayer] - 1];
                rc2 = testGame.discard[nextPlayer][testGame.discardCount[nextPlayer] - 1];
                foundCards = 2;
                nb_assertUpdate_tribute("Checking number of cards in nextPlayers Deck", testGame.deckCount[nextPlayer], baseGame.discardCount[nextPlayer], 
                                        currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
                nb_assertUpdate_tribute("Checking number of cards in next players discard pile", testGame.discardCount[nextPlayer], 1, 
                                        currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
            }
            else if(baseGame.deckCount[nextPlayer] == 0 && baseGame.discardCount[nextPlayer] >= 2){
                rc1 = testGame.discard[nextPlayer][testGame.discardCount[nextPlayer] - 2];
                rc2 = testGame.discard[nextPlayer][testGame.discardCount[nextPlayer] - 1];
                foundCards = 2;
                nb_assertUpdate_tribute("Checking number of cards in nextPlayers Deck", testGame.deckCount[nextPlayer], baseGame.discardCount[nextPlayer] - 2, 
                                        currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
                nb_assertUpdate_tribute("Checking number of cards in next players discard pile", testGame.discardCount[nextPlayer], 2, 
                                        currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
            }
            else if(baseGame.deckCount[nextPlayer] == 0 && baseGame.discardCount[nextPlayer] == 1){
                rc2 = rc1 = baseGame.discard[nextPlayer][baseGame.discardCount[nextPlayer] - 1];
                nb_assertUpdate_tribute("Checking number of cards in nextPlayers Deck", testGame.deckCount[nextPlayer], 1, 
                                        currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
                nb_assertUpdate_tribute("Checking number of cards in next players discard pile", testGame.discardCount[nextPlayer], 1, 
                                        currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
                foundCards = 2;
            }
            else{
                rc1 = rc2 = -1;
                foundCards = 0;
            }
        }
        
        tribute[0] = rc1;
        tribute[1] = rc2;
        
        if(tribute[0] == tribute[1] && foundCards > 1){
            tribute[1] = -1;
            foundCards = 1;
        }
        
        int c;
        for(c = 0; c < foundCards; c++){
            if(tribute[c] >= 4 && tribute[c] <= 6){
                addCoins += 2;
            }
            else if((tribute[c] >= 1 && tribute[c] <= 3) || tribute[c] == gardens || tribute[c] == great_hall){
                addDraws += 2;
            }
            else{
                addActions += 2;
            }
        }
        
        //Actual Check Cases
        nb_assertUpdate_tribute("Checking number of actions", testGame.numActions, baseGame.numActions + addActions, 
                                currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
        nb_assertUpdate_tribute("Checking number of coins", testGame.coins, baseGame.coins + addCoins, 
                                currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
        if((baseGame.deckCount[currentPlayer] + baseGame.discardCount[currentPlayer]) >= addDraws){
            nb_assertUpdate_tribute("Checking number of cards in current Players hand", testGame.handCount[currentPlayer], baseGame.handCount[currentPlayer] + addDraws, 
                                    currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
        }
        else{
            nb_assertUpdate_tribute("Checking number of cards in current players hand", testGame.handCount[currentPlayer], 
                                    baseGame.handCount[currentPlayer] + baseGame.deckCount[currentPlayer] + baseGame.discardCount[currentPlayer], 
                                    currentPlayer, nextPlayer, rc1, rc2, &testGame, &baseGame);
        }

    } // End of random generation iteration loop
    
    return 0;
}

void nb_assertUpdate_tribute(char* ParameterTest, int test, int base, int cp, int np, int rc1, int rc2, struct gameState * T, struct gameState *B){
    
    if(base != test){
        printf("\nTEST FAILED: %s. Result: %d; Expected: %d\n"
                "Input Variables --> "
                "Number of players: %d, Current Player: %d, Next Player %d, Reveal1: %d, Reveal2: %d \n",
                ParameterTest, test, base, B->numPlayers, cp, np, rc1, rc2);
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