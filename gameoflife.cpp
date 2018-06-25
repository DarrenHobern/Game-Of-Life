/**
Conways game of life using C++
Darren Hobern
2017

RULES:
0. Any live cell with fewer than 2 live neighbours, dies
1. Any live cell with 2 or 3 live neighbours, lives
2. Any live cell with more than 3 live neighbours, dies
3. Any dead cell with exactly 3 live neighbours, lives
*/

#include <iostream>
#include <unistd.h>  // sleep function

using namespace std;
const int B_SIZE = 10;

/*
Counts the number of live cells in the 8 adjacent cells.
The board is spherically wrapped, so the left is adjacent to the right,
and the top with the bottom.
*/
int count_adjacent(int** board, int x, int y) {
    int adjacent = 0;
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            if (r == 0 && c == 0) {  // ignore self
                continue;
            }
            // spherically wrap the board
            int col = (B_SIZE + ((x+c) % B_SIZE)) % B_SIZE;
            int row = (B_SIZE + ((y+r) % B_SIZE)) % B_SIZE;

            if (board[row][col] == 1) {
                adjacent++;
            }
        }
    }
    return adjacent;
}

/*
Clones the temp board to the actual board, indicating the start of a new step.
*/
void copy_board(int **board, int **temp) {
    for (int r = 0; r < B_SIZE; r++) {
        for (int c = 0; c < B_SIZE; c++) {
            board[r][c] = temp[r][c];
            temp[r][c] = 0;
        }
    }
}


/*
Converts cells to dead or alive based on the rules of the game.
*/
void apply_rules(int cell_state, int **tempBoard, int adjacent, int r, int c) {
    if (cell_state) {
        if (adjacent < 2) {  // ALIVE and less than 2 adjacent DIES
            tempBoard[r][c] = 0;
        }
        else if (adjacent == 2 || adjacent == 3) {  // ALIVE and 2 or 3 adjacent LIVES
            tempBoard[r][c] = 1;
        }
        else if (adjacent > 3){  // ALIVE and more than 3 adjacent DIES
            tempBoard[r][c] = 0;
        }
    }
    else {
        if (adjacent == 3) {  // DEAD and 3 adjacent LIVES
            tempBoard[r][c] = 1;
        }
    }
}

/*
Applies the game rules to each cell on the board.
Rules are applied to a temp board so that updates don't affect the board
as its updating.
*/
void update_board(int **board, int **temp) {
    int adjacent = 0;
    for (int r = 0; r < B_SIZE; r++) {
        for (int c = 0; c < B_SIZE; c++) {
            adjacent = count_adjacent(board, c, r);
            apply_rules(board[r][c], temp, adjacent, r,c);
        }
    }

    copy_board(board, temp);
}

/*
Prints the board state converting cell states to '#' or '-' for dead/alive
*/
void print_board(int **board) {
    for (int r = 0; r < B_SIZE; r++) {
        for (int c = 0; c < B_SIZE; c++) {
            if (board[r][c]) {
                cout << "# ";
            }
            else {
                cout << "- ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main () {

    // Allocate board memory
    int **board = new int*[B_SIZE];
    int **tempBoard = new int*[B_SIZE];
    for (int r = 0; r < B_SIZE; r++) {
        board[r] = new int[B_SIZE];
        tempBoard[r] = new int[B_SIZE];
    }

    // Glider
    board[0][1] = 1;
    board[2][0] = 1;
    board[2][1] = 1;
    board[2][2] = 1;
    board[1][2] = 1;

    while (1) {
        print_board(board);
        update_board(board, tempBoard);
        sleep(1);
    }
    delete board;
    delete tempBoard;
   return 0;
}
