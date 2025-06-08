#include "population.h"
#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"


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
