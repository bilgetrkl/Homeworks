#include <iostream> 
#include <fstream>  
#include <sstream>  
#include <vector>   
#include <string>
#include "SparseMatrix.h"

using namespace std;

void Swap (mat& a, mat& b) // swaps elements of two structures
{
    mat temp = a;
    a = b;
    b = temp;
}
void SparseMatrix :: add () // adds the datas with the same row and column indices
{
    for (int i = 0; i < alldata.size (); i++) // navigates the vector
    {
        for (int j = i + 1; j < alldata.size (); j++)
        {
            if (alldata [i].row_index == alldata [j].row_index && alldata [i].index == alldata [j].index) // checks if the row and column indices are the same 
            {
                alldata [i].data += alldata [j].data;
                alldata.erase (alldata.begin () + j);
                if (alldata [i].data == 0.0)  // if the sum is equal to zero, it deletes that index
                {
                    alldata.erase (alldata.begin () + i);
                    i--;
                }
                break;
            }
        }
    }
}
void SparseMatrix :: sort_1 () // sorts the vector by row indices
{
    for (int i = 0; i < alldata.size () - 1; i++) 
    {
        for (int j = i + 1; j < alldata.size (); j++) 
        {
            if (alldata [i].row_index > alldata [j].row_index) 
            {
                Swap (alldata [i], alldata [j]);
            }
        }
    }
}
void SparseMatrix :: sort_2 () // sorts the vector by column indices
{
    for (int i = 0; i < alldata.size () - 1; i++) 
    {
        for (int j = i + 1; j < alldata.size (); j++) 
        {
            if (alldata [i].row_index == alldata [j].row_index && alldata [i].row_index > alldata [j].row_index) 
            {
                Swap (alldata [i], alldata [j]);
            }
        }
    }
}
SparseMatrix :: SparseMatrix () {} // default constructor
SparseMatrix :: SparseMatrix (const string& filename) // reads the file
{
    ifstream file (filename);
    if (file.is_open ()) 
    {
        string line;
        int row_index;
        while (getline (file, line)) // gets the line
        {
            istringstream iss (line);
            iss >> row_index;
            int index;
            char colon;
            double data;
            while (iss >> index >> colon >> data) 
            {
                mat m = {index, row_index, data};
                alldata.push_back (m); // transfers the received data to the vector
            }
        }
        file.close ();
    } 
    else 
    {
        cerr << "File opening error: " << filename << endl;
    }
}
SparseMatrix SparseMatrix :: operator+ (const SparseMatrix& sparse_mat) // adds two matrices
{
    SparseMatrix sparse_mat_3;

    for (int i = 0; i < alldata.size (); i++) // first, the first vector is transferred to the vector of the newly created object
    {
        sparse_mat_3.alldata.push_back (alldata [i]);
    }
    for (int i = 0; i < sparse_mat.alldata.size (); i++) // then,the second vector is transferred behind the first vector in the vector of the newly created object
    {
        sparse_mat_3.alldata.push_back (sparse_mat.alldata [i]);
    }    
    sparse_mat_3.add (); // elements at the same row and column index are summed
    sparse_mat_3.sort_1 (); // elements are sorted according to their row indices
    sparse_mat_3.sort_2 (); // elements are sorted according to their column indices

    return sparse_mat_3; // new object is returned
}
SparseMatrix SparseMatrix :: operator- (const SparseMatrix& sparse_mat) // subtracts one sparse matrix from another
{
    SparseMatrix sparse_mat_3;

    for (int i = 0; i < alldata.size (); i++) // first, the first vector is transferred to the vector of the newly created object
    {
        sparse_mat_3.alldata.push_back (alldata [i]);
    }
    for (int i = 0; i < sparse_mat.alldata.size (); i++) // then,the second vector is transferred behind the first vector in the vector of the newly created object
    {
        sparse_mat_3.alldata.push_back (sparse_mat.alldata [i]);
        sparse_mat_3.alldata [i].data *= -1; // the transferred data is negated by multiplying by -1
    }
    sparse_mat_3.add (); // elements at the same row and column index are summed
    sparse_mat_3.sort_1 (); // elements are sorted according to their row indices
    sparse_mat_3.sort_2 (); // elements are sorted according to their column indices

    return sparse_mat_3; // new object is returned
}
SparseMatrix SparseMatrix :: operator- () // negates elements of a sparse matrix
{
    SparseMatrix sparse_mat_2;
    for (int i = 0; i < alldata.size (); i++) // navigates the vector
    {
        sparse_mat_2.alldata.push_back (alldata [i]); // transfers the entire vector to the vector of the newly created object
        sparse_mat_2.alldata [i].data *= -1; // negates all data variables
    }
    return sparse_mat_2; // new object is returned
}
SparseMatrix& SparseMatrix :: operator= (const SparseMatrix& sparse_mat) // assigns one sparse matrix to another
{
    if (this != &sparse_mat) // check for self-assignment
    {
        alldata.clear (); // clear the current data of the vector
        alldata = sparse_mat.alldata; // copy the data from the source vector to this vector
    }
    return *this; // return a reference to this vector
}
std::ostream& operator<< (std :: ostream& outputStream, const SparseMatrix& sparse_mat) // sends contents of a sparse matrix to a std::ostream object.
{
    int ex_row;

    for (int i = 0; i < sparse_mat.alldata.size (); i++)
    {
        if (i == 0 || (ex_row != sparse_mat.alldata [i].row_index)) // if the index is 0 or row index is not same with the ex row index
        {
            outputStream << endl << sparse_mat.alldata [i].row_index << " " << sparse_mat.alldata [i].index << ":" << sparse_mat.alldata [i].data << " "; // prints the vector with the row value
        }
        else
        {
            outputStream << sparse_mat.alldata [i].index << ":" << sparse_mat.alldata [i].data << " "; // else prints the vector without the row value
        }
        ex_row = sparse_mat.alldata [i].row_index; // ex row value assignment is made
    } 
    return outputStream;
}
SparseMatrix SparseMatrix :: transpose () // returns the transpose of a sparse matrix
{
    SparseMatrix sparse_mat;
    int i, temp;
    for (i = 0; i < alldata.size (); i++) // changes row and column index values ​​in the current location to obtain transpose and assigns this value to the object
    {
        temp = alldata [i].row_index;
        alldata [i].row_index = alldata [i].index;
        alldata [i].index = temp;
        sparse_mat.alldata.push_back (alldata [i]);
    }
    sparse_mat.sort_1 (); // elements are sorted according to their row indices
    sparse_mat.sort_2 (); // elements are sorted according to their column indices

    return sparse_mat; // returns the new object
}



