#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream> 
#include <fstream>  
#include <sstream>  
#include <vector>   
#include <string>

struct mat
{
    int index;
    int row_index;
    double data;
};
class SparseMatrix 
{
    public:
        void add (); // adds the datas with the same row and column indices
        void sort_1 (); // sorts the vector by row indices
        void sort_2 (); // sorts the vector by column indices
        SparseMatrix (); // default constructor
        SparseMatrix (const std :: string& filename); // reads the file
        SparseMatrix operator+ (const SparseMatrix& sparse_mat); // adds two sparse matrices
        SparseMatrix operator- (const SparseMatrix& sparse_mat); // subtracts one sparse matrix from another
        SparseMatrix operator- (); // negates elements of a sparse matrix
        SparseMatrix& operator= (const SparseMatrix& sparse_mat); // assigns one sparse matrix to another
        friend std::ostream& operator<< (std :: ostream& outputStream, const SparseMatrix& sparse_mat); 
        SparseMatrix transpose (); // returns the transpose of a sparse matrix
    private:
        std :: vector <mat> alldata;
};

#endif