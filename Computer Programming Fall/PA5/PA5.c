#include <stdio.h>
#include <string.h>

#define CARB_CAL_1G 4     	// number of calories in 1 g carbohydrate
#define PROTEIN_CAL_1G 4  	// number of calories in 1 g protein
#define FAT_CAL_1G 9      	// number of calories in 1 g fat
#define WATER_CAL_1G 0    	// number of calories in 1 g water
#define MAX_LENGTH 31 		// maximum number of characters of components
#define MAX_COMP 21		// maximum number of components
#define MAX_INPUT_LENGTH 1000 	// maximum number of characters of the input received from the user

typedef struct {
	double amount;		// grams of components
	double carb_amount;	// grams of carbs
	double protein_amount;	// grams of proteins
	double fat_amount;	// grams of fats
} Nutrients;

void get_and_parse (char components [MAX_COMP][MAX_LENGTH], int* count) {	// retrieves components from the user
	char input1 [MAX_INPUT_LENGTH];						// the array where the input received from the user will be kept

	while (getchar () != '\n');
	scanf ("%999[^\n]", input1);						// receiving input from the user 

	int input1_size = strlen (input1);
	char input2 [input1_size];
	int input2_size = 0;
	
	for (int i = 0; i < input1_size; ++i) {					// clears empty characters from received input
		if (input1 [i] != ' ') {
			input2 [input2_size] = input1 [i];			// the empty space-free version of the input is transferred to another array
			++input2_size;
		}
	}
	input2 [input2_size] = '\0';
	

    *count = 0; 								// number of components entered by user
    int i = 0;
    while (sscanf (input2 + i, "%30[^,]", components [*count]) == 1) {		// parse input into components according to commas
        (*count)++;
        i += strlen (components [*count - 1]);
        if (input2 [i] == ',') {
            i++;
        }
    }
}
	
int is_base (char component [MAX_LENGTH]) {	// checks whether the component is a leaf node or not
	int comp_length = strlen (component);
	for (int i = 0; i < comp_length; i++) {
		if (component [i] == '(') {
			return 1;
		}
	}
	return 0;
}

void process (char* name, Nutrients* p) {
	if (is_base (name) == 1) {								// if it is a leaf node
		char comp_name [MAX_LENGTH];
		sscanf (name, "%30[^()] (%lfg)", comp_name, &p -> amount);			// parses component name and gram
		printf ("> List the amount macro nutrients in %s:\n> ", comp_name);
		scanf ("%lf%lf%lf", &p -> carb_amount, &p -> protein_amount, &p -> fat_amount);	// retrieves nutritional percentages from the user
		p -> carb_amount = (p -> carb_amount * p -> amount) / 100;
		p -> protein_amount = (p -> protein_amount * p -> amount) / 100;
		p -> fat_amount = (p -> fat_amount * p -> amount) / 100;
		// and finds its grams
	} else { // if it is not a leaf node
		char components [MAX_COMP][MAX_LENGTH];		
		int comp_num;
		printf ("> What is %s?:\n> ", name);
		get_and_parse (components, &comp_num);		// calls this function to get the components
		for (int i = 0; i < comp_num; i++) {		// all components are processed
			Nutrients t = { 0 };
			process (components [i], &t);		// calls itself again	
			p->amount += t.amount;
       			p->carb_amount += t.carb_amount;
        		p->protein_amount += t.protein_amount;
        		p->fat_amount += t.fat_amount;		
			// the values taken from each component is summed to the previous values
		}
	}
}

int main () {
	Nutrients nutrients = { 0 };
	char meal [MAX_LENGTH]; 		// array that will hold the meal name
	printf ("> What is the meal?:\n> ");
	scanf ("%s", meal);			// gets the meal name from the user
	process (meal, &nutrients);
	
	double total_calories = (nutrients.carb_amount * CARB_CAL_1G + nutrients.protein_amount * PROTEIN_CAL_1G + nutrients.fat_amount * FAT_CAL_1G);

	double carb_percentage = ((nutrients.carb_amount * CARB_CAL_1G) / total_calories) * 100;
	double protein_percentage = ((nutrients.protein_amount * PROTEIN_CAL_1G) / total_calories) * 100;
	double fat_percentage = ((nutrients.fat_amount * FAT_CAL_1G) / total_calories) * 100;
    	printf ("Total calories is %.2lf in %.2lfg %s. %%%.2lf calories from carbs, %%%.2lf from proteins, %%%.2lf from fats.\n",
        	total_calories, nutrients.amount, meal, carb_percentage, protein_percentage, fat_percentage);
	

    return 0;
}

