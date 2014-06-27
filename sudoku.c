#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int board_test[9][9] = {
	{ 5,0,0, 8,7,0, 4,0,0 },
	{ 0,0,1, 0,9,0, 0,0,5 },
	{ 0,2,3, 6,0,5, 0,0,0 },

	{ 0,0,8, 0,2,0, 9,3,0 },
	{ 6,0,7, 1,0,9, 2,0,8 },
	{ 0,0,0, 0,0,0, 0,0,0 },

	{ 0,0,0, 2,0,6, 1,9,0 },
	{ 9,0,0, 0,4,0, 8,0,0 },
	{ 0,0,2, 0,8,1, 0,0,4 }
};

typedef struct stack {
	int len;
	void **st;
} stack_t;

typedef struct posval {
	int i, j, val;
} posval_t;

stack_t* st_create(int maxsize)
{
	stack_t *st = (stack_t *)malloc(sizeof(stack_t));
	st->st = (void **)calloc(maxsize, sizeof(void*));
	st->len = 0;
	return st;
}

void st_destroy(stack_t *st)
{
	free(st->st);
	memset(st, 0, sizeof(stack_t));
	free(st);
}

void st_push(stack_t *st, void *value)
{
	st->st[st->len] = value;
	st->len++;
}

void* st_pop(stack_t *st)
{
	if (st->len > 0)
	{
		void *value = st->st[st->len-1];
		st->len--;
		return value;
	}
	
	return NULL;
}

void* st_peek(stack_t *st)
{
	if (st->len > 0)
	{
		void *value = st->st[st->len-1];
		return value;
	}
	
	return NULL;
}

int st_count(stack_t *st)
{
	return st->len;
}

void st_shuffle(stack_t *st)
{
	srand(time(NULL));
	int i, rndcol;
	for (i = 0; i < st->len; i++) {
		void *tmp = st->st[i];
		rndcol = rand() % st->len; 
		st->st[i] = st->st[rndcol];
		st->st[rndcol] = tmp;
	}
}

int **clone_board(int **b) {
	int i, j;
	int **nb = (int**)malloc(sizeof(int*) * 9);
	for (i = 0; i < 9; i++) {
		nb[i] = (int*)malloc(sizeof(int) * 9);
		if (b != NULL) {
			for (j = 0; j < 9; j++)
				nb[i][j] = b[i][j];
		} else {
			for (j = 0; j < 9; j++)
				nb[i][j] = 0;
		}
	}

	return nb;
}

void free_board(int **b) {
	int i;
	for (i = 0; i < 9; i++)
		free(b[i]);

	free(b);
}

int is_valid(int **b, int i, int j)
{
	int k, k2;
	int value = b[i][j];
	int bi = i / 3, bj = j / 3;
	
	for (k = 0; k < 9; k++)
		if (i != k && b[k][j] == value)
			break;
	
	if (k == 9) {
		for (k = 0; k < 9; k++)
			if (j != k && b[i][k] == value)
				break;
	} else {
		return 0;
	}
	
	if (k == 9) {
		for (k = 0; k < 3; k++) {
			for (k2 = 0; k2 < 3; k2++) {
				if (i != bi * 3 + k && j != bj * 3 + k2 && b[bi * 3 + k][bj * 3 + k2] == value)
					return 0;
			}
		}
	} else {
		return 0;
	}

	return 1;
}

int solve_rec(int **b, int ni, int nj, int *count, stack_t *solutions, int first_n)
{
	int i, j;
	
	for (i = ni; i < 9; i++) {
		for (j = nj; j < 9; j++) {
			nj = 0;
			if (b[i][j] == 0) {
				int k;
				for (k = 1; k <= 9; k++) {
					b[i][j] = k;
					if (is_valid(b, i, j) && solve_rec(b, i, j, count, solutions, first_n)) {
						b[i][j] = 0;
						return 1;
					}
				}
				
				b[i][j] = 0;
				return 0;
			}
		}
	}
	
	(*count)++;
	if (solutions != NULL) {
		st_push(solutions, clone_board(b));
	}

	if (*count == first_n) {
		return 1;
	} else {

		return 0;
	}
}

int solve(int **b, stack_t *solutions)
{
	int count = 0;
	solve_rec(b, 0, 0, &count, solutions, 0);
	return count;
}

int solveone(int **b, stack_t *solutions)
{
	int count = 0;
	solve_rec(b, 0, 0, &count, solutions, 1);
	return count;
}

