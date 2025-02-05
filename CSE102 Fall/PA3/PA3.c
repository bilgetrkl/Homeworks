#include <stdio.h>
#include <math.h>

double evaluation (double *value, char *polynomial); // Function to evaluate a polynomial
char *next_term (char *term, double *coefficient, int *power); // Function to get the next term in the polynomial expression

int main() {
	double values[100]; // Array to store values read from values.txt
    double result; // Result of polynomial evaluation
    char line[1000]; // Buffer to read a line from polynomials.txt
    int i, j;
    
    FILE *fileptr1, *fileptr2, *fileptr3; // File pointers for input and output files

    fileptr1 = fopen("values.txt", "r"); // Open values.txt for reading
    if (fileptr1 == NULL) {
        printf("Error opening values.txt");
        return 1;
    } 
	else { // Read values from values.txt
        i = 0;
        while (fscanf(fileptr1, "%lf", &values[i]) != EOF) {
            i++;
        }
        fclose(fileptr1);
    }

    fileptr2 = fopen("polynomials.txt", "r"); // Open polynomials.txt for reading
    if (fileptr2 == NULL) {
        printf("Error opening polynomials.txt");
        return 1;
    }

    fileptr3 = fopen("evaluations.txt", "w"); // Open evaluations.txt for writing
    if (fileptr3 == NULL) {
        printf("Error opening evaluations.txt");
        fclose(fileptr2);
        return 1;
    }

    while (fgets(line, sizeof(line), fileptr2) != NULL) { // Read each line from polynomials.txt
        for (j = 0; j < i; j++) { // Evaluate the polynomial for each value 
            result = evaluation (&values [j], line); // Calculate the result for the current value
            fprintf(fileptr3, "%.2lf ", result); // Write the result to evaluations.txt
        }
        fprintf(fileptr3, "\n"); // Move to the next line in evaluations.txt
    }
    // Close files
    fclose(fileptr2);
    fclose(fileptr3);

    return 0;
}

double evaluation(double *value, char *polynomial) {
    double result = 0.0, coefficient;
    int power;
    char *term = polynomial; // Pointer to the current term in the polynomial

    while (*term != '\0') { // Process each term in the polynomial
        // Get the next term and update the result
        term = next_term(term, &coefficient, &power);
        result += coefficient * pow(*value, power);
    }

    return result;
}

char *next_term (char *term, double *coefficient, int *power) {
    int read = sscanf (term, "%lfx^%d", coefficient, power);
    if (read == 2) {
        sscanf(term, "%lfx^%d", coefficient, power); // Term with power
        while (*term != '+' && *term != '\n' && *term != '\0') { // Move to the next term
            term++;
        }
        if (*term == '+' || *term == '\n') {
            term++;
        }
    } 
	else { // Term without power
        read = sscanf (term, "%lfx", coefficient);
        if (read == 1) {
            sscanf(term, "%lfx", coefficient);
            *power = 1;
        } 

	else {
           	*power = 0;
        }
        
        while (*term != '+' && *term != '\n' && *term != '\0') { // Move to the next term
            term++;
        }
        if (*term == '+' || *term == '\n') {
            term++;
        }
    }
    return term;
}
