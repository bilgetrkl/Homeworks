#include <iostream>
#include <cstring>
#include "SchoolManagementSystem.h"
#include "Student.h"
#include "Course.h"

using namespace std;

int main ()
{
    PA3 :: SchoolManagementSystem system; // creates an instance of SchoolManagementSystem
    int choice, ID;
    string name_s, name_c, code;
    bool flag_m = true, flag_1, flag_2, flag_3, flag_4;
    cout << "Main_menu\n";

    while (flag_m) // main menu loop
    {
        system.main_menu (); // displays main menu options
        cout << ">> ";
        cin >> choice;
        cin.ignore ();
        switch (choice)
        {   
            case 0:
                flag_m = false; // exits the program 
                break;
            case 1:
                flag_1 = true;
                while (flag_1)
                {
                    system.student_menu (); // displays student menu options
                    cout << ">> ";
                    cin >> choice;
                    cin.ignore ();
                    switch (choice) 
                    {
                        case 0:
                            flag_1 = false; // goes back to main menu 
                            break;
                        case 1:
                            system.add_student (); // adds a student
                            break;
                        case 2:  // select a student
                            PA3 :: Student* selected_student = system.select_student ();
                            PA3 :: Course** selected_course_s;
                            PA3 :: Course** selected_course_taken_s;

                            flag_3 = true;
                            while (flag_3)
                            {
                                system.select_student_menu (); // displays options for selected student
                                cout << ">> ";
                                cin >> choice;
                                cin.ignore (); 
                                switch (choice)
                                {
                                    case 0:
                                        flag_3 = false; // goes back to previous menu
                                        break;
                                    case 1:
                                        system.delete_student (selected_student); // deletes selected student
                                        flag_3 = false;
                                        break;
                                    case 3:
                                        selected_course_s = system.list_courses (selected_student); // lists available courses
                                        cout << ">> ";
                                        cin >> choice;
                                        cin.ignore ();
                                        if (choice != 0)
                                        {
                                            selected_student -> add_course (selected_course_s [choice - 1]); // adds selected student to the selected course
                                            selected_course_s [choice - 1] -> add_student (selected_student); // adds the student to the selected course's list
                                        }
                                        break;
                                    case 4:
                                        selected_course_taken_s = system.list_courses_taken (selected_student); // lists courses taken by selected student
                                        cout << ">> ";
                                        cin >> choice;
                                        cin.ignore ();
                                        if (choice != 0)
                                        {
                                            selected_student -> remove_course (selected_course_taken_s [choice - 1]); // drops the selected course
                                            selected_course_taken_s [choice - 1] -> remove_student (selected_student); // deletes the student from the selected course's list                             
                                        }
                                        break;
                                }
                            }
                            break;
                    }
                }
                break;
            case 2:
                flag_2 = true;
                while (flag_2)
                {
                    system.course_menu (); // displays course menu options
                    cout << ">> ";
                    cin >> choice;
                    cin.ignore ();
                    switch (choice)
                    {
                        case 0:
                            flag_2 = false; // goes back to main menu
                            break;
                        case 1:
                            system.add_course (); // adds a course
                            break;
                        case 2:
                            PA3 :: Course* selected_course = system.select_course (); // select a course
                            flag_4 = true;
                            while (flag_4)
                            {
                                system.select_course_menu (); // displays options for selected course
                                cout << ">> ";
                                cin >> choice;
                                cin.ignore ();
                                switch (choice)
                                {
                                    case 0:
                                        flag_4 = false; // goes back to previous menu
                                        break;
                                    case 1:
                                        system.delete_course (selected_course); // deletes the selected course
                                        flag_4 = false;
                                        break;
                                    case 2:
                                        system.list_students (selected_course); // lists students registered to the selected course
                                        break;
                                }
                            }
                            break;                           
                    }
                }
                break;
            case 3:
                system.list_all_students (); // lists all students
                break;
            case 4:
                system.list_all_courses (); // lists all courses
                break;
        }
    }
    return 0;
}