int solvetwo(int **b, stack_t *solutions)
{
	int count = 0;
	solve_rec(b, 0, 0, &count, solutions, 2);
	return count;
}

void print_board(int **b)
{
	int i, j;
	
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (j % 3 == 2)
				printf("%d, ", b[i][j]);
			else
				printf("%d,", b[i][j]);
		}

		if (i % 3 == 2)
			printf("\n\n");
		else
			printf("\n");
	}
}

void print_board_html(int **b, int **s)
{
	int i, j;
	
	printf("<html><body>\n");
	printf("<table><tr><td>\n");

	printf("<table>\n");
	for (i = 0; i < 9; i++) {
		printf("<tr>\n");
		for (j = 0; j < 9; j++) {
			if (i % 3 == 0 && j % 3 == 0)
				printf("<td style='padding: 5px; border: 1px solid black; border-left: 3px solid black; border-top: 3px solid black'>\n");
			else if (i % 3 == 0)
				printf("<td style='padding: 5px; border: 1px solid black; border-top: 3px solid black'>\n");
			else if (j % 3 == 0)
				printf("<td style='padding: 5px; border: 1px solid black; border-left: 3px solid black'>\n");
			else
				printf("<td style='padding: 5px; border: 1px solid black'>\n");
			if (b[i][j] != 0) {
				printf("<strong style='font-size: x-large'>%d</strong>", b[i][j]);
			} else
				printf("&nbsp;");
			printf("</td>\n");
		}
		printf("</tr>\n");
	}
	printf("</table>\n");
	printf("</td><td style='width: 50px'></td><td>\n");


	printf("<table>\n");
	for (i = 0; i < 9; i++) {
		printf("<tr>\n");
		for (j = 0; j < 9; j++) {
			if (i % 3 == 0 && j % 3 == 0)
				printf("<td style='padding: 5px; border: 1px solid black; border-left: 3px solid black; border-top: 3px solid black'>\n");
			else if (i % 3 == 0)
				printf("<td style='padding: 5px; border: 1px solid black; border-top: 3px solid black'>\n");
			else if (j % 3 == 0)
				printf("<td style='padding: 5px; border: 1px solid black; border-left: 3px solid black'>\n");
			else
				printf("<td style='padding: 5px; border: 1px solid black'>\n");
			if (b[i][j] != 0) {
				printf("<strong style='font-size: x-large'>%d</strong>", b[i][j]);
			} else
				printf("%d", s[i][j]);
			printf("</td>\n");
		}
		printf("</tr>\n");
	}
	printf("</table>\n");

	printf("</td></tr></table>\n");
	printf("</body></html>\n");
}

void shuffle(int *n)
{
	srand(time(NULL));
	int i, rndcol;
	for (i = 0; i < 9; i++) {
		int tmp = n[i];
		rndcol = rand() % 9; 
		n[i] = n[rndcol];
		n[rndcol] = tmp;
	}
}

posval_t* posval_create(int i, int j, int val)
{
	posval_t *p = (posval_t*)malloc(sizeof(posval_t));
	p->i = i;
	p->j = j;
	p->val = val;

	return p;
}

void posval_destroy(posval_t *p)
{
	free(p);
}

void generate(int amount)
{
	int i, j;
	int **board = clone_board(NULL);
	int **solution;
	stack_t *solutions = st_create(1);
	stack_t *pos = st_create(9*9);
	int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int rndrow = rand() % 9; 

	shuffle(numbers);

	for (i = 0; i < 9; i++)
		board[rndrow][i] = numbers[i];

	solveone(board, solutions);
	solution = st_pop(solutions);
	free_board(board);
	st_destroy(solutions);
	board = clone_board(solution);

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			st_push(pos, posval_create(i, j, board[i][j]));

	while (st_count(pos) > amount) {
		st_shuffle(pos);

		posval_t *p = st_pop(pos);
		board[p->i][p->j] = 0;


		if (solvetwo(board, NULL) > 1) {
			board[p->i][p->j] = p->val;
			st_push(pos, p);
		} else {
			posval_destroy(p);
		}
	}

	print_board_html(board, solution);

	free_board(board);
	free_board(solution);

	while (st_count(pos) > 0) {
		posval_destroy(st_pop(pos));
	}

	st_destroy(pos);
}

int main()
{
	srand(time(NULL));

	generate(25);
}

