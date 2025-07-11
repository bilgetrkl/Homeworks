#ifndef SPARSEVECTOR_H
#define SPARSEVECTOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct vec
{
    int index;
    double data;
};

class SparseVector 
{ 
    public:
        void sort (); // sorts the vector by indices
        void add (); // adds the datas with the same index
        SparseVector (); // default constructor
        SparseVector (const std :: string& filename); // reads the file
        SparseVector operator+ (const SparseVector& sparse_vec); // adds two sparse vectors
        SparseVector operator- (const SparseVector& sparse_vec); // subtracts one sparse vector from another
        SparseVector operator- (); // negates the elements of a sparse vector
        SparseVector& operator= (const SparseVector& sparse_vec); // assigns one sparse vector to another
        friend std::ostream& operator<< (std :: ostream& outputStream, const SparseVector& sparse_vec); // sends contents of a sparse vector to a std::ostream object
        friend double dot (const SparseVector& sparse_vec_1, const SparseVector& sparse_vec_2); // calculates the dot product of two sparse vectors
    private:
        std :: vector <vec> alldata;
};

#endif 
