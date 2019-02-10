////////////////////////////////////////////////////////////////////////////////
// Main File:        check_queens.c
// This File:        check_queens.c
// Other Files:      check1.txt, check2.txt, check3.txt, etc..
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
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h> 
#include <stdlib.h>  
#include <string.h>   

char *COMMA = ",";

/*
 * Retrieves from the first line of the input file,
 * the number of rows and columns for the board.
 * 
 * fp: file pointer for input file
 * rows: pointer to number of rows
 * cols: pointer to number of columns
 */
void get_dimensions(FILE *fp, int *rows, int *cols) {     
	char *line = NULL; 
	size_t len = 0;
	if (getline(&line, &len, fp) == -1) { 
		printf("Error in reading the file\n");
		exit(1); 
	}

	char *token = NULL; 
	token = strtok(line, COMMA);
	*rows = atoi(token);

	token = strtok(NULL, COMMA);
	*cols = atoi(token);

}      


/* 
 * Returns 1 if and only if there exists at least one pair
 * of queens that can attack each other.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * rows: number of rows
 * cols: number of columns
 */
int check_queens(int **board, int rows, int cols) {           

	//Iterate through the board row by row
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){

			//When a queen is found
			if (*(*(board+i)+j) == 1){

				//Check same row
				for (int k=i+1; k<rows; k++){
					if ((*(*(board+k)+j)) == 1){
						return 1;
					}
				}

				//Check same column
				for (int l=j+1; l<cols; l++){
					if ((*(*(board+i)+l)) == 1){
						return 1;
					}
				}

				//Check diagonals
				//Only need to check two diagonals because iterating
				//through the board will catch the diagonals above.
				for (int m = 1; m+i<rows && m+j<cols; m++){
					if ((*(*(board+m+i)+m+j)) == 1){
						return 1;
					}
				}
				for (int n = 1; i-n>=0 && n+j<cols; n++){
					if ((*(*(board+i-n)+j+n)) == 1){
						return 1;
					}
				}
						
			}
		}
	}
	return 0;
}     


/* 
 * This program prints true if the input file has any pair
 * of queens that can attack each other, and false otherwise
 * 
 * argc: CLA count
 * argv: CLA value 
 */
int main(int argc, char *argv[]) {                

        //Check if number of command-line arguments is correct.
	if (argc != 2) {
		printf("Usage: ./check_queens <input_filename>\n");
		exit(1);
	}

	//Open the file and check if it opened successfully.
	FILE *fp = fopen(*(argv + 1), "r");
	if (fp == NULL) {
		printf("Cannot open file for reading\n");
		exit(1);
	}


        //Declare local variables.
        int rows, cols;


        //Call get_dimensions to retrieve the board dimensions.
	get_dimensions(fp, &rows, &cols);


        //Dynamically allocate a 2D array of dimensions retrieved above.
	int ** board;
	board = malloc(sizeof(int*)*rows);

	//check if memory successfully allocated
	if (board == NULL){
		printf("Could not allocate memory\n");
		exit(1);
	}
	
	for (int i=0; i<rows; i++){
		*(board+i) = malloc(sizeof(int)*cols);
		
		//check if memory successfully allocated
		if (*(board+i)	== NULL){ 
			printf("Could not allocate memory\n");
			exit(1);
		}

	}

	//Read the file line by line.
	//Tokenize each line wrt comma to store the values in your 2D array.
	char *line = NULL;
	size_t len = 0;
	char *token = NULL;
	for (int i = 0; i < rows; i++) {

		if (getline(&line, &len, fp) == -1) {
			printf("Error while reading the file\n");
			exit(1);	
		}

		token = strtok(line, COMMA);
		
		//check strtok output
		if (token == NULL){
			printf("Error reading file line, check formatting.\n");
			exit(1);
		}

		for (int j = 0; j < cols; j++) {
			// initialize your 2D array.
			*(*(board+i)+j) = atoi(token);

			token = strtok(NULL, COMMA);

			//check strtok output
			if (token == NULL && j != cols-1){
				printf("Error reading file line, check formatting\n");
				exit(1);
			}	
		}
	}

	//Call the function check_queens and print the appropriate
        //output depending on the function's return value.
	int result; //Variable used to store return result value
	result = check_queens(board, rows, cols);
	if (result == 1){
		printf("true\n");
	}
	else{
		printf("false\n");
	}
	
        
	//Free all dynamically allocated memory.
	for (int i=0; i<rows; i++){
		free(*(board+i));
	}
	free(board);
	board = NULL;

	//Close the file.
	if (fclose(fp) != 0) {
		printf("Error while closing the file\n");
		exit(1);	
	}

	return 0;
}    
