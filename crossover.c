#include "crossover.h"
#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"


// === CROSSOVER OPERATORS ===

void crossover(SudokuBoard* parent1, SudokuBoard* parent2,
              SudokuBoard* child, CrossoverType type) {
    switch(type) {
        case SINGLE_POINT:
            singlePointCrossover(parent1, parent2, child);
            break;
        case MULTI_POINT:
            multiPointCrossover(parent1, parent2, child);
            break;
        case UNIFORM:
            uniformCrossover(parent1, parent2, child);
            break;
        default:
            singlePointCrossover(parent1, parent2, child);
    }
}

// Single point crossover - replacement of one 3x3 block
void singlePointCrossover(SudokuBoard* parent1, SudokuBoard* parent2,
                         SudokuBoard* child) {
    copyBoard(parent1, child);

    // Select a random 3x3 block
    int blockRow = rand() % 3;
    int blockCol = rand() % 3;

    // Copy block from other parent
    for(int i = 0; i < BLOCK_SIZE; i++) {
        for(int j = 0; j < BLOCK_SIZE; j++) {
            int row = blockRow * BLOCK_SIZE + i;
            int col = blockCol * BLOCK_SIZE + j;
            if(!isFixed(child, row, col)) {
                child->board[row][col] = parent2->board[row][col];
            }
        }
    }

    calculateFitness(child);
}

// Multi point crossover -replacing multiple 3x3 blocks
void multiPointCrossover(SudokuBoard* parent1, SudokuBoard* parent2,
                        SudokuBoard* child) {
    copyBoard(parent1, child);

    // For each 3x3 block
    for(int blockRow = 0; blockRow < 3; blockRow++) {
        for(int blockCol = 0; blockCol < 3; blockCol++) {
            // 50% chance to replace a block
            if(rand() % 2 == 0) {
                for(int i = 0; i < BLOCK_SIZE; i++) {
                    for(int j = 0; j < BLOCK_SIZE; j++) {
                        int row = blockRow * BLOCK_SIZE + i;
                        int col = blockCol * BLOCK_SIZE + j;
                        if(!isFixed(child, row, col)) {
                            child->board[row][col] = parent2->board[row][col];
                        }
                    }
                }
            }
        }
    }

    calculateFitness(child);
}

// Uniform crossover - random selection of parent for each cell
void uniformCrossover(SudokuBoard* parent1, SudokuBoard* parent2,
                     SudokuBoard* child) {
    copyBoard(parent1, child);

    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(!isFixed(child, i, j) && rand() % 2 == 0) {
                child->board[i][j] = parent2->board[i][j];
            }
        }
    }

    calculateFitness(child);
}
