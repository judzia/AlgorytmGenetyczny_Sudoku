#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sudoku.h"

int main(void)
{
    // Inicjalizacja generatora liczb losowych
    srand(time(NULL));

    // Utworzenie populacji
    Population* population = createPopulation();

    // TODO: Implementacja głównej pętli algorytmu genetycznego

    // Zwolnienie pamięci
    destroyPopulation(population);

    return 0;
}