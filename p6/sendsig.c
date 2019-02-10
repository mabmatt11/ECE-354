////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c
// This File:        sendsig.c
// Other Files:      intdate.c
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

/* Send signal to specific PID
 *
 * argc -- The number of parameters entered
 * argv -- The parameters entered
 *
 * Will send user signal or command+c signal depending
 * on the parameters input
 */
int main(int argc, char **argv) {
    ///////// Make sure input count is correct /////////
    if (argc != 3) {
        printf("Usage: <signal type> <pid>\n");
        exit(0);
    }

    //////// Make sure input 2 in valid //////////
    if (strcmp(argv[1], "-u") != 0 && strcmp(argv[1], "-i") != 0) {
        printf("Usage: <signal type> <pid>\n<signal type> = -u or -i\n");
        exit(0);
    }

    //// convert input 3 to integer /////////
    int pid = atoi(argv[2]);

    /////// Make sure input 3 was valid ////////
    if (pid == 0) {
        printf("Usage: <signal type> <pid>\n");
        exit(0);
    }

    ////// Send signals based off of inputs ////////
    if (strcmp(argv[1], "-u") == 0) {
        if (kill(pid, SIGUSR1) != 0) {
            printf("Error sending signal");
        }
    } else if (strcmp(argv[1], "-i") == 0) {
        if (kill(pid, SIGINT) != 0) {
            printf("Error sending signal");
        }
    }

    return(0);
}
        
