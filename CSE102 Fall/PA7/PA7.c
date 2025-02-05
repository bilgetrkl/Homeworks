#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT 0
#define OUTPUT 1
#define AND 2
#define OR 3
#define NOT 4
#define FLIPFLOP 5

struct gate
{
    int type; // type of the gate
    char name [20]; // name of the gate
    struct gate *inp1; // points to input gate1
    struct gate *inp2; // points to input gate2
    int output; // current output
    int former_out; // only for FLIPFLOP gate
    int flag; // for AND, OR, FLIPFLOP gates
};
void gates (char gate_type [15], struct gate *Gate)
{
    Gate -> inp1 = NULL;
    Gate -> inp2 = NULL;

    if (strcmp (gate_type, "INPUT") == 0) 
    { 
        Gate -> type = INPUT; // assings 0 the type
    } 
    else if (strcmp (gate_type, "OUTPUT") == 0) 
    {
        Gate -> type = OUTPUT; // assings 1 to the type
    } 
    else if (strcmp (gate_type, "AND") == 0) 
    {
        Gate -> type = AND; // assings 2 to the type
        Gate -> flag = -1; // if a gate is connected to inp1, this value will change
    } 
    else if (strcmp (gate_type, "OR") == 0) 
    {
        Gate -> type = OR; // assings 3 to the type
        Gate -> flag = -1; // if a gate is connected to inp1, this value will change
    } 
    else if (strcmp (gate_type, "NOT") == 0) 
    {
        Gate -> type = NOT; // assings 4 to the type
    } 
    else if (strcmp (gate_type, "FLIPFLOP") == 0) 
    {
        Gate -> type = FLIPFLOP; // assigns 5 to the type
        Gate -> former_out = 0; // when the gate is first created, it assigns the former out to 0
        Gate -> flag = -1; // once entered here this value will change
    }
}
void connection (char from_gate [15], char to_gate [15], struct gate **gate_arr, int gate_count)
{
    struct gate* from_gateptr = NULL; 
    struct gate* to_gateptr = NULL; 
    int i; // for the loop
    for (i = 0; i < gate_count; i++) // navigates through the array containing gate addresses
    {
        if (strcmp (gate_arr [i] -> name, from_gate) == 0) // determines the address of from gate in the file
        {
            from_gateptr = gate_arr [i]; // assigns a pointer here
        }
        else if (strcmp (gate_arr [i] -> name, to_gate) == 0) // determines the address of to gate in the file
        {
            to_gateptr = gate_arr [i]; // assigns a pointer here
        }
    }
    if (from_gateptr != NULL && to_gateptr != NULL)
    {  
        if (to_gateptr -> type == AND || to_gateptr -> type == OR) // two gates should be connected to or and and gates
        {   
            if (to_gateptr -> flag == -1) // if no connection has been done for this gate
            {
                to_gateptr -> inp1 = from_gateptr; // establishes the connection with first gate written in the file
                to_gateptr -> flag = 0; // changes the flag value
            }
            else // if a connection has been done for this gate
            { 
                to_gateptr -> inp2 = from_gateptr; // establishes the connection with second gate written in the file
            }
        }
        else  // only one gate should be connected to other gates
        {
            to_gateptr -> inp1 = from_gateptr; // establishes a connection 
        }
    }
}
void create_circuit (struct gate **gate_arr, int gate_count) 
{
    char line [100]; // holds a line
    char keyword [15]; // holds the keyword
    char name [20]; // holds the name of the gate
    char from_gate [15]; // holds the name of the from gate
    char to_gate [15]; // holds the name of the to gate
    char gate_type [15]; // holds the gate type
    int count = 0; // position in the gate array
     
    FILE* fileptr2 = fopen ("circuit.txt", "r");
    if (fileptr2 == NULL) 
    {
        printf ("File open error\n");
    }
    else 
    {   
        while (fgets (line, sizeof (line), fileptr2) != NULL) // gets the lines one by one
        {
            if (sscanf (line, "%s %s %s", keyword, gate_type, name) == 3 && strcmp (keyword, "GATE") == 0) // parses the lines and if the keyword is "GATE", it does the following 
            {
                struct gate *Gate = (struct gate *) malloc (sizeof (*Gate)); // creates a gate struct
                gate_arr [count] = Gate; // assings the address of the gate to the gate array
                count++;
                strcpy (Gate -> name, name); // assigns the name of the gate to the created struct
                gates (gate_type, Gate); // assigns the types of the gates created
            }
        }
        fclose (fileptr2);
    }

    FILE* fileptr3 = fopen ("circuit.txt", "r");
    if (fileptr3 == NULL) 
    {
        printf ("File open error\n");
    }
    else 
    { 
         while (fgets (line, sizeof (line), fileptr3) != NULL) // gets the lines one by one
         {  
            if (sscanf (line, "%s %s %s", keyword, from_gate, to_gate) == 3 && strcmp (keyword, "CONNECTION") == 0) // parses the lines and if the keyword is "CONNECTION", it does the following
            { 
                connection (from_gate, to_gate, gate_arr, gate_count); // connects the gates
            }
         }
    }
    fclose (fileptr3);
}
int evaluate (struct gate *gate) 
{ // evaluates the circuit in reverse
    if (gate -> type == INPUT) 
    {
        return gate -> output; // when it comes to the input gate, it should return the output
    }
    if (gate -> type == AND)
    {
        return evaluate (gate -> inp1) && evaluate (gate -> inp2); // when it comes to the and gate, the function must be called again for the gates connected to it 
    }
    else if (gate -> type == OR)
    {
        return evaluate (gate -> inp1) || evaluate (gate -> inp2); // when it comes to the or gate, the function must be called again for the gates connected to it 
    }
    else if (gate -> type == NOT)
    {
        return !evaluate (gate -> inp1); // when it comes to the not gate, it should return the inverse  of the function
    }
    else if (gate -> type == FLIPFLOP && gate -> flag == -1) 
    {
        if (gate -> former_out == 0) // if the former out is 0, the function must be called and the result of it should be equal to former out
        {
            gate -> former_out = evaluate (gate -> inp1);
            return gate -> former_out; 
        }
        else // if the former out is 1, the inverse of the function must be called and the result of it should be equal to former out
        {
            gate -> former_out = !evaluate (gate -> inp1);
            return gate -> former_out;
        } 
        gate -> flag = 0; // the value of the flag must change on first entry
    }  
    else // if the flag is 0 the former value should be returned
    {
        return gate -> former_out; 
    }  
    return 0;
}
void operations (struct gate **gate_arr, int gate_count)
{
    char line [100]; // holds a line
    int result; // holds the output 
    int i, j; // for the loops
    struct gate *outputptr = NULL;

    FILE* fileptr4 = fopen ("input.txt", "r");
    if (fileptr4 == NULL) 
    {
        printf ("File open error\n");
    }
    else 
    {
        for (i = 0; i < gate_count; i++) // finds the address of the output gate by traversing the gate array
        {
            if (gate_arr [i] -> type == OUTPUT)
            {
                outputptr = gate_arr [i]; // assigns a pointer to this address
                break;
            }    
        }
       while (fgets (line, 100, fileptr4) != NULL) // gets the lines one by one
       {
            int digits [100]; // holds the digits as integers
            int digit_num = 0; // holds the number of the digits
            for (i = 0; line [i] != '\0'; i++) // converts the characters from the file to the integers and stores them in the array
            {
                if (line [i] == '0' || line [i] == '1')
                {
                    digits [digit_num] = line [i] - '0';
                    digit_num++;
                }
            }
            j = 0;
            for (i = 0; i < gate_count; i++)
            { // searches for the input gates in the array containing addresses
                if (gate_arr [i] -> type == INPUT) 
                {
                    gate_arr [i] -> output = digits [j]; // assigns the bits taken from the input file to the input gates
                    j++;
                }  
            }
            result = evaluate (outputptr -> inp1); // evaluates the circuit
            printf ("%d\n", result); // prints the output result
       } 
    }
    fclose (fileptr4);
}
void free_circuit2 (struct gate *gate)
{
    if (gate == NULL)
    {
        return;
    }
    free_circuit2 (gate -> inp1); // deallocates the inp1 struct pointer
    free_circuit2 (gate -> inp2); // deallocates the inp2 struct pointer
}
void free_circuit (struct gate **gate_arr, int gate_count)
{
    for (int i = 0; i < gate_count; i++)
    {
        if (gate_arr [i] != NULL)
        {
            free_circuit2 (gate_arr [i]); // the input pointers to which the gates are connected are deallocated
            free (gate_arr [i]); // the gates where the addresses are kept are deallocated
        }
    }

    free (gate_arr);
}
int main () 
{
    int gate_count = 0; // number of the gates in circuit
    char ch;
    FILE* fileptr1 = fopen ("circuit.txt", "r");
    if (fileptr1 == NULL)
    {
        printf ("File open error\n");
    }
    else 
    {
        while ((ch = fgetc (fileptr1)) != EOF) // gets the first character of each line of the circuit.txt file
        {
            if (ch == 'G') // if this character is "G", it increases the number of gates
            {
                gate_count++;
            }
            while ((ch = fgetc (fileptr1)) != '\n' && ch != EOF); // moves to the end of line or end of file
        }
    }
    fclose (fileptr1);
    struct gate **gate_arr = (struct gate **) malloc (gate_count * sizeof (struct gate *)); // pointer array that will hold the addresses of the gates
    create_circuit (gate_arr, gate_count); // creates the circuit
    operations (gate_arr, gate_count); // puts the inputs in place and performs the evaluation
    free_circuit (gate_arr, gate_count); // deallocates the memory
    return 0;
}