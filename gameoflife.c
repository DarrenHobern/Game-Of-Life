/*
Darren Hobern 2018
C lang implementation of Conway's Game Of Life using a single dimensional pointer
and a spherical board.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int _WIDTH = 10;
const int _HEIGHT = 10;

// live w/ fewer than 2 live = dead
// live w/ more than 3 live = dead
// live w/ exactly 2 or 3 = live
// dead w/ exactly 3 = live

/*
Python style modulus where negative values will wrap around and be positive.
*/
int mod(int x, int m) {
    return (x%m + m)%m;
}

/*
Converts a row/column position into an index value for the board array.
*/
int convert_position(int row, int col) {
    int value = row * _WIDTH + col;
}

/*
Returns the number of adjacent living cells to the cell in the given position.
*/
int count_adjacent(int *board, int row, int col) {
    int adj = 0;  // Number of adjacent live cells

    // NB: both for loops using <= for end condition.
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            if (r == 0 && c == 0)
                continue;  // ignore self

            int adjust_row = mod((row+r),_HEIGHT);
            int adjust_col = mod((col+c),_WIDTH);
            if (*(board + convert_position(adjust_row, adjust_col))) {
                adj++;
            }
        }
    }
    return adj;
}

/*
Copies the board state from the src to the dest.
*/
int boardcpy(int *dest, int *src) {
    for (size_t i = 0; i < _HEIGHT*_WIDTH; i++) {
        *(dest + i) = *(src + i);
    }
    return 0;
}

/*
Iterate over the board determining the state of the cells in the current board
and applying them to the temp board.
Returns the n
*/
void age_board(int *tmp_board, int *board, int num_adj, int row, int col) {
    //apply rules
    int pos = convert_position(row, col);
    int cell = *(board+pos);
    int state = cell;
    if (cell) {  // alive
        if (num_adj < 2 || num_adj > 3) {  // dies
            state = 0;
        }
        else if (num_adj == 2 || num_adj == 3) {
            state = 1;
        }
    }
    else {  // dead
        if (num_adj == 3) {  // alives
            state = 1;
        }
    }
    *(tmp_board+pos) = state;
}

/*
Perform a game event, updating the board based on the cell states.
The main board is read with updates written to the temp board,
then the temp board is copied to the main board.
*/
void update_board(int *temp, int *board) {
    for (size_t r = 0; r < _HEIGHT; r++) {
        for (size_t c = 0; c < _WIDTH; c++) {
            int adj = count_adjacent(board, r, c);
            age_board(temp, board, adj, r, c);
        }
    }
    boardcpy(board, temp);
}

/*
Prints the board state in ascii.
*/
void print_board(int *board) {
    char cell = ' ';
    int pos = 0;
    for (size_t r = 0; r < _HEIGHT; r++) {
        for (size_t c = 0; c < _WIDTH; c++) {
            pos = convert_position(r,c);
            if (*(board+pos))
                cell = '#';
            else
                cell = ' ';
            printf("%c", cell);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[]) {
    int *tmp_board = (int*)malloc(sizeof(int)*_WIDTH*_HEIGHT);
    int board[] =
            {   0,0,0,1,0,0,0,0,0,0,
                0,0,0,0,1,0,0,0,0,0,
                0,0,1,1,1,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,1,0,0,
                0,0,0,0,0,0,0,0,1,0,
                0,0,0,0,0,0,1,1,1,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0};
    // Infinite loop - keyboard interrupt to quit.
    while(1) {
        print_board(board);
        update_board(tmp_board, board);
        sleep(1);
        printf("-----------------------------\n");
    }
    free(tmp_board);
    free(board);
    return 0;
}
