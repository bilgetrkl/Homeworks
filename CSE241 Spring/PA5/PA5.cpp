#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <algorithm>

using namespace std;

class field_format
{
    public:
        field_format () : name (""), type (""), is_arr (false) {} // default constructor
        field_format (string name, string type, bool is_arr) : name (name), type (type), is_arr (is_arr) {} // parameterized constructor
        string get_name () const { return name; } // gets the name of the field
        string get_type () const { return type; } // gets the type of the field
        bool get_is_arr () const { return is_arr; } // gets if the field is an array or not 
    private:
        string name; // name of the field
        string type; // type of the field
        bool is_arr; // is array or not
};
class field // field class
{
    public:
        virtual ~field () = default; // virtual destructor, makes the class abstract 
        virtual string get_data () const = 0; // pure virtual function for getting data
};
class field_str : public field // the string type field class
{
    public:
        field_str (string input) : data (input) {} // constructor
        string get_data () const override { return data; } // gets the data
    private:
        string data; // the string type data
};
class field_str_arr : public field // the string array type field class 
{
    public:
        field_str_arr (string input); // constructor, parses the input
        string get_data () const override; // gets the vector
    private:
        vector <string> data; // vector of the data
};
field_str_arr :: field_str_arr (string input)
{
    stringstream ss (input); // creates a string stream from the input
    string token; // string the store each individual 
    while (getline (ss, token, ':')) // gets the line
    {
        data.push_back (token); // adds the data to the vector
    }
}
string field_str_arr :: get_data () const 
{
    stringstream ss;
    for (size_t i = 0; i < data.size (); ++i) 
    {
        ss << data [i];
        if (i != data.size () - 1) 
        {
            ss << ":"; // add : between the elements
        }
    }
    return ss.str ();
}
class field_char : public field 
{
    public:
        field_char (string input) : data (input [0]) {} // constructor
        string get_data () const override { return string (1, data); } // gets the data
    private:
        char data;
};
class field_char_arr : public field // the char array type field class 
{
    public:
        field_char_arr (string input); // constructor, parses the input
        string get_data () const override; // gets the vector
    private:
        vector <char> data; // vector of the data
};
field_char_arr :: field_char_arr (string input)
{
    stringstream ss (input); // creates a string stream from the input
    char token; // string the store each individual 
    while (ss >> token) 
    {
        if (token != ':')
        {
            data.push_back( token); // adds the data to the vector
        }
       
    }
}
string field_char_arr :: get_data () const 
{
    stringstream ss;
    for (size_t i = 0; i < data.size (); ++i) 
    {
        ss << data [i];
        if (i != data.size () - 1) 
        {
            ss << ":"; // add : between the elements
        }
    }
    return ss.str ();
}
class field_int : public field 
{
    public:
        field_int (string input) : data (stoi (input)) {} // constructor
        string get_data () const override {return to_string (data); } // gets the data
    private:
        int data;
};
class field_int_arr : public field // the int array type field class 
{
    public:
        field_int_arr (string input); // constructor, parses the input
        string get_data () const override; // gets the vector
    private:
        vector <int> data; // vector of the data
};
field_int_arr :: field_int_arr (string input)
{
    stringstream ss (input); // creates a string stream from the input
    string token; // string to store each individual component
    while (getline (ss, token, ':')) // uses string to extract tokens
    {
        stringstream token_stream (token);
        int value;
        token_stream >> value; // converts string to int
        data.push_back (value); // adds the data to the vector
    }
}
string field_int_arr :: get_data () const 
{
    stringstream ss;
    for (size_t i = 0; i < data.size (); ++i) 
    {
        ss << data [i];
        if (i != data.size () - 1) 
        {
            ss << ":"; // add : between the elements
        }
    }
    return ss.str ();
}
class field_double : public field 
{
    public:
        field_double (std :: string input) : data (stod (input)) {} // constructor
        string get_data () const override { return to_string (data); } // gets the data
    private:
        double data;
};
class field_double_arr : public field // the double array type field class 
{
    public:
        field_double_arr (string input); // constructor, parses the input
        string get_data () const override; // gets the vector
    private:
        vector <double> data; // vector of the data
};
field_double_arr :: field_double_arr (string input)
{
    stringstream ss (input); // creates a string stream from the input
    string token; // string to store each individual component
    while (getline( ss, token, ':')) // uses string to extract tokens
    {
        stringstream token_stream (token);
        double value;
        token_stream >> value; // converts string to double
        data.push_back(value); // adds the data to the vector
    }
}
string field_double_arr :: get_data () const 
{
    stringstream ss;
    for (size_t i = 0; i < data.size (); ++i) 
    {
        ss << data [i];
        if (i != data.size () - 1) 
        {
            ss << ":"; // add : between the elements
        }
    }
    return ss.str ();
}
class Catalog
{
    public:
        Catalog () = default;// default constructor
        Catalog (const vector <field_format>& format) : format (format) {} // constructor that takes the format
        Catalog (const vector <field_format>& format, const vector <vector <field*>>& data) : format (format), data (data) {} // constructor that takes a format and initial data entries
        ~Catalog (); // destructor
        void add_entry (const vector <field*>& entry) { if (!entry.empty ()) data.push_back (entry); } // adds entry
        void print (ofstream& out_file) const; // prints the data to the output.txt 
        void search (const string &command_line, const string &keyword, const string &field_name, ofstream &out_file) const; // searches for the desired word
        void sort (const string &field_name); // sorts the structure according to the desired field
        vector <vector <string>>  get_entry () const; // gets the entries
    private:
        vector <field_format> format; // the format of the catalog
        vector <vector <field*>> data; // the data
};
Catalog :: ~Catalog () // cleans up dynamically allocated fields
{ 
    for (auto& entry : data) 
    {
        for (auto& field_ptr : entry)
        {
            delete field_ptr;
        }
    }
}
void Catalog :: print (ofstream& out_file) const // prints the lines
{
    for (const auto& entry : data)  // iterates thorugh each entry in the data vector
    {
        for (auto it = entry.begin (); it != entry.end (); ++it)  // iterates through each field pointer in the current entry
        {
            out_file << (*it) -> get_data (); // writes the data of the current field to the output file
            if (it != entry.end () - 1) 
            {
                out_file << "|";
            }
        }
        out_file << endl;
    }
}
void Catalog :: search (const string &command_line, const string &keyword, const string &field_name, ofstream &out_file) const 
{
    // finds the format that matches the given field name
    auto it = find_if (format.begin (), format.end (), [&] (const field_format &fmt) { return fmt.get_name () == field_name; });
    if (it == format.end ()) // if the field name is not found, throws a runtime error
    {
        throw runtime_error ("command is wrong");
    }

    int field_index = distance (format.begin (), it); // calculates the index of the found format

    out_file << command_line << endl; // writes the command line to the output.txt
    for (const auto &entry : data) // iterates through each entry in the data vector
    {
        if (entry [field_index] -> get_data ().find (keyword) != string :: npos) // checks if the keyword is found within the data of the specified field
        {
            for (auto field_it = entry.begin (); field_it != entry.end (); ++field_it) // iterates through each field in the entry
            {
                out_file << (*field_it) -> get_data (); // writes the data of the current field to the output.txt
                if (field_it != entry.end () - 1) 
                {
                    out_file << "|"; 
                }
            }
            out_file << endl;
        }
    }
}
void Catalog :: sort (const string &field_name) 
{
    // finds the format that matches the given field name
    auto it = find_if (format.begin (), format.end (), [&] (const field_format &fmt) { return fmt.get_name () == field_name; });
    if (it == format.end ()) // if the field name is not found
    {
        throw runtime_error ("command is wrong");
    }

    int field_index = distance (format.begin (), it); // calculates the index of the found format

    // sorts the data vector based on the values of the specified field
    std :: sort (data.begin (), data.end (), [field_index] (const vector <field *> &a, const vector <field *> &b) 
    {
        return a [field_index] -> get_data () < b [field_index] -> get_data ();
    });
}
vector <vector <string>> Catalog :: get_entry () const
{
    vector <vector <string>> lines;
    for (const auto& entry : data)
    {
        vector <string> line;
        for (const auto& field_ptr : entry)
        {
            line.push_back (field_ptr -> get_data ()); // push back the fields 
        }
        lines.push_back (line); // push back the lines
    }
    return lines;
}
vector <field_format> parse_format (const string& format_line) // parses the first line of data.txt
{
    vector <field_format> format; // stores the parsed format
    stringstream ss (format_line); // creates a string stream from the format line
    string field; // string to store each individual field

    while (getline (ss, field, '|')) // loops through each field
    {
        stringstream fd (field); // creates a string stream from the current field
        string name, type, is_array_str; // strings to store the name, type, and is_array components of the field
        getline (fd, name, ':'); // the name component
        getline (fd, type, ':'); // the type component
        getline (fd, is_array_str, ':'); // the is_array component

        bool is_array = (is_array_str == "multi"); // returns true if multi is written in the file
        format.push_back (field_format (name, type, is_array)); // creates a field object and add it to the vector
    }
    return format; // returns the vector of parsed format
}
field* create_field (const field_format& format, const string& value)
{
    if (format.get_type () == "string" && !format.get_is_arr ()) return new field_str (value); // if the field type is string and is not an array
    else if (format.get_type () == "string" && format.get_is_arr ()) return new field_str_arr (value); // if the field type is string and is an array
    else if (format.get_type () == "char" && !format.get_is_arr ()) return new field_char (value); // if the field type is char and is not an array
    else if (format.get_type () == "char" && format.get_is_arr ()) return new field_char_arr (value); // if the field type is char and is an array
    else if (format.get_type () == "integer" && !format.get_is_arr ()) return new field_int (value); // if the field type is int and is not an array
    else if (format.get_type () == "integer" && format.get_is_arr ()) return new field_int_arr (value); // if the field type is int and is an array
    else if (format.get_type () == "double" && !format.get_is_arr ()) return new field_double (value); // if the field type is double and is not an array
    else if (format.get_type () == "double" && format.get_is_arr ()) return new field_double_arr (value); // if the field type is double and is an array
}
vector <field*> parse_entry (const vector <field_format>& format, const string& entry_line, ofstream& out_file) 
{
    vector <field*> entry;
    stringstream ss (entry_line);
    string value;

    static unordered_set <string> parsed_entries; // keeps track of parsed entries

    try 
    {
        string entry_str; // String representation of the entry
        while (getline (ss, value, '|')) 
        {
            entry_str += value + "|"; // concatenates field values to form the entry string
            entry.push_back (create_field (format [entry.size ()], value)); // adds the field to the entry vector
        }
        if (!parsed_entries.insert (entry_str).second) // checks if the entry is duplicate
        {
            throw runtime_error ("duplicate entry");
        }
        if (entry.size () < format.size ()) // checks if there is a missing field
        {
            throw runtime_error ("missing field");
        }
    } 
    catch (const runtime_error& e) 
    {
        out_file << "Exception: " << e.what () << endl; // writes error message to output.txt
        out_file << entry_line << endl; // writes the corresponding line to the output.txt
        return {}; // returns an empty entry
    }
    return entry; // returns the parsed entry
}
bool parse_command (const string &command_line, string &command, string &keyword, string &field_name,  ofstream& out_file)
{
    stringstream ss (command_line); // creates a string stream from the command line
    ss >> command; // extracts the first word as the command

    if (command == "search")
    {
        size_t first_quote = command_line.find ('"'); // finds the position of the first double quote
        size_t second_quote = command_line.find ('"', first_quote + 1); // finds the position of the second double quote
        if (first_quote == string :: npos || second_quote == string :: npos) // checks if the quotes are found
        {
            throw runtime_error ("command is wrong");
        }
        keyword = command_line.substr (first_quote + 1, second_quote - first_quote - 1); // extracts the keyword between the first and second quotes

        size_t third_quote = command_line.find ('"', second_quote + 1); // finds the positions of the third double quote
        size_t fourth_quote = command_line.find ('"', third_quote + 1); // finds the positions of the fourth double quote
        if (third_quote == string :: npos || fourth_quote == string :: npos) // checks if the quotes are found
        {
            throw runtime_error ("command is wrong");
        }
        field_name = command_line.substr (third_quote + 1, fourth_quote - third_quote - 1); // extracts the field name between the third and fourth quotes
        return true; // returns true if the command is successfully parsed
    }
    else if (command == "sort")
    {
        size_t first_quote = command_line.find ('"'); // finds the position of the first double quote
        size_t second_quote = command_line.find ('"', first_quote + 1);  // finds the position of the second double quote
        if (first_quote == string :: npos || second_quote == string :: npos) // checks if the quotes are found
        {
            throw runtime_error ("command is wrong");
        }
        field_name = command_line.substr (first_quote + 1, second_quote - first_quote - 1); // extracts the field name between the first and second quotes
        return true; // returns true if the command is successfully parsed
    }
    else
    {
        throw runtime_error ("command is wrong"); // throws an error if the command is neither "search" nor "sort"
    }
}
int main ()
{
    ifstream file_d ("data.txt"); // opens the data.txt for reading
    ofstream out_file ("output.txt"); // creates and output.txt for writing

    string format_line; // the format line
    getline (file_d, format_line); // gets the first line of the data.txt file
    
    vector <field_format> format = parse_format (format_line); // returns the format of the entries
    for (auto it = format.begin (); it != format.end (); ++it) // prints the field names to the output.txt
    {
        out_file << it -> get_name ();
        if (it != format.end () - 1) 
        {
            out_file << "|";
        }
    }
    out_file << endl;
    Catalog catalog (format); // creates a Catalog object and sets the format
    
    string entry_line; // the entry line
    while (getline (file_d, entry_line))
    {
        catalog.add_entry (parse_entry (format, entry_line, out_file)); // adds the entries to the Catalog object
    }
    file_d.close (); // closes the data.txt file
    out_file << catalog.get_entry ().size () << " unique entries" << endl; // writes the number of the unique entries to the output.txt file

    ifstream file_c ("commands.txt"); // opens the commands.txt for reading

    string command_line; // the command line
    while (getline (file_c, command_line)) // reads each line from the commands.txt
    {
        string command, keyword, field_name;
        try
        {
            if (parse_command (command_line, command, keyword, field_name, out_file)) // parses the command line and extracts the command, keyword, and field name
            {
                if (command == "search") // first command type
                {
                    try
                    {
                        catalog.search (command_line, keyword, field_name, out_file); // searches for the given keyword in the given field 
                    }
                    catch (const runtime_error &e)
                    {
                        out_file << "Exception: " << e.what () << endl; // writes an error message to the output.txt
                        out_file << command_line << endl; // writes the command line to the output.txt
                    }
                }
                else if (command == "sort") // second command type
                {
                    try
                    {
                        catalog.sort (field_name); // sorts the lines according to the field
                        out_file << command_line << endl; // writes the command line
                        catalog.print (out_file); // writes the sorted lines
                    }
                    catch (const runtime_error &e)
                    {
                        out_file << "Exception: " << e.what () << endl; // writes an error message to the output.txt
                        out_file << command_line << endl; // writed the command line to the output.txt
                    }
                }
            } 
        }
        catch (const runtime_error &e) // if the command format is wrong
        {
            out_file << "Exception: " << e.what () << endl; // writes an error message to the output.txt
            out_file << command_line << endl; // writed the command line to the output.txt
        }
    }
    file_c.close (); // closes the commands.txt
    out_file.close (); // closes the output.txt
    return 0;
}
