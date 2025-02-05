#include <iostream>
#include <fstream>
#include <sstream>  
#include <string>
#include <vector>
#include "SparseVector.h"

using namespace std;

void Swap (vec& a, vec& b) // swaps elements of two structures
{
    vec temp = a;
    a = b;
    b = temp;
}
void SparseVector :: sort () // sorts the vector by indices
{
    for (int i = 0; i < alldata.size () - 1; i++) 
    {
        for (int j = i + 1; j < alldata.size (); j++) 
        {
            if (alldata [i].index > alldata [j].index) 
            {
                Swap (alldata [i], alldata [j]);
            }
        }
    }
}
void SparseVector :: add () // adds the datas with the same index
{
    for (int i = 0; i < alldata.size () - 1; i++) // navigates the vector
    {
        if (alldata [i].index == alldata [i + 1].index) // checks if the indices are the same 
        {
            alldata [i].data += alldata [i + 1].data;
            alldata.erase (alldata.begin () + i + 1);
            if (alldata [i].data == 0.0) // if the sum is equal to zero, it deletes that index
            {
                alldata.erase (alldata.begin () + i);
                i--;
            }
        }
    }
}
SparseVector :: SparseVector () {} // default constructor
SparseVector :: SparseVector (const string& filename) // reads the file
{
    ifstream file (filename);
    if (file.is_open ()) 
    {
        string line;
        while (getline (file, line)) // gets the line
        {
            istringstream iss (line);
            char colon; // :
            int index;
            double data;
            
            while (iss >> index >> colon >> data) 
            {
                vec v = {index, data};
                alldata.push_back (v); // transfers the received data to the vector
            }
        }
        file.close ();
    } 
    else 
    {
        std::cerr << "File opening error: " << filename << std::endl;
    }
}
SparseVector SparseVector :: operator+ (const SparseVector& sparse_vec) // adds two sparse vectors
{   
    SparseVector sparse_vec_3;

    for (int i = 0; i < alldata.size (); i++) // first, the first vector is transferred to the vector of the newly created object
    {
        sparse_vec_3.alldata.push_back (alldata [i]);
    }
    for (int i = 0; i < sparse_vec.alldata.size (); i++) // then,the second vector is transferred behind the first vector in the vector of the newly created object
    {
        sparse_vec_3.alldata.push_back (sparse_vec.alldata [i]);
    }
    sparse_vec_3.sort (); // elements are sorted according to their indices
    sparse_vec_3.add (); // elements at the same index are summed

    return sparse_vec_3; // new object is returned
}
SparseVector SparseVector :: operator- (const SparseVector& sparse_vec) // subtracts one sparse vector from another
{
    SparseVector sparse_vec_3;

    for (int i = 0; i < alldata.size (); i++) // first, the first vector is transferred to the vector of the newly created object
    {
        sparse_vec_3.alldata.push_back (alldata [i]);
    }
    for (int i = 0; i < sparse_vec.alldata.size (); i++) // then,the second vector is transferred behind the first vector in the vector of the newly created object
    {
        sparse_vec_3.alldata.push_back (sparse_vec.alldata [i]);
        sparse_vec_3.alldata [i].data  *= -1; // the transferred data is negated by multiplying by -1
    }
    sparse_vec_3.sort (); // elements are sorted according to their indices
    sparse_vec_3.add (); // elements at the same index are summed

    return sparse_vec_3; // new object is returned
}
SparseVector SparseVector :: operator- () // negates the elements of a sparse vector
{
    SparseVector sparse_vec_2;

    for (int i = 0; i < alldata.size (); i++) // navigates the vector
    {
        sparse_vec_2.alldata.push_back (alldata [i]); // transfers the entire vector to the vector of the newly created object
        sparse_vec_2.alldata [i].data *= -1; // negates all data variables
    }
    return sparse_vec_2; // new object is returned
}
SparseVector& SparseVector :: operator= (const SparseVector& sparse_vec) // assigns one sparse vector to another
{
    if (this != &sparse_vec) // check for self-assignment
    {
        alldata.clear (); // clear the current data of the vector
        alldata = sparse_vec.alldata;  // copy the data from the source vector to this vector
    }
    return *this; // return a reference to this vector
}
std::ostream& operator<< (std::ostream& outputStream, const SparseVector& sparse_vec) // sends contents of a sparse vector to a std::ostream object
{
    for (int i = 0; i < sparse_vec.alldata.size (); i++) 
    {
        outputStream << sparse_vec.alldata [i].index << ":" << sparse_vec.alldata [i].data << " "; // prints the vector in the desired format
    }
    return outputStream;
}
double dot (const SparseVector& sparse_vec_1, const SparseVector& sparse_vec_2) // calculates the dot product of two sparse vectors
{
    double dot_product = 0;

    for (int i = 0; i < sparse_vec_1.alldata.size (); i++) // loops through both vectors, multiplies and adds elements at the same index
    {
        for (int j = 0; j < sparse_vec_2.alldata.size (); j++)
        {
            if (sparse_vec_1.alldata [i].index == sparse_vec_2.alldata [j].index)
            {
                dot_product += sparse_vec_1.alldata [i].data * sparse_vec_2.alldata [j].data;
            }
        }
    }
    return dot_product; // return the dot product
}