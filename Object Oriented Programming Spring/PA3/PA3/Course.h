#ifndef COURSE_H
#define COURSE_H
#include <iostream>
#include <string>
#include "Student.h"

using namespace std;
namespace PA3
{
    class Student;
    class Course
    {
        public:
            Course (); // default constructor
            ~Course (); // destructor
            string get_name () const; // returns the name of the course
            string get_code () const; // returns the code of the course
            Student** get_students () const; // returns the student references
            int get_num_of_students () const; // returns the num of the students
            void set_name (const string& new_name); // sets the new name to the name
            void set_code (const string& new_code); // sets the new code to the codeS
            void add_student (Student* student); // adds the student to the course
            void remove_student (Student* student); // removes the student from the course
        private:
            string name; // name of the course
            string code; // code of the course
            Student** students; // student references
            int num_of_students;
            int size_s; // the current size of the student reference array
    };
}


#endif
