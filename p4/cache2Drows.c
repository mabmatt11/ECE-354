////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Drows.c
// This File:        cache2Drows.c
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

#define ROWS 3000  // Global var for number of rows
#define COLS 500  // Global var for number of Columns
int arr2D[ROWS][COLS];  // Global array to be set in main

/* Will set values of global array. Inner loop traverses through elements of a 
 * row, outer loop interates through the rows.
 *
 * No Parameters
 * No meaningful return
 */
int main() {
    // Inner loop works through elements of row, outer loop interates rows
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            arr2D[row][col] = row + col; 
	}
    }
    return 1;
}
