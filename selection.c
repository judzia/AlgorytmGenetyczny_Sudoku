#include "selection.h"
#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

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

// Tournament selection - selects the best from a random group
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
