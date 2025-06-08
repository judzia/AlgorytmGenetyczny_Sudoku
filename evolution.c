#include "evolution.h"
#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"


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
