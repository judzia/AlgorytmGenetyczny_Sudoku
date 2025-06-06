#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sudoku.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showCurrentSettings(SelectionType sel, CrossoverType cross, MutationType mut) {
    printf("\nCurrent settings:\n");
    printf("Selection: %s\n", 
           sel == TOURNAMENT ? "Tournament" : 
           sel == ROULETTE ? "Roulette" : "Ranking");
    printf("Crossover: %s\n",
           cross == SINGLE_POINT ? "Single Point" :
           cross == MULTI_POINT ? "Multi Point" : "Uniform");
    printf("Mutation: %s\n",
           mut == ROW_SWAP ? "Row Swap" :
           mut == BLOCK_SWAP ? "Block Swap" : "Random Change");
}

void showSelectionMenu() {
    clearScreen();
    printf("Select Selection Method:\n");
    printf("1. Tournament Selection (selects best from random group)\n");
    printf("2. Roulette Wheel Selection (probability based on fitness)\n");
    printf("3. Ranking Selection (probability based on rank)\n");
}

void showCrossoverMenu() {
    clearScreen();
    printf("Select Crossover Method:\n");
    printf("1. Single Point (3x3 block exchange)\n");
    printf("2. Multi Point (multiple blocks)\n");
    printf("3. Uniform (random for each cell)\n");
}

void showMutationMenu() {
    clearScreen();
    printf("Select Mutation Method:\n");
    printf("1. Row Swap (swap numbers in a row)\n");
    printf("2. Block Swap (swap 3x3 blocks)\n");
    printf("3. Random Change (change random number)\n");
}


int main(void) {
    srand(time(NULL));

    SelectionType selectionType = TOURNAMENT;
    CrossoverType crossoverType = SINGLE_POINT;
    MutationType mutationType = ROW_SWAP;
    char input[10];

    while(1) {
        clearScreen();
        printf("=== SUDOKU SOLVER - GENETIC ALGORITHM ===\n\n");
        printf("Configure genetic operators:\n");
        showCurrentSettings(selectionType, crossoverType, mutationType);
        
        printf("\nOptions:\n");
        printf("1. Change Selection Method\n");
        printf("2. Change Crossover Method\n");
        printf("3. Change Mutation Method\n");
        printf("4. Start Solving\n");
        printf("5. Exit\n");
        
        printf("\nEnter your choice (1-5): ");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        input[strcspn(input, "\n")] = 0;

        if(input[0] == '5') {
            printf("\nThank you for using Sudoku Solver!\n");
            return 0;
        }

        switch(input[0]) {
            case '1': {
                showSelectionMenu();
                printf("\nEnter your choice (1-3): ");
                fflush(stdout);
                if (fgets(input, sizeof(input), stdin) == NULL) continue;
                switch(input[0]) {
                    case '1': selectionType = TOURNAMENT; break;
                    case '2': selectionType = ROULETTE; break;
                    case '3': selectionType = RANKING; break;
                }
                break;
            }

            case '2': {
                showCrossoverMenu();
                printf("\nEnter your choice (1-3): ");
                fflush(stdout);
                if (fgets(input, sizeof(input), stdin) == NULL) continue;
                switch(input[0]) {
                    case '1': crossoverType = SINGLE_POINT; break;
                    case '2': crossoverType = MULTI_POINT; break;
                    case '3': crossoverType = UNIFORM; break;
                }
                break;
            }

            case '3': {
                showMutationMenu();
                printf("\nEnter your choice (1-3): ");
                fflush(stdout);
                if (fgets(input, sizeof(input), stdin) == NULL) continue;
                switch(input[0]) {
                    case '1': mutationType = ROW_SWAP; break;
                    case '2': mutationType = BLOCK_SWAP; break;
                    case '3': mutationType = RANDOM_CHANGE; break;
                }
                break;
            }

            case '4': {
                clearScreen();
                printf("=== STARTING SUDOKU SOLVER ===\n\n");
                printf("Using:\n");
                showCurrentSettings(selectionType, crossoverType, mutationType);
                printf("\nInitializing population...\n");

                Population* population = createPopulation();
                printf("Initial puzzle:\n");
                printBoard(&population->boards[0]);

                printf("\nStarting evolution...\n");
                printf("Press Enter to continue...");
                getchar();

                clock_t start = clock();
                SudokuBoard solution = evolve(population, selectionType,
                                            crossoverType, mutationType);
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

                printf("\n=== FINAL SOLUTION ===\n");
                printBoard(&solution);
                printf("\nExecution time: %.2f seconds\n", time_spent);

                if(solution.fitness == 243) {
                    printf("Perfect solution found!\n");
                } else {
                    printf("No perfect solution found.\n");
                    printf("Current solution quality: %.1f%%\n",
                           (solution.fitness * 100.0) / 243.0);
                }

                destroyPopulation(population);
                printf("\nPress Enter to return to menu...");
                getchar();
                clearScreen();
                break;

            }
        }
    }
}