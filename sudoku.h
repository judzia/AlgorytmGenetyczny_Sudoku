#ifndef ALGORYTMGENETYCZNYSUDOKU_SUDOKU_H
#define ALGORYTMGENETYCZNYSUDOKU_SUDOKU_H

// Board configuration
#define BOARD_SIZE 9        // Size of Sudoku board (9x9)
#define BLOCK_SIZE 3        // Size of 3x3 blocks
#define POPULATION_SIZE 200 // Number of individuals in population

// Genetic Algorithm parameters
#define MAX_GENERATIONS 1000
#define CROSSOVER_RATE 0.9
#define MUTATION_PROBABILITY 0.05
#define MUTATION_RATE 0.2
#define TOURNAMENT_SIZE 10
#define ELITE_COUNT 8       // Number of best individuals to preserve

// Selection types
typedef enum {
    TOURNAMENT,  // Tournament selection
    ROULETTE,   // Roulette wheel selection
    RANKING     // Ranking selection
} SelectionType;

// Crossover types
typedef enum {
    SINGLE_POINT,    // Single point crossover (3x3 block exchange)
    MULTI_POINT,     // Multi point crossover (multiple blocks)
    UNIFORM         // Uniform crossover
} CrossoverType;

// Mutation types
typedef enum {
    ROW_SWAP,       // Swap numbers in a row
    BLOCK_SWAP,     // Swap 3x3 blocks
    RANDOM_CHANGE   // Change single random number
} MutationType;

// Structure representing a Sudoku board
typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];   // 9x9 Sudoku grid
    int initial[BOARD_SIZE][BOARD_SIZE]; // Initial board state (fixed numbers)
    int fitness;                         // Fitness value (higher is better)
} SudokuBoard;

// Structure representing a population of Sudoku boards
typedef struct {
    SudokuBoard* boards;  // Array of Sudoku boards
    int size;            // Population size
} Population;

// Initialization functions
void initializeBoard(SudokuBoard* board);
Population* createPopulation(void);
void destroyPopulation(Population* population);
void setInitialPuzzle(SudokuBoard* board, const int puzzle[BOARD_SIZE][BOARD_SIZE]);

// Fitness evaluation
int calculateFitness(SudokuBoard* board);
int isValidSolution(SudokuBoard* board);
int countConflicts(SudokuBoard* board);

// Genetic operators - Selection
SudokuBoard selectParent(Population* population, SelectionType type);
SudokuBoard tournamentSelection(Population* population, int tournamentSize);
SudokuBoard rouletteSelection(Population* population);
SudokuBoard rankingSelection(Population* population);

// Genetic operators - Crossover
void crossover(SudokuBoard* parent1, SudokuBoard* parent2, 
              SudokuBoard* child, CrossoverType type);
void singlePointCrossover(SudokuBoard* parent1, SudokuBoard* parent2, 
                         SudokuBoard* child);
void multiPointCrossover(SudokuBoard* parent1, SudokuBoard* parent2, 
                        SudokuBoard* child);
void uniformCrossover(SudokuBoard* parent1, SudokuBoard* parent2, 
                     SudokuBoard* child);

// Genetic operators - Mutation
void mutate(SudokuBoard* board, MutationType type);
void rowSwapMutation(SudokuBoard* board);
void blockSwapMutation(SudokuBoard* board);
void randomChangeMutation(SudokuBoard* board);

// Helper functions
void printBoard(SudokuBoard* board);
void copyBoard(SudokuBoard* source, SudokuBoard* destination);
int isValidNumber(SudokuBoard* board, int row, int col, int num);
int isFixed(SudokuBoard* board, int row, int col);

// Evolution function
SudokuBoard evolve(Population* population, 
                  SelectionType selType,
                  CrossoverType crossType,
                  MutationType mutType);

#endif //ALGORYTMGENETYCZNYSUDOKU_SUDOKU_H