
  #include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *DELIM = ",";  // commas ',' are a common delimiter character for data strings
     
/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, DELIM);
    *size = atoi(token);
}

/* 
 * Returns 0 if there is no row winner
 * Returns 1 if X won the row
 * Returns 2 if O won the row
 * 
 * row: row we are checking for win
 * size: number of rows and columns
*/
int check_row_win(int *row, int size) {
    int xCount = 0;
    int oCount = 0;

    for (int i = 0; i < size; i++) {
        if (*(row+i) == 1) {         // If X
            xCount++;
        } else if (*(row+i) == 2) {  // If O
            oCount++;
        }
 
        // Check if xCount or oCount == size which would indicate winning line in row
        if (xCount == size) {
            return 1;                 // Return 1 if X won this row
        } else if (oCount == size) {
            return 2;                 // Return 2 if O won this row
        }
    }

    // If no winner in this row return 0
    return 0;
}

/* 
 * Returns 0 if there is no column winner
 * Returns 1 if X won the column
 * Returns 2 if O won the column
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 * col: column to check
*/
int check_column_win(int **board, int size, int col) {
    int xCount = 0;
    int oCount = 0;

    for (int i = 0; i < size; i++) {
        if (*(*(board+i)+col) == 1) {         // If X
            xCount++;
        } else if (*(*(board+i)+col) == 2) { // If O
            oCount++;
        }

        // Check if xCount or oCount == size which would indicate winning line in column
        if (xCount == size) {
            return 1;                 // Return 1 if X won this col
        } else if (oCount == size) {
            return 2;                 // Return 2 if O won this col
        }
    }

    // If no winner in this col return 0
    return 0;
}

/* 
 * Returns 0 if there is no diagonal winner
 * Returns 1 if X won the diagonal
 * Returns 2 if O won the diagonal
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
*/
int check_diagonal_win(int **board, int size) {
    // Diagonal win can obviously only happen when the whole diagonal section of board is one player
    // so only have to use one for loop and check board[x][x]
    // since only one loop we reset counter variables outside of loop
    int xCount = 0;
    int oCount = 0;  

    for (int i = 0; i < size; i++) {
        if (*(*(board+i)+i) == 1) {         // If X
            xCount++;
        } else if (*(*(board+i)+i) == 2) { // If O
            oCount++;
        }

        // Check if xCount or oCount == size which would indicate winning line in diagonal
        if (xCount == size) {
            return 1;                 // Return 1 if X won this diagonal
        } else if (oCount == size) {
            return 2;                 // Return 2 if O won this diagonal
        }
    }

    // If no winner in this diagonal return 0
    return 0;
}
 
/* 
 * Check 1: Is board size odd
 *
 * Returns 1 if the size of the board is even
 * 
 * size: number of rows and columns
*/
int is_even(int size) {
    return !(size % 2);
}

/* 
 * Check 2: Same number Xs as Os, or 1 more X than O
 *
 * Returns 1 if the number of turns is invalid
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
*/
int count_turns(int **board, int size) {
    int xCount = 0;
    int oCount = 0;
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (*(*(board+i)+j) == 1) {         // If X
                xCount++;
            } else if (*(*(board+i)+j) == 2) {  // If O
                oCount++;
            }
        }
    }
    return !(xCount == oCount || (xCount - oCount) == 1);
}

/* 
 * Check 3: Either no winner or one winner
 * Check 4: Either one winning line, or two winning lines that intersect on one mark; 
 *          two parallel winning lines are invalid
 *
 * Returns 1 if the number of turns is invalid
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
*/
int check_winner(int **board, int size) {
    int xWinCount = 0;
    int oWinCount = 0;

    // Check rows  
    int xRowWinCount = 0;
    int oRowWinCount = 0;
    for (int i = 0; i < size; i++) {
        int rowWinner;

        rowWinner = check_row_win(*(board+i), size);
        if (rowWinner == 1) {
            xWinCount++;
            xRowWinCount++;
        } else if (rowWinner == 2) {
            oWinCount++;
            oRowWinCount++;
        }
    }
    // Check for parallel row wins
    if (xRowWinCount > 1 || oRowWinCount > 1) {
        return 1;
    }


    // Check columns
    int xColWinCount = 0;
    int oColWinCount = 0;
    for (int i = 0; i < size; i++) {
        int colWinner;

        colWinner = check_column_win(board, size, i);
        if (colWinner == 1) {
            xWinCount++;
            xColWinCount++;
        } else if (colWinner == 2) {
            oWinCount++;
            oColWinCount++;
        }
    }
    // Check for parallel col wins
    if (xColWinCount > 1 || oColWinCount > 1) {
        return 1;
    }

    // Check diag
    int diagonalWinner = check_diagonal_win(board, size);
    if (diagonalWinner == 1) {
        xWinCount++;
    } else if (diagonalWinner == 2) {
        oWinCount++;
    }
    
    // printf("x win count: %i\n", xWinCount);
    // printf("o win count: %i\n", oWinCount);

    if (xWinCount == 0 && oWinCount == 0) {
        return 0;
    } else if (xWinCount == 1 && oWinCount == 0) {
        return 0;
    } else if (xWinCount == 0 && oWinCount == 1) {
        return 0;
    } else if (xWinCount > 1 && oWinCount == 0) {
        return 0;
    } else if (xWinCount == 0 && oWinCount > 1) {
        return 0;
    }

    return 1;

}
  
/*
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size) {

    // Check 1: Is board size odd
    if (is_even(size)) {
        return 0;
    }

    // Check 2: Same number Xs as Os, or 1 more X than O
    if (count_turns(board, size)) {
        return 0;
    }

    // Check 3: Either no winner or one winner
    // Check 4: Either one winning line, or two winning lines that intersect on one mark; 
    //          two parallel winning lines are invalid
    if (check_winner(board, size)) {
        return 0;
    }

    return 1;   
}    
 
/*
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              
     
    //Check if number of command-line arguments is correct.
    if (argc != 2 ) {
        printf("Incorrect arguments. Only the name of the .txt file containing the board is needed.\n");
        printf("Example: './n_in_a_row board1.txt'\n");
        exit(1);
    }

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
    int size;

    // Call get_dimensions()
    get_dimensions(fp, &size);

    //Dynamically allocate a 2D array of dimensions retrieved above.
    int** board;
    board = malloc(sizeof(int*) * size);
    if (board == NULL) {
        printf("Can't allocate memory for board.\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        *(board + i) = malloc(sizeof(int) * size);
        if (*(board + i) == NULL) {
            printf("Can't allocate memory for board[%i].\n", i);
            exit(1);
        }
    }

    //Read the file line by line.
    //Tokenize each line wrt the delimiter character to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, DELIM);

        // iterate token and put values in dimensions[][]
        for (int j = 0; j < size; j++) {            
            *(*(board+i)+j) = atoi(token);
            token = strtok(NULL, DELIM);
        }
    }

    // Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    if(n_in_a_row(board, size)) {
        printf("valid\n");
    } else {
        printf("invalid\n");
    }

    //Free all dynamically allocated memory.
    for (int i = 0; i < size; i++) {
        free(*(board + i));
        *(board + i) = NULL;
    }
    free(board);
    board = NULL;

    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}