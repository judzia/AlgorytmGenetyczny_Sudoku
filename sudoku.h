#ifndef ALGORYTMGENETYCZNYSUDOKU_SUDOKU_H
#define ALGORYTMGENETYCZNYSUDOKU_SUDOKU_H

#define BOARD_SIZE 9
#define BLOCK_SIZE 3
#define POPULATION_SIZE 100

// Struktura reprezentująca planszę Sudoku
typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
    int fitness;
} SudokuBoard;

// Struktura reprezentująca populację
typedef struct {
    SudokuBoard* boards;
    int size;
} Population;

// Funkcje do inicjalizacji
void initializeBoard(SudokuBoard* board);
Population* createPopulation(void);
void destroyPopulation(Population* population);

// Funkcje oceny
int calculateFitness(SudokuBoard* board);

// Operatory genetyczne
SudokuBoard selectParent(Population* population);
void crossover(SudokuBoard* parent1, SudokuBoard* parent2, SudokuBoard* child);
void mutate(SudokuBoard* board);

// Funkcje pomocnicze
void printBoard(SudokuBoard* board);
int isValidSolution(SudokuBoard* board);

#endif //ALGORYTMGENETYCZNYSUDOKU_SUDOKU_H