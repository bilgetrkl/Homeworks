#ifndef STUDENT_H
#define STUDENT_H
#include <iostream>
#include <string>
#include "Course.h"

using namespace std;

namespace PA3
{
    class Course;
    class Student
    {
    public:
        Student (); // default constructor
        ~Student (); // destructor
        string get_name () const; // returns the student name
        int get_ID () const; // returns the student ID
        Course** get_courses () const; // returns the course references
        int get_num_of_courses () const; // returns the number of the courses
        void set_name (const string& new_name); // sets the new name to the name
        void set_ID (int new_ID); // sets the new ID to the ID
        void add_course (Course* course); // adds the course to the student's courses
        void remove_course (Course* course); // removes the course from the student's courses
    private:
        string name; // name of the student
        int ID; // ID of the student
        Course** courses; // course references
        int num_of_courses;
        int size_c; // the current size of the course reference array
    };
}


#endif