/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// STANDARD C LIBRAIRIES
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// SPECIFIC DOMINION HEADER FILES
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

void nb_assert(int, int);
void nb_assert_test_GTE(int, int);

int main(){
    
    // Variables to initialize game --START
    int checkPlayer = 0;
    int players[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int playersAllHigh[10] = {99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
    
    struct gameState baseGame;
    
    baseGame.whoseTurn = 0;
        
    baseGame.numPlayers = 2;
    baseGame.handCount[checkPlayer] = 5;
    baseGame.deckCount[checkPlayer] = 5;
    baseGame.discardCount[checkPlayer] = 5;
    baseGame.handCount[checkPlayer +1] = 5;
    baseGame.deckCount[checkPlayer + 1] = 5;
    baseGame.discardCount[checkPlayer +1] = 5;
    
    int i = 0;
    for(i = 0; i < 5; i++){
        baseGame.hand[checkPlayer][i] = estate;
        baseGame.deck[checkPlayer][i] = estate;
        baseGame.discard[checkPlayer][i] = estate;
        
        baseGame.hand[checkPlayer +1][i] = province;
        baseGame.deck[checkPlayer +1][i] = province;
        baseGame.discard[checkPlayer +1][i] = province;
    }
    
    printf("\nTESTING GETWINNDERS FUNCTION\n");
    
    printf("Testing winning player array.  Should be position 1\n");
    getWinners(players, &baseGame);
    nb_assert(players[1], 1);
    
    printf("Testing player 0.  Should be 0\n");
    nb_assert(players[0], 0);
    
    printf("Testing player position greater than number of players.  Should be 0\n");
    nb_assert(players[8], 0);
    
    printf("\nTesting array populated with all '99'.  Position 1 should be 1\n");
    getWinners(playersAllHigh, &baseGame);
    nb_assert(playersAllHigh[1], 1);
    
    printf("Testing array populated with all '99'.  Position 0 Should be 0\n");
    nb_assert(playersAllHigh[0], 0);
    
    printf("Testing array populated with all '99'.  Position 8 Should be 0\n");
    nb_assert(playersAllHigh[8], 0);
    
    
    printf("Testing array populated with all '99'.  Position 8 Should be 0\n");
    nb_assert(playersAllHigh[8], 0);
    
    printf("\nTesting tie game when player in position i has less turns.\n");
    
    for(i = 0; i < 10; i++){
        players[i] = 0;
    }
    
    for(i = 0; i < 5; i++){
        baseGame.hand[checkPlayer][i] = estate;
        baseGame.deck[checkPlayer][i] = estate;
        baseGame.discard[checkPlayer][i] = estate;
        
        baseGame.hand[checkPlayer +1][i] = estate;
        baseGame.deck[checkPlayer +1][i] = estate;
        baseGame.discard[checkPlayer +1][i] = estate;
    }
    
    getWinners(players, &baseGame);
    
    printf("Player in position 1 should have value 1.\n");
    nb_assert(players[1], 1);
    
    printf("Player in position 0 should have value 0.\n");
    nb_assert(players[0], 0);
    
    printf("\nTesting tie game when both players have equal turns.\n");
    
    for(i = 0; i < 10; i++){
        players[i] = 0;
    }
    baseGame.whoseTurn = 1;

    for(i = 0; i < 5; i++){
        baseGame.hand[checkPlayer][i] = estate;
        baseGame.deck[checkPlayer][i] = estate;
        baseGame.discard[checkPlayer][i] = estate;
        
        baseGame.hand[checkPlayer +1][i] = estate;
        baseGame.deck[checkPlayer +1][i] = estate;
        baseGame.discard[checkPlayer +1][i] = estate;
    }
    
    getWinners(players, &baseGame);
    
    printf("Player in position 1 should have value 1.\n");
    nb_assert(players[1], 1);
    
    printf("Player in position 0 should have value 1.\n");
    nb_assert(players[1], 1);

    return 0;
}

void nb_assert(int test, int base){
    
       printf("Expected %d, result was %d\n", base, test);
    if(base == test){
        printf("TEST PASSED\n");
    }
    
    else{
        printf("TEST FAILED\n");
    }
    
    return;
}

void nb_assert_test_GTE(int test, int base){
    
       printf("Expected at least %d, result was %d\n", base, test);
    if(base <= test){
        printf("TEST PASSED\n");
    }
    
    else{
        printf("TEST FAILED\n");
    }
    
    return;
}

