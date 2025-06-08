#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "sudoku.h"

SudokuBoard evolve(Population* population, 
                  SelectionType selType,
                  CrossoverType crossType,
                  MutationType mutType);

#endif