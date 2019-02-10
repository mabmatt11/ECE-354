////////////////////////////////////////////////////////////////////////////////
// Main File:        intdate.c
// This File:        intdate.c
// Other Files:      sendsig.c
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
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

///// time to wait between alarms (s) /////
int waitTime = 3;

//// Counter for user signal //////
int userSigCount = 0;

/* handler for the alarm signal
 *
 * When alarm signal is received it will print the PID
 * and the current time.
 */
void handle_sigalrm() {
    //// initialize the currtent time variable ////
    time_t curtime;

    ///// Get the current time, check it worked /////
    if (time(&curtime) == -1) {
        printf("Error getting current time.");
        exit(0);
    }

    //// Formate the time, check it worked /////
    if (ctime(&curtime) == NULL) {
        printf("Error converting time to format.");
        exit(0);
    }

    //// Print out data to user //////
    printf("PID: %d | Current Time: %s", getpid(), ctime(&curtime));

    //// set next alarm /////
    alarm(waitTime);
    return;
} 

/* handler for the user1 signal
 *
 * Will exclaim that the signal was caught, increment sig counter
 */
void handle_sigusr1() {
    printf("SIGUSR1 caught!\n");
    userSigCount += 1;
    return;
}

/* handler for the keyboard command+c signal
 *
 * Will note the signal was received and exit telling
 * us the User Signal count
 */
void handle_sigint(int sig) {
    printf("\nSIGINT received.\n");
    printf("SIGUSR1 was received %d time. Exiting now.\n", userSigCount);
    exit(0);
    return;
}

/* main/heart of the program!
 *
 * Will print the PID and the current time every 3 seconds.
 * Will catch user signals as well.
 */  
int main() 
{
    ////// Opening statement to user ////////
    printf("Pid and time will be printed every 3 seconds.\n");
    printf(" Enter ^C to end the program.\n");

    ////// Set up signal action for alarm //////
    struct sigaction alrm;
    memset(&alrm, 0, sizeof(alrm));  // clear/initialize the mem
    alrm.sa_handler = handle_sigalrm;  // assign handler
    alrm.sa_flags = 0;

    ///// Finish set up for handling SIGALRM, check it worked ///
    if (sigaction(SIGALRM, &alrm, NULL) != 0) { 
        printf("ERROR: Binding SIGALRM Handler");
        exit(0);
    }

    ///// Set up signal action for User signal //////
    struct sigaction usr1;
    memset(&usr1, 0, sizeof(usr1));  // clear/initialize the mem
    usr1.sa_handler = handle_sigusr1;  // assign handler
    usr1.sa_flags = 0;

    ///// Finish set up for handling SIGUSR1, check it worked ///
    if (sigaction(SIGUSR1, &usr1, NULL) != 0) {
        printf("ERROR: Binding SIGUSR1 Handler");
        exit(0);
    }

    ///// Set up signal action for sigint ////
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));  // clear/initialize the mem
    sa.sa_handler = handle_sigint;  // assign handler
    sa.sa_flags = 0;

    ///// Finish set up for handling SIGINT, check it worked ////
    if (sigaction(SIGINT, &sa, NULL) != 0) {
            printf("ERROR: Binding SIGUSR1 Handler");
            exit(0);
    }

    
    //// start alarm timer once /////////
    alarm(waitTime);

    ///// Enter infinite loop ///////
    while (1) {
    }

    return 0; 
} 

