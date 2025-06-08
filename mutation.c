#include "mutation.h"
#include <stdlib.h>
#include "sudoku.h"


// === MUTATION OPERATORS ===

void mutate(SudokuBoard* board, MutationType type) {
    switch(type) {
        case ROW_SWAP:
            rowSwapMutation(board);
            break;
        case BLOCK_SWAP:
            blockSwapMutation(board);
            break;
        case RANDOM_CHANGE:
            randomChangeMutation(board);
            break;
        default:
            rowSwapMutation(board);
    }
}

// Row swap mutation - swapping two numbers in a row
void rowSwapMutation(SudokuBoard* board) {
    // Select a random row
    int row = rand() % BOARD_SIZE;

    // Find two positions that are not fixed
    int col1, col2;
    do {
        col1 = rand() % BOARD_SIZE;
    } while(isFixed(board, row, col1));

    do {
        col2 = rand() % BOARD_SIZE;
    } while(isFixed(board, row, col2) || col1 == col2);

    // Swap the numbers
    int temp = board->board[row][col1];
    board->board[row][col1] = board->board[row][col2];
    board->board[row][col2] = temp;

    calculateFitness(board);
}

// Block swap mutation - swapping two 3x3 blocks
void blockSwapMutation(SudokuBoard* board) {
    // Select two different blocks in the same row or column
    int blockRow = rand() % 3;
    int block1Col = rand() % 3;
    int block2Col;
    do {
        block2Col = rand() % 3;
    } while(block1Col == block2Col);

    // Swap blocks (unblocked cells only)
    for(int i = 0; i < BLOCK_SIZE; i++) {
        for(int j = 0; j < BLOCK_SIZE; j++) {
            int row = blockRow * BLOCK_SIZE + i;
            int col1 = block1Col * BLOCK_SIZE + j;
            int col2 = block2Col * BLOCK_SIZE + j;

            if(!isFixed(board, row, col1) && !isFixed(board, row, col2)) {
                int temp = board->board[row][col1];
                board->board[row][col1] = board->board[row][col2];
                board->board[row][col2] = temp;
            }
        }
    }

    calculateFitness(board);
}

// Random change mutation - changing a random number
void randomChangeMutation(SudokuBoard* board) {
    // Select a random item
    int row, col;
    do {
        row = rand() % BOARD_SIZE;
        col = rand() % BOARD_SIZE;
    } while(isFixed(board, row, col));

    // Select new random value
    int oldValue = board->board[row][col];
    int newValue;
    do {
        newValue = 1 + (rand() % 9);
    } while(newValue == oldValue);

    board->board[row][col] = newValue;
    calculateFitness(board);
}

