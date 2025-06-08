#ifndef MUTATION_H
#define MUTATION_H

#include "sudoku.h"

// Genetic operators - Mutation
void mutate(SudokuBoard* board, MutationType type);
void rowSwapMutation(SudokuBoard* board);
void blockSwapMutation(SudokuBoard* board);
void randomChangeMutation(SudokuBoard* board);

#endif