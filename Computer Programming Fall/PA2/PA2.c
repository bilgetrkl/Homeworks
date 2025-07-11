#include <stdio.h>

int main () {
	int counter [100], two_gram1 [100], two_gram2 [100]; 
	int i = 0;

	FILE *fileptr1 = fopen ("input.txt","r"); // opens the input.txt file in read mode
	
	if (fileptr1 == NULL) { 
		printf ("File open error");
	}
	
	else { // assigns all integers in the input.txt file to arrays as 2-grams
		fscanf (fileptr1, "%d", &two_gram1 [i]); // saves the first integer it reads as the first term of the first array
		while (fscanf (fileptr1, "%d", &two_gram2 [i]) != EOF) { // reads other integers to the end of file and saves them in the second array
			two_gram1 [i+1] = two_gram2 [i]; // assigns the terms of the second array to the next term of the first array
			i++; 
		}
	}

	fclose (fileptr1); // closes the input.txt file

	int j = 0;
	while (j <= i) { // until the last term of the arrays
		counter [j] = 0;
		counter [j]++; // repetition of jth terms of arrays 
		for (int n = 1; n <= j; n++) { 
			if (two_gram1 [j] == two_gram1 [j-n] && two_gram2 [j] == two_gram2 [j-n]) { // checks if there are 2-grams appearing more than once in the arrays
				j = j - n; // goes to the location of previously saved 2-gram
				counter [j]++; // increases the repetition of 2-gram at this location
				j = j + n; // returns to the first location
				int p = j; // saves this location
				counter [j]--; // it saved the repetition of the 2-gram in the location where it was saved before, so it deletes this repetition
				for (int t = 0; t < i; t++) { // 2-grams that appear more than once should be deleted from this location so it shifts next terms to the left
					two_gram1 [j] = two_gram1 [j+1]; 
					two_gram2 [j] = two_gram2 [j+1];
					j++;
				}
				i--; //  2-grams that appear more than once are placed in a single location, so the number of the terms of the arrays decreases
				j = p;	// goes back to saved location
				j--; // since j++ is the next statement, the location will not change because the next terms arrived at this location should be checked 
			}
		}
		j++; // next term of the arrays
	}


	FILE *fileptr2 = fopen ("output.txt", "w"); // opens the output.txt file in write mode

	if (fileptr2 == NULL) {
		printf ("File open error");
	}
	
	else {
		for (int k = 0; k < i; k++) { // writes 2-grams and their repetitions to the output.txt file
			fprintf (fileptr2, "%d %d: %d\n", two_gram1 [k], two_gram2 [k], counter [k]);
		}
	}
	
	fclose (fileptr2); // closes the output.txt file

	return 0;
}
