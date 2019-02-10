////////////////////////////////////////////////////////////////////////////////
// Main File:        traverse_spiral.c
// This File:        traverse_sprial.c
// Other Files:      traverse1.txt, traverse2.txt, ect ..
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

// Structure representing Matrix
// size: Dimension of the square (size*size)
// array: 2D array of integers
typedef struct _Square {  
    int size;
    int **array;
} Square;

/* 
 * Retrieves from the first line of the input file,
 * the size of the square matrix.
 * 
 * fp: file pointer for input file
 * size: pointer to size of the square matrix
 */
void get_dimensions(FILE *fp, int *size) {        
	char *line = NULL;
       	size_t len = 0;
	
	//get first line. exit if it fails
       	if (getline(&line, &len, fp) == -1) {
               	printf("Error in reading the file\n");
               	exit(1);
     	}

	//change from string to int
	*size = atoi(line);

	//check atoi output, make sure size is acceptable
	if (*size == 0){
		printf("Error in square size.\n");
		exit(1);
	}
}

/* 
 * Traverses a given layer from the square matrix
 * 
 * array: Heap allocated 2D square matrix
 * size: size of square
 * layer: layer number to be traversed
 * op: pointer to the output file
 */
void traverse_layer(int **array, int size, int layer, FILE *op) {       
	int row_num, col_num;
	int i,j; //used to traverse for loops
	int error; //used to tracks erros in printing to file
	
	//corner case: size is odd & the layer is last so only one entry to print
        if(size % 2 == 1 && layer == (size + 1) / 2 - 1){
                fprintf(op, "%d ", *(*(array + layer) + layer));
                return;
        }
	

	//Traverse upper row from left to right with appropriate bounds
	row_num = layer;
	for(i = row_num; i < size-layer; i++) {
		error = fprintf(op, "%d ", *(*(array + row_num) + i));
		//if printing to file fails exit program
		if (error < 0){
			printf("File could not be written to.\n");
			exit(1);
		}
	}

	//Traverse right column from top to bottom with appropriate bounds
	col_num = size-layer-1;
	for(j = layer + 1; j < size-layer; j++) {
		error = fprintf(op, "%d ", *(*(array + j) + col_num));
		//if printing to file fails exit program
		if (error < 0){
                        printf("File could not be written to.\n");
			exit(1);
                }
	}

	//Traverse lower row from right to left with appropriate bounds
	row_num = size-layer-1;
	for(i = row_num - 1; i >= layer; i--) {
		error = fprintf(op, "%d ", *(*(array + row_num) + i));
		//if printing to file fails exit program
		if (error < 0){
                        printf("File could not be written to.\n");
			exit(1);
                }
	}

	//Traverse left column from bottom to top with appropriate bounds
	col_num = layer;
	for(j = size - layer - 2; j > layer; j--) {
		error = fprintf(op, "%d ", *(*(array + j) + col_num));
		//if printing to file fails exit program
		if (error < 0){
                        printf("File could not be written to.\n");
			exit(1);
                }
	}
	
}


/* COMPLETED:
 * Traverses the square matrix spirally
 * 
 * square: pointer to a structure that contains 2D square matrix
 * op: pointer to the output file
 */
void traverse_spirally(Square *square, FILE *op) {       
	int size = square->size; 
	int num_layers = 0;   
	num_layers = size/2; 
	if(size%2 == 1) {
		num_layers++;
	}
	 
	int i;
	for(i = 0; i < num_layers; i++) {
		traverse_layer(square->array, size, i, op);
	}
}

/* 
 * This program reads a square matrix from the input file
 * and outputs its spiral order traversal to the output file
 *
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {                  
			
	//Check if number of arguments is correct
	if (argc != 3) {
                printf("Usage: ./check_queens <input_filename> <output_filename>\n");
                exit(1);
        }
		
	//Open the file and check if it opened successfully
        FILE *fp = fopen(*(argv + 1), "r");
        if (fp == NULL) {
                printf("Cannot open file for reading.\n");
		fclose(fp);
                exit(1);
        }	
	
	//Call the function get_dimensions to retrieve size of the square matrix
	int size;
	get_dimensions(fp, &size);	
	
	//Dynamically allocate a 2D array as per the retrieved dimensions
	int ** data;
	data = malloc(sizeof(int)*size);
	if (data == NULL){
		//if heap memory fails allocation exit program
		printf("Could not allocate memory\n");
		exit(1);
	}
	for (int i = 0; i < size; i++){
	       *(data+i) = malloc(sizeof(int) * size);
	       //if heap memory fails allocation exit program
	       if (*(data+i) == NULL){
		       printf("Could not allocate memory\n");
		       exit(1);
	       }
	}	       


	//Read the file line by line by using the function getline as used in get_dimensions
	//Tokenize each line wrt comma to store the values in the square matrix
	char *line = NULL;
        size_t len = 0;
        char *token = NULL;
        for (int i = 0; i < size; i++) {

                if (getline(&line, &len, fp) == -1) {
                        printf("Error while reading the file\n");
                        exit(1);
                }

                token = strtok(line, COMMA);

		if (token == NULL){
			printf("No characters remaining to format, error in size\n");
			exit(1);
		}

                for (int j = 0; j < size; j++) {
                        // initialize the 2D array.
                        *(*(data+i)+j) = atoi(token);

                        token = strtok(NULL, COMMA); //Used to get next value

			if (token == NULL && j != size - 1){
				printf("Error in size or formatting\n");
				exit(1);
			}

                }
        }
	


	//Create a structure and initialize its size and array fields appropriately
	Square square;
	square.size = size;
	square.array = data;

	//Open the output file
	FILE *outFile = fopen(*(argv + 2), "w");
        if (outFile == NULL) {
                printf("Cannot open file for writing\n");
                exit(1);
        }


	//Call the function traverse_spirally
	traverse_spirally(&square, outFile);

	//Free the dynamically allocated memory
	for (int i=0; i<size; i++){
                free(*(data+i));
        }
        free(data);
        data = NULL;

	//Close the input file
	if (fclose(fp) != 0) {
                printf("Error while closing the file\n");
                exit(1);
        }

	//Close the output file
	if (fclose(outFile) != 0) {
                printf("Error while closing the file\n");
                exit(1);
        }

	return 0; 
}     
