#ifndef SCHOOLMANAGEMENTSYSTEM_H
#define SCHOOLMANAGEMENTSYSTEM_H
#include <iostream>
#include <sstream>
#include <string>
#include "Course.h"
#include "Student.h"

using namespace std;
namespace PA3
{
    class SchoolManagementSystem
    {
        public:
            SchoolManagementSystem (); // default constructor
            ~SchoolManagementSystem (); //destructor
            void main_menu (); // prints the main menu
            void student_menu (); // prints the student menu
            void add_student (); // takes the name and the ID of the student from the user and creates a student object
            Student* select_student (); // takes the name and the ID of the student from the user and searches for this student in the array
            void select_student_menu (); // prints the select student menu
            void delete_student (Student* student); // deletes the student
            Course** list_courses (Student* student); // returns and lists the courses except the ones which are already taken by the student
            Course** list_courses_taken (Student* student); // returns and lists the courses which are taken by the student
            void course_menu (); // prints the course menu
            void add_course (); // takes the name and the code of the course from the user and creates a course object
            Course* select_course (); // takes the name and the code of the course from the user and searches for this course in the array
            void select_course_menu (); // prints the select course menu
            void delete_course (Course* course); // deletes the course
            void list_students (Course* course); // lists students registered to the selected course
            void list_all_students (); // lists all students
            void list_all_courses (); // lists all courses
        private:
            Course** courses; // course references
            int num_of_courses;
            int size_c; // the current size of the course references array
            Student** students; // student references
            int num_of_students;
            int size_s; // the current size of the student references array
    };
}


#endif
