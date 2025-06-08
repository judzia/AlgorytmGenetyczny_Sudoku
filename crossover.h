#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "sudoku.h"

// Genetic operators - Crossover
void crossover(SudokuBoard* parent1, SudokuBoard* parent2, 
              SudokuBoard* child, CrossoverType type);
void singlePointCrossover(SudokuBoard* parent1, SudokuBoard* parent2, 
                         SudokuBoard* child);
void multiPointCrossover(SudokuBoard* parent1, SudokuBoard* parent2, 
                        SudokuBoard* child);
void uniformCrossover(SudokuBoard* parent1, SudokuBoard* parent2, 
                     SudokuBoard* child);

#endif