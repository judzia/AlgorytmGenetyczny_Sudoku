#ifndef SELECTION_H
#define SELECTION_H

#include "sudoku.h"

// Genetic operators - Selection
SudokuBoard selectParent(Population* population, SelectionType type);
SudokuBoard tournamentSelection(Population* population, int tournamentSize);
SudokuBoard rouletteSelection(Population* population);
SudokuBoard rankingSelection(Population* population);

#endif