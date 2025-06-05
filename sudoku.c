#include "sudoku.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void initializeBoard(SudokuBoard* board) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            board->board[i][j] = 0;
        }
    }
    board->fitness = 0;
}

Population* createPopulation(void) {
    Population* population = (Population*)malloc(sizeof(Population));
    population->size = POPULATION_SIZE;
    population->boards = (SudokuBoard*)malloc(POPULATION_SIZE * sizeof(SudokuBoard));

    for(int i = 0; i < POPULATION_SIZE; i++) {
        initializeBoard(&population->boards[i]);
    }

    return population;
}

void destroyPopulation(Population* population) {
    if (population) {
        free(population->boards);
        free(population);
    }
}

void printBoard(SudokuBoard* board) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(i % BLOCK_SIZE == 0 && i != 0) {
            printf("---------------------\n");
        }
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(j % BLOCK_SIZE == 0 && j != 0) {
                printf("| ");
            }
            printf("%d ", board->board[i][j]);
        }
        printf("\n");
    }
    printf("\nFitness: %d\n", board->fitness);
}