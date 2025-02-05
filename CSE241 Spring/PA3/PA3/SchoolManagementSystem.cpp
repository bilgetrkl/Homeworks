#include <iostream>
#include <sstream>
#include <string>
#include "Course.h"
#include "Student.h"
#include "SchoolManagementSystem.h"

using namespace std;
namespace PA3
{
    SchoolManagementSystem :: SchoolManagementSystem () : courses (nullptr), num_of_courses (0), size_c (10), students (nullptr), num_of_students (0), size_s (10) {} // default constructor
    SchoolManagementSystem :: ~SchoolManagementSystem () // destructor
    {   
        // deallocates memory for course references
        for (int i = 0; i < num_of_courses; i++) 
        {
            delete courses [i];
        }
        delete [] courses;

        // deallocates memory for student references
        for (int i = 0; i < num_of_students; i++) 
        {
        delete students [i];
        }
        delete [] students;
    }
    void SchoolManagementSystem :: main_menu () // prints the main menu
    {
        cout << "0 exit\n"
             << "1 student\n"
             << "2 course\n"
             << "3 list_all_students\n"
             << "4 list_all_courses\n";
    }
    void SchoolManagementSystem :: student_menu () // prints the student menu
    {
        cout << "0 up\n"
             << "1 add_student\n"
             << "2 select_student\n";
    }
    void SchoolManagementSystem :: add_student () // takes the name and the ID of the student from the user and creates a student object
    {
        string input, name, ID_str, word;
        cout << ">> ";
        getline (cin, input); // gets the line entered by the user
        stringstream ss (input); // creates a string stream to parse the input
        while (ss >> word) // iterates through each word in the input
        {
            if (!name.empty ()) // if name is not empty, adds a space before appending the next word
            {
                name += " ";
            }
            if (ss.eof ()) // checks if the end of file is reached to determine the last word as ID_str
            {
                ID_str = word;
            }
            else
            {
                name += word;
            }
        }
        int ID = 0;
        if (!ID_str.empty ())
        {
            ID = stoi (ID_str); // converts the ID string to an integer
        }
        Student *new_student = new Student (); // creates a new student object and set its name and ID
        new_student -> set_name (name);
        new_student -> set_ID (ID);

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
    Student* SchoolManagementSystem :: select_student () // takes the name and the ID of the student from the user and searches for this student in the array
    {
        string input, name, ID_str, word;
        cout << ">> ";
        getline (cin, input); // gets the line entered by the user
        stringstream ss (input); // creates a string stream to parse the input
        while (ss >> word) // iterates through each word in the input
        {
            if (!name.empty ()) // if name is not empty, adds a space before appending the next word
            {
                name += " ";
            }
            if (ss.eof ()) // checks if the end of file is reached to determine the last word as ID_str
            {
                ID_str = word;
            }
            else
            {
                name += word;
            }
        }
        int ID = 0;
        if (!ID_str.empty ())
        {
            ID = stoi (ID_str); // converts the ID string to an integer
        }

        for (int i = 0; i < num_of_students; i++) 
        {
            if (students [i] -> get_ID () == ID) 
            {
                return students [i]; 
            }
        }
    }
    void SchoolManagementSystem :: select_student_menu () // prints the select student menu
    {
        cout << "0 up\n"
             << "1 delete_student\n"
             << "3 add_selected_student_to_a_course\n"
             << "4 drop_selected_student_from_a_course\n";
    }
    void SchoolManagementSystem :: delete_student (Student* student) // deletes the student 
    {
        for (int i = 0; i < num_of_students; i++) // searches in the whole course references array
        {
            if (students [i] -> get_ID () == student -> get_ID ()) // if it founds the selected student in the array 
            {
                Course** student_courses = students [i] -> get_courses (); // creates a temporary course reference array for this student and assigns the student's courses to it
                for (int j = 0; j < students [i] -> get_num_of_courses (); j++) // searches for the student's course in the whole course references array
                {
                    for (int k = 0; k < num_of_courses; k++)
                    {
                        if (courses [k] -> get_code () == student_courses [j] -> get_code ()) // if it founds the student's courses in the array
                        {
                            courses [k] -> remove_student (student); // removes the student from this course's student reference array
                            break;
                        }
                    }
                }
                delete student; 
                for (int j = i; j < num_of_students - 1; j++) // and then removes the student from the whole student references array
                {
                    students [j] = students [j + 1];
                }
                students [num_of_students - 1] = nullptr; // assigns nullptr to the empty place
                num_of_students--;
            }
        }
    }
    Course** SchoolManagementSystem :: list_courses (Student* student) // returns and lists the courses except the ones which are already taken by the student
    {
        Course** taken_courses = student -> get_courses (); // creates a temporary course reference array and assigns the selected student's courses to it
        Course** available_courses = new Course* [num_of_courses - (student -> get_num_of_courses ())]; // creates a dynamic array for the references of the courses which are not taken by the selected student 
        int num_of_available_courses = 0; 

        for (int i = 0; i < num_of_courses; i++)  // searches in the whole course reference array
        {
            bool taken = false; // initially it is not taken
            for (int j = 0; j < student -> get_num_of_courses (); j++) // searches in the student's courses array
            {
                if (courses [i] -> get_code () == taken_courses [j] -> get_code ()) // if it founds the courses in the array
                {
                    taken = true; // then it is taken 
                    break;
                }
            }
            if (!taken) // if it is not taken 
            {
                available_courses [num_of_available_courses] = courses [i]; // then it adds this course to the available courses references array
                num_of_available_courses++;
            }
        }
        for (int i = 0; i < num_of_available_courses; i++) // lists these courses
        {
            cout << i + 1 << " " << available_courses [i] -> get_code () << " "<< available_courses [i] -> get_name () << endl;
        }
        return available_courses;
    }   
    Course** SchoolManagementSystem :: list_courses_taken (Student* student) // returns and lists the courses which are taken by the student
    {
        Course** student_courses = student -> get_courses ();
        for (int i = 0; i < student -> get_num_of_courses (); i++) 
        {
            cout << i + 1 << " " << student_courses [i] -> get_code () << " " << student_courses [i] -> get_name () << endl;
        }
        return student_courses;
    }
    void SchoolManagementSystem :: course_menu () // prints the course menu 
    {
        cout << "0 up\n"
             << "1 add_course\n"
             << "2 select_course\n";
    }
    void SchoolManagementSystem :: add_course () // takes the name and the code of the course from the user and creates a course object
    {
        string input, code, name;
        cout << ">> ";
        getline (cin, input); // gets the line entered by the user
        stringstream ss (input); // creates a string stream to parse the input
        ss >> code; // parses the input to get the course code
        getline (ss, name); // reads the remaining input as the course name
        if (!name.empty () && name [0] == ' ') // removes the leading space from the name if it exists
        {
            name.erase (0, 1);
        }

        Course *new_course = new Course (); // creates a new course object and set its code and name
        new_course -> set_code (code);
        new_course -> set_name (name);

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
    Course* SchoolManagementSystem :: select_course () // takes the name and the code of the course from the user and searches for this course in the array and returns it
    {
        string input, code, name;
        cout << ">> ";
        getline (cin, input); // gets the line entered by the user
        stringstream ss (input); // creates a string stream to parse the input
        ss >> code; // parses the input to get the course code
        getline (ss, name); // reads the remaining input as the course name
        if (!name.empty () && name [0] == ' ') // removes the leading space from the name if it exists
        {
            name.erase (0, 1);
        }
        for (int i = 0; i < num_of_courses; i++) // searches the course entered by the user in the whole courses array 
        {
            if (courses [i] -> get_code () == code) 
            {
                return courses [i]; 
            }
        }
    }
    void SchoolManagementSystem :: select_course_menu () // prints the select course menu
    {
        cout << "0 up\n"
             << "1 delete_course (after delete, print course menu)\n"
             << "2 list_students_registered_to_the_selected_course\n";
    }
    void SchoolManagementSystem :: delete_course (Course* course) // deletes the course
    {
        for (int i = 0; i < num_of_courses; i++) // searches in the selected courses's student reference array
        {
            if (courses [i] -> get_code () == course -> get_code ()) // if it founds the selected course in the array 
            {
                Student** course_students = courses [i] -> get_students (); // creates a temporary student reference array for this course and assigns the course's students to it
                for (int j = 0; j < courses [i] -> get_num_of_students (); j++) // searches for the course's student in the whole student references array
                {
                    for (int k = 0; k < num_of_students; k++)
                    {
                        if (students [k] -> get_ID () == course_students [j] -> get_ID ()) // if it founds the course's students in the array
                        {
                            students [k] -> remove_course (course); // removes the course from this student's course reference array
                            break;
                        }
                    }
                }
                delete course;
                for (int j = i; j < num_of_courses - 1; j++) // and then removes the course from the whole course references array
                {
                    courses [j] = courses [j + 1];
                }
                courses [num_of_courses - 1] = nullptr; // assigns nullptr to the empty place
                num_of_courses--;
            }
        }
    }
    void SchoolManagementSystem :: list_students (Course* course) // lists students registered to the selected course
    {
        Student** registered_students = course -> get_students ();

        for (int i = 0; i < course -> get_num_of_students (); i++) 
        {
            cout << registered_students [i] -> get_name () << registered_students [i] -> get_ID () << endl;
        }
    }
    void SchoolManagementSystem :: list_all_students () // lists all students
    {
        for (int i = 0; i < num_of_students; i++) 
        {
            cout << students [i] -> get_name () << students [i] -> get_ID () << endl;
        }
    }
    void SchoolManagementSystem :: list_all_courses () // lists all courses
    {
        for (int i = 0; i < num_of_courses; i++) 
        {
            cout << courses [i] -> get_code () << " " << courses [i] -> get_name () << endl;
        }
    }
}
