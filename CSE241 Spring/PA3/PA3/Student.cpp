#include <iostream>
#include <string>
#include "Course.h"
#include "Student.h"

using namespace std;

namespace PA3
{
    Student :: Student () : name (""), ID (0), courses (nullptr), num_of_courses (0), size_c (10) {} // default constructor 
    Student :: ~Student () //destructor
    {
        if (courses) 
        {
            for (int i = 0; i < num_of_courses; i++) 
            {
                delete courses [i];
            }
            delete [] courses;
        }
    } 
    string Student :: get_name () const // returns the student name
    {
        return name;
    }
    int Student :: get_ID () const // returns the student ID
    {
        return ID;
    }
    Course** Student :: get_courses () const // returns the course references
    {
        return courses;
    }
    int Student :: get_num_of_courses () const // returns the number of the courses
    {
        return num_of_courses;
    }
    void Student :: set_name (const string& new_name)  // sets the new name to the name
    {
        name = new_name;
    }
    void Student :: set_ID (int new_ID) // sets the new ID to the ID
    {
        ID = new_ID;
    }
    void Student :: add_course (Course* course) // adds the course to the student's courses
    {
        Course *new_course = new Course (); // creates a new course object and set its code and name
        new_course -> set_code (course -> get_code ());
        new_course -> set_name (course -> get_name ());

        if (!courses) // checks if the course references array is empty
        {
            courses = new Course* [size_c]; // if the array is empty, allocates memory for size_c which is initially ten
            courses [0] = new_course; 
        } 
        else 
        {
            if (num_of_courses >= size_c) // checks if the array is full
            { 
                Course **temp_courses = new Course* [size_c * 2]; // if the array is full, allocates memory for twice the size_c
        
                for (int i = 0; i < num_of_courses; i++) // copies the courses to a temporary array
                {
                    temp_courses [i] = courses [i]; 
                }
        
                temp_courses [num_of_courses] = new_course; // adds the new course to the end of the temporary array
                delete [] courses; // frees memory for the full array
                courses = temp_courses; // assigns the temporary array to the array
                size_c *= 2; // updates the size of the array
            }
            else 
            {
                courses [num_of_courses] = new_course; // if the array is not full, adds the new course to the end 
            }
        }
        num_of_courses++; // increments the number of the courses
    }
    void Student :: remove_course (Course* course) // removes the course from the student's courses
    {
        for (int i = 0; i < num_of_courses; i++) // searches in the course reference array 
        {
            if (courses [i] -> get_code () == course -> get_code ()) // if it founds the selected course in the array 
            {
                for (int j = i; j < num_of_courses - 1; j++) // replaces this place with the reference of the next object
                {
                    courses [j] = courses [j + 1];
                }
                courses [num_of_courses - 1] = nullptr; // assigns nullptr to the empty place
                num_of_courses--;
                break;
            }
        }
    }
}



