#include "board_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

// Helper Functions
void copyBoard(SudokuBoard* source, SudokuBoard* destination) {
    memcpy(destination->board, source->board, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
    memcpy(destination->initial, source->initial, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
    destination->fitness = source->fitness;
}

int isFixed(SudokuBoard* board, int row, int col) {
    return board->initial[row][col] != 0;
}

void setInitialPuzzle(SudokuBoard* board, const int puzzle[BOARD_SIZE][BOARD_SIZE]) {
    memcpy(board->initial, puzzle, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
    memcpy(board->board, puzzle, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
}

int isValidNumber(SudokuBoard* board, int row, int col, int num) {
    // Check row
    for(int j = 0; j < BOARD_SIZE; j++) {
        if(j != col && board->board[row][j] == num) return 0;
    }

    // Check column
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(i != row && board->board[i][col] == num) return 0;
    }

    // Check 3x3 block
    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if((blockRow + i != row || blockCol + j != col) &&
               board->board[blockRow + i][blockCol + j] == num) return 0;
        }
    }

    return 1;
}

// Initialize board with random valid numbers
void initializeBoard(SudokuBoard* board) {
    // Set initial puzzle
    setInitialPuzzle(board, INITIAL_PUZZLE);

    // Fill empty cells with random valid numbers
    for(int row = 0; row < BOARD_SIZE; row++) {
        for(int col = 0; col < BOARD_SIZE; col++) {
            if(board->initial[row][col] == 0) {
                int numbers[9] = {1,2,3,4,5,6,7,8,9};
                // Shuffle numbers
                for(int i = 8; i > 0; i--) {
                    int j = rand() % (i + 1);
                    int temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }

                // Try numbers until finding a valid one
                for(int i = 0; i < 9; i++) {
                    if(isValidNumber(board, row, col, numbers[i])) {
                        board->board[row][col] = numbers[i];
                        break;
                    }
                }

                // If no valid number found, just put any number (will be fixed by evolution)
                if(board->board[row][col] == 0) {
                    board->board[row][col] = 1 + (rand() % 9);
                }
            }
        }
    }

    calculateFitness(board);
}

// Count conflicts in the board
int countConflicts(SudokuBoard* board) {
    int conflicts = 0;

    // Check rows
    for(int row = 0; row < BOARD_SIZE; row++) {
        int used[10] = {0};  // 0-based indexing for numbers 1-9
        for(int col = 0; col < BOARD_SIZE; col++) {
            int num = board->board[row][col];
            if(num > 0) used[num]++;
        }
        for(int i = 1; i <= 9; i++) {
            if(used[i] > 1) conflicts += (used[i] - 1);
        }
    }

    // Check columns
    for(int col = 0; col < BOARD_SIZE; col++) {
        int used[10] = {0};
        for(int row = 0; row < BOARD_SIZE; row++) {
            int num = board->board[row][col];
            if(num > 0) used[num]++;
        }
        for(int i = 1; i <= 9; i++) {
            if(used[i] > 1) conflicts += (used[i] - 1);
        }
    }

    // Check 3x3 blocks
    for(int blockRow = 0; blockRow < 3; blockRow++) {
        for(int blockCol = 0; blockCol < 3; blockCol++) {
            int used[10] = {0};
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    int num = board->board[blockRow*3 + i][blockCol*3 + j];
                    if(num > 0) used[num]++;
                }
            }
            for(int i = 1; i <= 9; i++) {
                if(used[i] > 1) conflicts += (used[i] - 1);
            }
        }
    }

    return conflicts;
}

int calculateFitness(SudokuBoard* board) {
    int fitness = 0;

    // Check rows (max 9 points per row, total 81 points possible)
    for(int row = 0; row < BOARD_SIZE; row++) {
        int used[10] = {0};
        for(int col = 0; col < BOARD_SIZE; col++) {
            int num = board->board[row][col];
            if(num > 0 && used[num] == 0) {
                used[num] = 1;
                fitness++;
            }
        }
    }

    // Check columns (max 9 points per column, total 81 points possible)
    for(int col = 0; col < BOARD_SIZE; col++) {
        int used[10] = {0};
        for(int row = 0; row < BOARD_SIZE; row++) {
            int num = board->board[row][col];
            if(num > 0 && used[num] == 0) {
                used[num] = 1;
                fitness++;
            }
        }
    }

    // Check 3x3 blocks (max 9 points per block, total 81 points possible)
    for(int blockRow = 0; blockRow < 3; blockRow++) {
        for(int blockCol = 0; blockCol < 3; blockCol++) {
            int used[10] = {0};
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    int num = board->board[blockRow*3 + i][blockCol*3 + j];
                    if(num > 0 && used[num] == 0) {
                        used[num] = 1;
                        fitness++;
                    }
                }
            }
        }
    }

    board->fitness = fitness;
    return fitness;
}

// Check if solution is valid
int isValidSolution(SudokuBoard* board) {
    return countConflicts(board) == 0;
}

// Print board in a nice format
void printBoard(SudokuBoard* board) {
    printf("\n");
    printf("    1 2 3   4 5 6   7 8 9\n");
    printf("  +-----------------+\n");

    for(int i = 0; i < BOARD_SIZE; i++) {
        if(i % 3 == 0 && i != 0) {
            printf("  +-----------------+\n");
        }
        printf("%d | ", i + 1);

        for(int j = 0; j < BOARD_SIZE; j++) {
            if(j % 3 == 0 && j != 0) {
                printf("| ");
            }

            if(isFixed(board, i, j)) {
                printf("%d ", board->board[i][j]);  // Fixed numbers
            } else {
                printf("%d ", board->board[i][j]);  // Generated numbers
            }
        }
        printf("|\n");
    }
    printf("  +-----------------+\n");

    // Print stats
    printf("\nFitness: %d/243 (%.1f%%)\n",
           board->fitness,
           (board->fitness * 100.0) / 243.0);
}
