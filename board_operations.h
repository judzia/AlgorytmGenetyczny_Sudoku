#ifndef BOARD_OPERATIONS_H
#define BOARD_OPERATIONS_H

#include "sudoku.h"

// Basic functions for the board
void copyBoard(SudokuBoard* source, SudokuBoard* destination);
int isFixed(SudokuBoard* board, int row, int col);
void setInitialPuzzle(SudokuBoard* board, const int puzzle[BOARD_SIZE][BOARD_SIZE]);
int isValidNumber(SudokuBoard* board, int row, int col, int num);
int calculateFitness(SudokuBoard* board);
int countConflicts(SudokuBoard* board);
int isValidSolution(SudokuBoard* board);
void printBoard(SudokuBoard* board);
void initializeBoard(SudokuBoard* board);

// Fixed Starting Board
extern const int INITIAL_PUZZLE[BOARD_SIZE][BOARD_SIZE];

#endif