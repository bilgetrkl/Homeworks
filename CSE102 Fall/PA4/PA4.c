#include <stdio.h>
#include <string.h>

#define MAX_LINE 101
#define MAX_CHAR 251
#define MAX_WORD_LENGTH 11

char lines [MAX_LINE][MAX_CHAR]; // character array containing the lines in the input2.txt file
char word [MAX_WORD_LENGTH]; // array holding the current word in the input1.txt file
int line_length [MAX_LINE]; // array containing the length of lines in the input2.txt file	
int rows; // number of lines in the input2.txt file
int word_length; // number of letters of the current word in the input1.txt file
int max_length; // length of the longest line in the input2.txt file

int is_horizontal (int row, int column) {
	int i;
	if (line_length [row] - column >= word_length) { // checks to see if the searched word fit in the remaining line
		for (i = 1; i < word_length; i++) { // checks other letters of the word
			if (lines [row][column + i] == word [i] ) { // continue to look at the letters 
				continue;
			} else { // even one letter is different, exits the loop
				break;}
		}
		if (i == word_length) { // this happens if it reaches the last letter 
			return 1;
		} else { 
			return 0; }
	} else { // it doesn't fit on the line
		return 0; }
}

int is_vertical (int row, int column) {
	int i;
	if (rows - row >= word_length) { // checks to see if the searched word fit in the remaining column
		for (i = 1; i < word_length; i++) { // checks other letters of the word	
			if (lines [row + i][column] == word [i]) { // continue to look at the letters
				continue;
			} else { // even one letter is different, exits the loop
				break; }
		}
		if (i == word_length) { // this happens if it reaches the last letter
			return 1;
		} else {
			return 0; }	
	} else { // it doesn't fit on the column
		return 0; }
}

int is_diagonal (int row, int column) {
	int i;
	if (rows - row >= word_length && max_length - column >= word_length) { // checks to see if the searched word fit in the maximum remaining line or remaining column 
		for (i = 1; i < word_length; i++) { // checks other letters of the word
			if (lines [row + i][column + i] != '\0' && lines [row + i][column + i] == word [i]) { // continue to look at the letters if they are not null characters
				continue;
			} else { // even one letter is different, exists the loop
				break; }
		}
		if (i == word_length) { // this happens if it reaches the last letter
			return 1;
		} else {
			return 0; }
	} else {
		return 0; }
}

int main () {
    	FILE *input1, *input2, *output; // filepointers
	
	for (int i = 0; i < MAX_LINE; i++) { // assigns null characters to all characters in the array
		for (int j = 0; j < MAX_CHAR; j++) {
			lines [i][j] = '\0';
		}
	}	
	
	input2 = fopen ("input2.txt", "r");
    	if (input2 == NULL) {
        	printf ("Error opening input2.txt");
    	} else {
        	rows = 0;
        	while (fgets (lines [rows], sizeof (lines [0]), input2) != NULL && rows < MAX_LINE) { // gets lines into 2D character array
				line_length [rows] = strlen (lines [rows]) - 1; // finds the length of lines
				if (line_length [rows] > 0 && lines [rows][line_length [rows]] == '\n') { // removes the newline characters 
                		lines [rows][line_length [rows]] = '\0';
            	}
            		rows++;
        	}
    	}
    	fclose (input2);
	
	for (int i = 0; i < rows; i++) { // finds the longest line in the input2.txt file
        	if (line_length [i] > max_length) {
            		max_length = line_length [i];
        	}
    	}

    	input1 = fopen ("input1.txt", "r");
    	output = fopen ("output.txt", "w");
    	if (input1 == NULL || output == NULL) {
        	printf ("Error opening files");
    	} else {
        	while (fgets (word, sizeof (word), input1) != NULL) { // gets words into character array one by one
            		word_length = strlen (word) - 1; // finds the length of the current word
            		if (word_length > 0 && word [word_length] == '\n') { // removes the newline characters
                		word [word_length] = '\0';
            		}
            		for (int i = 0; i < rows; i++) { // searches for the first letter of the word in the 2D character array
                		for (int j = 0; j < line_length [i]; j++) {
                    			if (lines [i][j] == word [0]) { 
                        			if (is_horizontal (i, j) == 1) { // if the word is found horizontally 
                            				fprintf (output, "%s (%d,%d) Horizontal\n", word, i + 1, j + 1);
                        			}	 
									else if (is_vertical (i, j) == 1) { // if the word is found vertically 
                            				fprintf (output, "%s (%d,%d) Vertical\n", word, i + 1, j + 1);
									}
									else if (is_diagonal (i, j) == 1) { // if the word is found diagonally 
											fprintf (output, "%s (%d,%d) Diagonal\n", word, i + 1, j + 1);
									}
                    			}
                		}
            		}
        	}
    	}
    	fclose (input1);
 	fclose (output);

	return 0;
}
