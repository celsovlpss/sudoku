
all: sudoku

sudoku: sudoku.c
	gcc sudoku.c -std=c99 -o sudoku
