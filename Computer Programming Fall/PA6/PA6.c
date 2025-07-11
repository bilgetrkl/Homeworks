#include <stdio.h>
#include <string.h>

#define MAX_CHAR 27
#define EPSILON 0.001

float bigram_x [MAX_CHAR][MAX_CHAR];
float bigram [MAX_CHAR][MAX_CHAR];

void finding_bigrams (char *filename, float bigrams [MAX_CHAR][MAX_CHAR])
{ // function to find bigrams in a file and populate the bigram matrix
	FILE *fileptr = fopen (filename, "r");
	int ch1, ch2; // characters in bigram
	int a, b; // position of characters in the matrix
	if (fileptr == NULL) 
	{ 
		printf ("File open error");
	}
	else 
	{
		for (int i = 0; i < MAX_CHAR; i++) 
		{ 
			for (int j = 0; j < MAX_CHAR; j++) 
			{	
				bigrams [i][j] = 0; // initialize the bigram matrix to zeros
			}
		}	
		ch1 = getc (fileptr); // getting the first character from the file
		for (int i = 97; i <= 122 ; i++) 
		{
			if (ch1 == 32 || ch1 == 10) // if the character is space or new line character
			{
				a = 26; 
				break;
			}
			else if (ch1 == i) // if the character is one of the letters
			{ 
				a = i - 97;
				break;
			}    
		}
		
		while ((ch2 = getc (fileptr)) != EOF) // getting the other characters until end of line
		{
			for (int i = 97; i <= 122 ; i++) 
			{
				if (ch1 != 32 && ch2 == 32 || ch1 != 32 && ch2 == 10) // if the received character is a space or a new line, the previous character should not be a space or a new line
				{
					b = 26;
					break;
				}
				else if (ch2 == i) // if the character is one of the letters
				{ 
					b = i - 97;
					break;
				}   
			}
		
			if ((ch1 == 32 && ch2 == 32) || (ch1 == 32 && ch2 == 10) || (ch1 == 10 && ch2 == 32) || (ch1 == 10 && ch2 == 10)) 
			{ // if both characters are spaces or newlines, they do not form a bigram
				bigrams [a][b] = 0;
			}
			else 
			{
				bigrams [a][b]++;
			}
			a = b;
			ch1 = ch2;
		}
		fclose (fileptr);
	}
}

void frequency (char *filename, float bigrams [MAX_CHAR][MAX_CHAR])
{ // function to calculate frequencies of bigrams
	float count = 0;
	
	if (strcmp (filename, "language_x.txt") != 0) // we found the values ​​of the language_x.txt file in the main function
	{ 
		finding_bigrams (filename, bigrams); // finding the values ​​of other files
	}

	for (int i = 0; i < MAX_CHAR; i++) 
	{
		for (int j = 0; j < MAX_CHAR; j++) 
		{
			count = count + bigrams [i][j]; // finding the sum of all bigrams
		}
	}
	
	for (int i = 0; i < MAX_CHAR; i++) 
	{
		for (int j = 0; j < MAX_CHAR; j++)
		{
			if (count == 0)
			{ // dealing with the situation where the denominator is zero
				bigrams [i][j] = bigrams [i][j] / (count + EPSILON);
			}	
			else
			{
				bigrams [i][j] = bigrams [i][j] / count; // finding the frequency of bigrams
			}
		}
	}
}

void dissimilarity (char *filename, float bigram_x [MAX_CHAR][MAX_CHAR]) 
{ // function to calculate dissimilarity between two sets of bigrams
	float diss = 0;
	frequency (filename, bigram); // finding the frequency of bigrams of other files

	for (int i = 0; i < MAX_CHAR; i++) 
	{
		for (int j = 0; j < MAX_CHAR; j++)
		{
			bigram [i][j] = bigram_x [i][j] - bigram [i][j]; 
			if (bigram [i][j] < 0) 
			{ // get rid of minus
				bigram [i][j] = -1 * bigram [i][j]; 
			}
			diss = diss + bigram [i][j];
		}
	}
	printf ("%f\n", diss); // printing the dissimilarity values
}

int main () 
{
	finding_bigrams ("language_x.txt", bigram_x); // finding the bigram values of the language_x.txt file
	frequency ("language_x.txt", bigram_x); // finding the frequency of bigrams of the language_x.txt file
	dissimilarity ("language_1.txt", bigram_x);
	dissimilarity ("language_2.txt", bigram_x);
	dissimilarity ("language_3.txt", bigram_x);
	dissimilarity ("language_4.txt", bigram_x);
	dissimilarity ("language_5.txt", bigram_x);
	return 0; 
}
