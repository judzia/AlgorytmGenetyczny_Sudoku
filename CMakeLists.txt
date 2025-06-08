cmake_minimum_required(VERSION 3.31)
project(AlgorytmGenetycznySudoku C)

set(CMAKE_C_STANDARD 11)

add_executable(AlgorytmGenetycznySudoku
        main.c
        sudoku.c
        sudoku.h
        board_operations.c
        board_operations.h
        population.c
        population.h
        selection.c
        selection.h
        crossover.c
        crossover.h
        mutation.c
        mutation.h
        evolution.c
        evolution.h
)