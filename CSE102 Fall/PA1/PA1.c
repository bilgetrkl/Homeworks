#include <stdio.h>
#include <math.h>
#define TRAINING_SAMPLE 10
#define EPSILON 0.001

int main () {
	
    	float x, y, sum_x1 = 0, sum_y1 = 0;
    	for (int i = 1; i <= TRAINING_SAMPLE; i++) { // retrieves the requested number of values from the user for class 1
        	scanf ("%f%f",&x,&y);
        	sum_x1 = sum_x1 + x; // the received x coordinate values of the class 1 are added one by one
        	sum_y1 = sum_y1 + y;  // the received y coordinate values of the class 1 are added one by one
    	}

    	float sum_x2 = 0, sum_y2 = 0; 
    	for (int i = 1; i <= TRAINING_SAMPLE; i++) { // retrieves the requested number of values from the user for class 2
    	    scanf ("%f%f",&x,&y);
    	    sum_x2 = sum_x2 + x; // the received x coordinate values of the class 2 are added one by one
    	    sum_y2 = sum_y2 + y; // the received y coordinate values of the class 2 are added one by one 
    	}
	
    	float avg_x1 = sum_x1 / TRAINING_SAMPLE; // average point of class 1 (x coordinate)
    	float avg_y1 = sum_y1 / TRAINING_SAMPLE; // average point of class 1 (y coordinate)
    	float avg_x2 = sum_x2 / TRAINING_SAMPLE; // average point of class 2 (x coordinate)
    	float avg_y2 = sum_y2 / TRAINING_SAMPLE; // average point of class 2 (y coordinate)

    	float mid_x = (avg_x1 + avg_x2) / 2; // midpoint of the line passing through the average points of class 1 and class 2 (x coordinate)
    	float mid_y = (avg_y1 + avg_y2) / 2; // midpoint of the line passing through the average points of class 1 and class 2 (y coordinate)
    	printf ("midpoint = %f %f", mid_x, mid_y); 

    	float slope;
    	if (avg_x2 - avg_x1 == 0) { // avoiding divide by zero situations
        	slope = -1 * (avg_y2 - avg_y1) / (avg_x2 - avg_x1 + EPSILON);
    	}
    	else { // the seperating line is perpendicular with the connecting line 
        	slope = -1 * (avg_y2 - avg_y1) / (avg_x2 - avg_x1); // -1 * slope of the connecting line
    	}
    	printf ("\nseperating line slope = %f\n", slope);

    	while (1) { // infinite loop
       	 	if (scanf ("%f%f",&x,&y) != 2) { // if the status indicates an unexpected input case, the loop ends and the program quits
       	   		printf ("q\n");
           	 	break; 
     	   	}
        	float distance1 = sqrt (pow (avg_x1 - x, 2) + pow (avg_y1 - y, 2)); // the distance between the average point of class 1 and the test point 
       		float distance2 = sqrt (pow (avg_x2 - x, 2) + pow (avg_y2 - y, 2)); // the distance between the average point of class 2 and the test point 
        	if (distance1 > distance2){ // the test point is closer to the average point of class 2
            		printf ("class 2, distance = %f\n", distance2); 
        	}
        	else { // the test point is closer to the average point of class 1
            		printf ("class 1, distance = %f\n", distance1);
        	}
    	}
    return 0;
}
