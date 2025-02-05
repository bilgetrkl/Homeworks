#include <iostream>
#include <string>
#include "Student.h"
#include "Course.h"

using namespace std;
namespace PA3
{
    Course :: Course ()  : name (""), code (""), students (nullptr), num_of_students (0), size_s (10) {} // default constructor
    Course :: ~Course () // destructor
    {
        if (students) 
        {
            for (int i = 0; i < num_of_students; i++) 
            {
                delete students[i];
            }
            delete [] students;
        }
    } 
    string Course :: get_name () const // returns the name of the course
    {
        return name;
    }
    string Course :: get_code () const // returns the code of the course
    {
        return code;
    }
    Student** Course :: get_students () const // returns the student references
    {
        return students;
    }
    int Course :: get_num_of_students () const // returns the number of the students
    {
        return num_of_students;
    }
    void Course :: set_name (const string& new_name) // sets the new name to the name
    {
        name = new_name;
    }
    void Course :: set_code (const string& new_code) // sets the new code to the code
    {
        code = new_code;
    }
    void Course :: add_student (Student* student) // adds the student to the course 
    {
        Student *new_student = new Student (); // creates a new student object and set its name and ID
        new_student -> set_name (student -> get_name ());
        new_student -> set_ID (student -> get_ID ());

        if (!students) // checks if the student references array is empty
        {
            students = new Student* [size_s]; // if the array is empty, allocates memory for size_s which is initially ten
            students [0] = new_student; 
        } 
        else 
        {
            if (num_of_students >= size_s) // checks if the array is full
            { 
                Student **temp_students = new Student* [size_s * 2]; // if the array is full, allocates memory for twice the size_s
                for (int i = 0; i < num_of_students; i++) // copies the students to a temporary array
                {
                    temp_students [i] = students [i]; 
                }
        
                temp_students [num_of_students] = new_student; // adds the new student to the end of the temporary array
                delete [] students; // frees memory for the full array
                students = temp_students; // assigns the temporary array to the array
                size_s *= 2; // updates the size of the array
            }
            else 
            {
                students [num_of_students] = new_student; // if the array is not full, adds the new student to the end 
            }
        }
        num_of_students++; // increments the number of the students
    }
    void Course :: remove_student (Student* student) // removes the student from the course
    {
        for (int i = 0; i < num_of_students; i++) // searches in the student reference array 
        {
            if (students [i] -> get_ID () == student -> get_ID ()) // if it founds the selected student in the array  
            {
                for (int j = i; j < num_of_students - 1; j++) // replaces this place with the reference of the next object
                {
                    students [j] = students [j + 1];
                }
                students [num_of_students - 1] = nullptr; // assigns nullptr to the empty place
                num_of_students--; 
                break;
            }
        }
    }
}
