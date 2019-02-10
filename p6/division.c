////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        divisoin.c
// Other Files:      N/A
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// Total number of operations
int totalOp = 0;

/* Handler for SIGFPE (divide by 0)
 *
 * Prints the error message for division by 0,
 * also show the total number of opperations that were completed and exit
 */
void handle_sigfpe() {
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", totalOp);
    printf("The program will be termiinated.\n");
    exit(0);
    return; 
}

/* Handler for SIGINT (command+c)
 *
 * Print the number of successful operations, exit
 */
void handle_sigint() {
    printf("\nTotal number of operations successfully completed: %d\n",
                     totalOp);
    printf("The program will be terminated.\n");
    exit(0);
    return;
}

/* main/heart of the program
 *
 * This will set up handlers for signals, enter a loop that
 * will ask user for inputs then divide them and give result
 */
int main() {
    //////// Set up signal action for divide by 0 //////////
    struct sigaction fpe;
    memset(&fpe, 0, sizeof(fpe));  // clear/initialize the mem
    fpe.sa_handler = handle_sigfpe;  // set the handler for fpe
    fpe.sa_flags = 0;

    /////// Finish set up of fpe signal handler, check it worked ///
    if (sigaction(SIGFPE, &fpe, NULL) != 0) {
        printf("ERROR: Binding SIGFPE Handler");
        exit(0);
    }

    ////// Set up signal action for keyboard interrupt Control+c //////
    struct sigaction sigint;
    memset(&sigint, 0, sizeof(sigint));  // clear/initialize the mem
    sigint.sa_handler = handle_sigint;  // set the handler for sigint
    sigint.sa_flags = 0;

    ////// Finish set up of the sigint handler, check it worked /////
    if (sigaction(SIGINT, &sigint, NULL) != 0) {
        printf("ERROR: Binding SIGINT Handler");
        exit(0);
    }

    //// buffer for fgets //////
    int inputSize = 100;

    ///// input from user ////
    char input[2][inputSize];

    ///// input from user as int /////
    int inputVal[2];

    ///// Two values divided ////
    int divVal;

    ///// Two values remainer //////
    int remVal;

    ////// begin the infinite loop to divide nubmers input ///// 
    while (1) {
        ////// Get the first input, check it worked //////    
        printf("Enter first integer: ");
        if (fgets(input[0], inputSize, stdin) == NULL){
            printf("Error: input Null");
            exit(0);
        }
        
        ///// Get the second input, check it worked //////
        printf("Enter second integer: ");
        if (fgets(input[1], inputSize, stdin) == NULL){
            printf("Error: input Null");
            exit(0);
        }

        ///// ignore atoi errors, get inputs as ints /////
        inputVal[0] = atoi(input[0]);
        inputVal[1] = atoi(input[1]);

        ///// get the math values ///////
        divVal = inputVal[0] / inputVal[1];
        remVal = inputVal[0] % inputVal[1];

        ////// print answer response //////
        printf("%d / %d is %d with a remainder of %d\n",
                      inputVal[0], inputVal[1], divVal, remVal);

        ////// increment total amount of operations /////
        totalOp += 1;
    }
}
