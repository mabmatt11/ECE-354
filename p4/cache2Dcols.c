////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dcols.c
// This File:        cahce2Dcols.c
// Other Files:      dcache.so
// Semester:         CS 354 Fall 2018
//
// Author:           Matt Bachmeier
// Email:            mbachmeier@wisc.edu
// CS Login:         bachmeier
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          N/A
//                   
//
// Online sources:   N/A
//                    
//                   
//////////////////////////// 80 columns wide ///////////////////////////////////

#define ROWS 3000  // Global rows size
#define COLS 500  // global columns size 
int arr2D[ROWS][COLS];  // Global Array to be set

/* Will set values of global array. Inner loop traverses through elements of a
 * column, outer loop interates through the columns.
 *
 * No Parameters
 * No meaningful return
 */
int main() {
    // Inner loop goes through elements of column, outer loop through columns
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            arr2D[row][col] = row + col;
        }
    }
    return 1;
}

