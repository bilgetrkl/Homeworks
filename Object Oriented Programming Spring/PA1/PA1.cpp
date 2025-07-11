#include <iostream> 
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;
const int DIGIT = 17; // maximum size of arrays to hold numbers

void generate_random_number (int digit_num, int random_num []) // generates a hexadecimal random number
{
    srand (time (0)); // seeds the random number generator used by the rand () function
    int digit; // digit produced each step
    
    for (int i = 0; i < digit_num; i++) 
    {
        digit = rand () % 16; // a random number from 0 to 15 is generated
        random_num [i] = digit; // generated number is assigned to an int array step by step
        for (int j = 0; j < i; j++)
        {
            if (random_num [i] == random_num [j]) // it is checked to see if there are any repeating values ​​in the assigned numbers
            {
                i--; // if there is, another number is generated for that digit
                break;
            }
        }
    }  
}

void get_input (int digit_num, int user_num []) // receives input from the user and if the input is not valid, gives an error message and exits
{    
    string input1;
    getline (cin, input1); // receieves the input including the whitespaces

    string input2;
    for (char c : input1)  // removes spaces from the input
    {
        if (!isspace (c)) 
        {
            input2 += c;
        }
    }

    int i; // the length of the input
    for (i = 0; i < input2.length (); i++) // 
    {
        if (input2 [i] >= 48 && input2 [i] <= 57) // if the input digits are between 0 and 9
        {
            user_num [i] = input2 [i] - 48;
        }
        else if (input2 [i] >= 97 && input2 [i] <= 102) // if the input digits are between a to f
        {
            user_num [i] = input2 [i] - 87;
        }
        else // if the characters in the digit are not one of the ones above, the entered value is not an integer
        {
            cout << "E2" << endl; // prints the error message and exits
            exit (0);
        }
    }

    if (user_num [0] == 0) // if the first digit of the entered number is 0, this is not a valid number 
    {
        cout << "E1" << endl;
        exit (0);
    }

    if (i != digit_num) // if a number was not entered in the initially specified number of digits
    {
        cout << "E1" << endl;
        exit (0);
    }

    for (int j = 0; j < i; j++)
    {
        for (int k = j + 1; k < i; k++)
        {
            if (user_num [j] == user_num [k]) // if the digits of the entered number are not unique
            {
                cout << "E1" << endl;
                exit (0);
            }
        }
    }
}

void hints (int digit_num, int random_num [], int user_num [], int count) // how many digits are correct, how many numbers are correct
{
    int arr [DIGIT] = {0}; // an array that records how many times a number occurs
    int exact = 0; // correct digit number
    int misplaced = 0; // correct numbers but misplaced

    for (int i = 0; i < digit_num; i++)
    {
        if (random_num [i] == user_num [i]) // it is checked whether the digits of the randomly generated number and the number entered by the user are equal
        {
            exact++; // if they are equal the first hint will be incremented
        }
        else 
        { // if they are not, the value of the digits of both numbers will be incremented in the array
            arr [random_num [i]]++;
            arr [user_num [i]]++;
        }
    }

    if (exact == digit_num) // if the user finds all the digits, the FOUND message and how many tries the user found is printed
    {
        cout << "FOUND " << count << endl;
        exit (0);
    }

    for (int j = 0; j < DIGIT; j++)
    {
        if (arr [j] == 2) // since all digits are unique, if a number is recorded twice in this array then it is misplaced
        {
            misplaced++; 
        }
    }

    cout << exact << " " << misplaced << endl; // prints the hints
}

int main (int argc, char *argv []) // command line arguments
{    
    int exact, misplaced, digit_num; // first hint, second hint, number of digits of the number to be generated
    int count = 0; // number of guesses entered by the user
    int length = 0; // number of digits of the number entered for -u mode
    int random_num_arr [DIGIT] = {0}; // array to hold the randomly generated number
    int user_num_arr [DIGIT] = {0}; // array to hold the number entered by the user
    bool is_unique = true; // are the digits of the number entered for -u mode unique

    if (argc != 3 || (string (argv [1]) != "-r" && string (argv [1]) != "-u")) { // if command line arguments are not 3 or the second argument entered is not one of -r and -u 
        cout << "E0" << endl;
        exit (0);
    }

    if (string (argv [1]) == "-r") // -r mode
    {
        digit_num = atoi (argv [2]); // converts third argument to an integer

        if (digit_num <= 0 || digit_num > 16) // if the number of digits is less than 0 or greater than 16, prints an error message and exits
        {
            cout << "E0" << endl;
            exit (0);
        }
        else 
        {
            generate_random_number (digit_num, random_num_arr); // generates the random number

            do 
            {
                get_input (digit_num, user_num_arr); // receives the input from the user
                count++; // increments the number of guesses entered by user

                if (count > 100) // if the user cannot find the generated number in 100 iterations, prints FAILED message and exits
                {
                    cout << "FAILED" << endl;
                    exit (0);
                }

                hints (digit_num, random_num_arr, user_num_arr, count); // prints hints

            } while (1); // numbers will continue to be received from the user until some messages are received
        }
    }
    else if (string (argv [1]) == "-u") // -u mode
    {
        if (argv [2][0] == '0') // the number entered must not start with 0
        {
            cout << "E0" << endl;
            exit (0);
        }

        while (argv [2][length] != '\0') // finds the number of digits of the entered number
        {
            length++; 
        }

        for (int i = 0; i < length && is_unique; i++) // are the digits of the entered number unique
        {
            for (int j = i + 1; j < length && is_unique; j++)  
            {
                if (argv [2][i] == argv [2][j]) 
                {
                    is_unique = false; // if any two digits are matched this loop is broken
                }
            }
        }

        if (!is_unique) // if not unique, prints an error message and exits
        {
            cout << "E0" << endl;
            exit (0);
        }

        for (int i = 0; i < length; i++) // the entered number is put into an int array
        {
            if (argv [2][i] >= 48 && argv [2][i] <= 57) // if the input digits are between 0 and 9
            {
                random_num_arr [i] = argv [2][i] - 48;
            }
            else if (argv [2][i] >= 97 && argv [2][i] <= 102) // if the input digits are between a to f
            {
                random_num_arr [i] = argv [2][i] - 87;
            }
            else // if the characters in the digit are not one of the ones above, the entered value is not valid
            {
                cout << "E0" << endl; // // prints the error message and exits
                exit (0);
            }
        }        
        
        do 
        {
            get_input (length, user_num_arr);
            count++; // increments the number of guesses entered by user    

            if (count > 100) // if the user cannot find the generated number in 100 iterations, prints FAILED message and exits
            {
                cout << "FAILED" << endl;
                exit (0);
            }
                hints (length, random_num_arr, user_num_arr, count); // prints hints
                
        } while (1); // // numbers will continue to be received from the user until some messages are received
    }

    return 0;
}