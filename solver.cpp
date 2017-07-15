/*
*
* Paul Fatum
* paulfa@umich.edu
*
* Sudoku Solver
* 4/26/2016
* Takes in an unsolved sudoku board and prints out the solution using backtracking
* 
*/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

void solve(vector<vector<int>> &board, vector<vector<int>> &solution, int current_x, int current_y, bool done, int &count);
bool promising(vector<vector<int>> &board, int current_x, int current_y);
bool board_complete(vector<vector<int>> &board);
void find_next_open_spot(vector<vector<int>> &board, int &current_x, int &current_y);
void print_board(vector<vector<int>> &board);
void print_menu();

int main() {
    cout << "Welcome to Paul's Sudoku Solver!\nPlease enter the sudoku board: \n" << 
            "(NOTE! Please input 0 for blank squares and put spaces between numbers) \n";
    //creates a 9x9 board
    vector<vector<int>> board;
    board.resize(9);
    for (int i = 0; i < 9; ++i) {
        board[i].resize(9);
    }
    vector<vector<int>> solution;
    solution.resize(9);
    for (int i = 0; i < 9; ++i) {
        solution[i].resize(9);
    }

    int int_in;
    int empty_squares = 0;
    //reads in the new board from input
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cin >> int_in;
            board[i][j] = int_in;
            if (int_in == 0) {
                empty_squares++;
            }
        }
    }
    bool done = false;
    int count = 0;
    //find the solution to the sudoku board
    //starts at (0, 0)
    solve(board, solution, 0, 0, done, count);
    string user_choice;
    int row_choice;
    int col_choice;
    while (true) {
        print_menu();
        cin >> user_choice;
        //sudoku solution
        if (user_choice == "1") {
            cout << "\nSolved board: \n";
            //output solved board
            print_board(solution);
        }
        //sudoku hint
        else if (user_choice == "2") {
            cout << "\nPlease enter \"row # column #\"\n";
            cin >> row_choice >> col_choice;
            if (row_choice < 0 || row_choice >= 9 || col_choice < 0 || col_choice >= 9) {
                cout << "Invalid row/column choice\n";
            }
            else {
                cout << "Try this number: " << solution[row_choice][col_choice] << "\n";
            }
        }
        else if (user_choice == "3") {
            cout << "\nPerformed " << count << " calculations out of a possible 9^" << empty_squares << "\n";
        }
        else if (user_choice == "4") {
            cout << "\nThanks for using Paul's Sudoku Solver!\n";
            break;
        }
        else {
            cout << "Invalid command! Try again.\n";
        }
    }

    return 0;
}

void solve(vector<vector<int>> &board, vector<vector<int>> &solution, int current_x, int current_y, bool done, int &count) {
    //makes sure the board is promising
    if (!promising(board, current_x, current_y)) {
        return;
    }
    //conditions for completed correct board
    if (board_complete(board)) {
        solution = board;
        done = true;
        return;
    }
    //finds next changeable box and recurses
    find_next_open_spot(board, current_x, current_y);
    //recurses through all possible permutations of each available spot
    for (int i = 1; i <= 9; ++i) {
        //sets empty spot to each possible number 1-9
        board[current_x][current_y] = i;
        count++;
        solve(board, solution, current_x, current_y, done, count);
        //if that solve call came up with the correct board
        if (done) {
            return;
        }
        //otherwise reset for a new recursion
        if (!promising(board, current_x, current_y)) {
            board[current_x][current_y] = 0;
        }
    }
    //non-promising
    board[current_x][current_y] = 0;
    return;
}

bool promising(vector<vector<int>> &board, int current_x, int current_y) {
    vector<bool> row_check(10, false);
    vector<bool> col_check(10, false);
    vector<bool> block_check(10, false);
    int current;
    for (int i = 0; i < 9; ++i) {
        //checks for duplicates in the row
        current = board[current_x][i];
        if (!row_check[current]) {
            row_check[current] = true;
        }
        else if (current != 0) {
            return false;
        }
    }
    for (int j = 0; j < 9; ++j) {
        //checks for duplicates in the column
        current = board[j][current_y];
        if (!col_check[current]) {
            col_check[current] = true;
        }
        else if (current != 0) {
            return false;
        }
    }
    int block_row = current_x - (current_x % 3);
    int block_col = current_y - (current_y % 3);
    //iterates through block
    for (int i = block_row; i < block_row + 3; ++i) {
        for (int j = block_col; j < block_col + 3; ++j) {
            current = board[i][j];
            //checks for duplicates that aren't the actual entry
            if (!block_check[current]) {
                block_check[current] = true;
            }
            else if (current != 0) {
                return false;
            }
        }
    }
    //all constraints passed
    return true;
}

bool board_complete(vector<vector<int>> &board) {
    //checks to see if each box is full
    //can probably be optimized
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }
    //board is solved if no X's are found
    return true;
}

void find_next_open_spot(vector<vector<int>> &board, int &x, int &y) {
    //starts at appropriate row to save time
    for (int i = x; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == 0) {
                x = i;
                y = j;
                return;
            }
        }
    }
    cout << "No open spot found! No solution possible!\n";
    return;
}

void print_board(vector<vector<int>> &board) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cout << board[i][j] << " ";
            //prints funky little grid
            if (j == 2 || j == 5) {
                cout << "|";
            }
        }
        if (i == 2 || i == 5) {
            cout << "\n- - - - - - - - - -\n";
        }
        else {
            cout << "\n";
        }
    }
}

void print_menu() {
    cout << "\nWhat would you like to do?\n";
    cout << "1) Board solution\n2) Hint for square\n3) Backtracking statistics\n4) Quit\n";
}