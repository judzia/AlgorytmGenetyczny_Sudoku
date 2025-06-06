#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Example Sudoku puzzle (0 represents empty cells)
const int INITIAL_PUZZLE[BOARD_SIZE][BOARD_SIZE] = {
    {5,3,0, 0,7,0, 0,0,0},
    {6,0,0, 1,9,5, 0,0,0},
    {0,9,8, 0,0,0, 0,6,0},
    
    {8,0,0, 0,6,0, 0,0,3},
    {4,0,0, 8,0,3, 0,0,1},
    {7,0,0, 0,2,0, 0,0,6},
    
    {0,6,0, 0,0,0, 2,8,0},
    {0,0,0, 4,1,9, 0,0,5},
    {0,0,0, 0,8,0, 0,7,9}
};

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

// Create initial population
Population* createPopulation(void) {
    Population* population = (Population*)malloc(sizeof(Population));
    if(!population) {
        fprintf(stderr, "Failed to allocate population!\n");
        exit(1);
    }
    
    population->size = POPULATION_SIZE;
    population->boards = (SudokuBoard*)malloc(POPULATION_SIZE * sizeof(SudokuBoard));
    if(!population->boards) {
        fprintf(stderr, "Failed to allocate boards!\n");
        free(population);
        exit(1);
    }
    
    for(int i = 0; i < POPULATION_SIZE; i++) {
        initializeBoard(&population->boards[i]);
    }
    
    return population;
}

void destroyPopulation(Population* population) {
    if(population) {
        free(population->boards);
        free(population);
    }
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

// === SELECTION OPERATORS ===

SudokuBoard selectParent(Population* population, SelectionType type) {
    switch(type) {
        case TOURNAMENT:
            return tournamentSelection(population, TOURNAMENT_SIZE);
        case ROULETTE:
            return rouletteSelection(population);
        case RANKING:
            return rankingSelection(population);
        default:
            return tournamentSelection(population, TOURNAMENT_SIZE);
    }
}

// Tournament selection - wybiera najlepszego z losowej grupy
SudokuBoard tournamentSelection(Population* population, int tournamentSize) {
    int bestIndex = rand() % population->size;
    
    for(int i = 1; i < tournamentSize; i++) {
        int randomIndex = rand() % population->size;
        if(population->boards[randomIndex].fitness > 
           population->boards[bestIndex].fitness) {
            bestIndex = randomIndex;
        }
    }
    
    return population->boards[bestIndex];
}

// Roulette selection - probability proportional to fitness
SudokuBoard rouletteSelection(Population* population) {
    // Find the smallest fitness (to shift all values to positive)
    int minFitness = population->boards[0].fitness;
    for(int i = 1; i < population->size; i++) {
        if(population->boards[i].fitness < minFitness) {
            minFitness = population->boards[i].fitness;
        }
    }
    
    // Calculate the sum of scaled fitness values
    long totalFitness = 0;
    for(int i = 0; i < population->size; i++) {
        totalFitness += (population->boards[i].fitness - minFitness + 1);
    }
    
    // Select a random point
    long point = rand() % totalFitness;
    long sum = 0;
    
    //  Find a suitable individual
    for(int i = 0; i < population->size; i++) {
        sum += (population->boards[i].fitness - minFitness + 1);
        if(sum > point) {
            return population->boards[i];
        }
    }
    
    return population->boards[population->size - 1];
}

// Ranking selection - the probability depends on the position in the ranking
SudokuBoard rankingSelection(Population* population) {
    // Create an index table
    int indices[POPULATION_SIZE];
    for(int i = 0; i < population->size; i++) indices[i] = i;
    
    // Sort indexes by fitness (bubble sort)
    for(int i = 0; i < population->size - 1; i++) {
        for(int j = 0; j < population->size - i - 1; j++) {
            if(population->boards[indices[j]].fitness < 
               population->boards[indices[j + 1]].fitness) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
    
    // Select by rank
    int totalRank = (population->size * (population->size + 1)) / 2;
    int point = rand() % totalRank;
    int sum = 0;
    
    for(int i = 0; i < population->size; i++) {
        sum += (population->size - i);
        if(sum > point) {
            return population->boards[indices[i]];
        }
    }
    
    return population->boards[indices[0]];
}

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

// Main evolution function
SudokuBoard evolve(Population* population, 
                  SelectionType selType,
                  CrossoverType crossType,
                  MutationType mutType) {
    SudokuBoard bestEver = population->boards[0];
    int generationsWithoutImprovement = 0;
    int totalImprovements = 0;
    
    printf("\nStarting evolution...\n");
    printf("Initial fitness: %d/243 (%.1f%%)\n", 
           bestEver.fitness, (bestEver.fitness * 100.0) / 243.0);
    
    for(int gen = 0; gen < MAX_GENERATIONS; gen++) {
        SudokuBoard* newGeneration = 
            (SudokuBoard*)malloc(POPULATION_SIZE * sizeof(SudokuBoard));
        
        // Elitism
        for(int i = 0; i < ELITE_COUNT; i++) {
            int bestIdx = 0;
            for(int j = 1; j < population->size; j++) {
                if(population->boards[j].fitness > 
                   population->boards[bestIdx].fitness) {
                    bestIdx = j;
                }
            }
            copyBoard(&population->boards[bestIdx], &newGeneration[i]);
            population->boards[bestIdx].fitness = -999999;
        }
        
        // Create new generation
        SudokuBoard bestInGeneration = bestEver;
        // In the evolve function, in the main 'child' generation loop:
        for(int i = ELITE_COUNT; i < POPULATION_SIZE; i++) {
            SudokuBoard parent1 = selectParent(population, selType);
            SudokuBoard parent2 = selectParent(population, selType);
            
            // First we copy the first parent
            copyBoard(&parent1, &newGeneration[i]);
            
            // Crossover with a certain probability
            if((double)rand() / RAND_MAX < CROSSOVER_RATE) {
                crossover(&parent1, &parent2, &newGeneration[i], crossType);
            }
            
            // Mutation with a specific probability
            if((double)rand() / RAND_MAX < MUTATION_RATE) {
                mutate(&newGeneration[i], mutType);
            }
            
            if(newGeneration[i].fitness > bestInGeneration.fitness) {
                copyBoard(&newGeneration[i], &bestInGeneration);
            }
        }
        
        // Check if we have improvement
        if(bestInGeneration.fitness > bestEver.fitness) {
            copyBoard(&bestInGeneration, &bestEver);
            totalImprovements++;
            printf("\nImprovement %d (Generation %d): Fitness improved to %d/243 (%.1f%%)\n", 
                   totalImprovements, gen, bestEver.fitness, 
                   (bestEver.fitness * 100.0) / 243.0);
            
            if(bestEver.fitness >= 243) {
                printf("\nPerfect solution found!\n");
                free(population->boards);
                population->boards = newGeneration;
                return bestEver;
            }
            generationsWithoutImprovement = 0;
        } else {
            generationsWithoutImprovement++;
            if(generationsWithoutImprovement >= 300) {
                printf("\nNo improvement for 300 generations.\n");
                printf("Consider trying different genetic operators.\n");
                break;
            }
        }
        
        free(population->boards);
        population->boards = newGeneration;
    }
    
    return bestEver;
}