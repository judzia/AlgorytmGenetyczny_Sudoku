# Genetic Algorithm Sudoku Solver

This project implements a **Genetic Algorithm** to solve 9x9 Sudoku puzzles. It uses various genetic operators such as selection, crossover, and mutation, and supports multiple strategies for each of them.

## 📌 Features

- Solves Sudoku puzzles using evolutionary techniques
- Multiple selection methods:
  - Tournament
  - Roulette wheel
  - Ranking
- Multiple crossover methods:
  - Single-point (3x3 block exchange)
  - Multi-point
  - Uniform
- Multiple mutation types:
  - Row swap
  - Block swap
  - Random number change
- Elitism support (preserves top individuals)
- Configurable parameters for easy tuning

## ⚙️ Configuration

All parameters are defined in `sudoku.h`:

```
#define BOARD_SIZE 9
#define POPULATION_SIZE 200
#define MAX_GENERATIONS 1000
#define CROSSOVER_RATE 0.9
#define MUTATION_PROBABILITY 0.05
#define MUTATION_RATE 0.2
#define TOURNAMENT_SIZE 10
#define ELITE_COUNT 8
```

## 🧬 Structures
`SudokuBoard`
Represents a Sudoku board, including:

board – current values (9x9)

initial – fixed starting values

fitness – quality of the solution

`Population`
Dynamic array of SudokuBoard

size – number of individuals

You can modify the initial Sudoku puzzle by editing the hardcoded puzzle in main.cpp or using setInitialPuzzle().


## 🔍 Main Functions

### Initialization
- initializeBoard(SudokuBoard*)

- createPopulation()

- setInitialPuzzle(board, puzzle)

### Fitness
- calculateFitness(): evaluates number of conflicts

- isValidSolution(): checks for complete, valid solution

### Selection
- selectParent(): supports 3 strategies:

 TOURNAMENT, ROULETTE, RANKING

### Crossover
- crossover(parent1, parent2, child, type): uses one of:

 SINGLE_POINT, MULTI_POINT, UNIFORM

### Mutation
- mutate(board, type): uses:

 ROW_SWAP, BLOCK_SWAP, RANDOM_CHANGE

### Evolution
- evolve(population, selType, crossType, mutType): core evolutionary loop

## 🧠 Fitness Function
Fitness is calculated based on the number of valid rows, columns, and blocks (with fewer duplicates = better fitness).

## Random Screenshot 
![image](https://github.com/user-attachments/assets/8547644e-b521-4686-b315-6374f690d085)
