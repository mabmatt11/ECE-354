////////////////////////////////////////////////////////////////////////////////
// Main File:        cache1D.c
// This File:        cache1D.c
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

#define SIZE 100000  //size of our array to create
int arr[SIZE]; //Declare global array

/* Assigns values to global array
 *
 * No Parameters
 * No meaningful return value
 */
int main() {
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }
    return 1;
}
