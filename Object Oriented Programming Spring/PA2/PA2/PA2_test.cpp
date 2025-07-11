#include <iostream>
#include <fstream>
#include <string>
#include "SparseVector.h"
#include "SparseMatrix.h"

using namespace std;
int main()
{
	ofstream outfile;
	outfile.open("output.txt", ios::out | ios::trunc );
	//Creating the first SparseVector from file
	SparseVector a1("a1.txt");
	outfile<<"a1"<<endl<<a1<<endl;
	//Creating the second SparseVector from file
	SparseVector a2("a2.txt");
	outfile<<"a2"<<endl<<a2<<endl;
	//operator +
	a1 = a1 + a2;
	outfile<<"a1 + a2"<<endl<<a1<<endl;
	// operator - two vectors
	a1 = a1 - a2;
	outfile<<"a1 - a2"<<endl<<a1<<endl;
	//operator - one vector
	a1 = -a1;
	outfile<<"-a1"<<endl<<a1<<endl;
	//Dot product
	outfile<<"dot(a1, a2)"<< endl << dot(a1,a2) <<endl;
	//Assign
	a2 = a1;
	outfile<<"a2 = a1"<<endl<<a1<<endl;
	
	//Creating SparseMatrix from file
	SparseMatrix m1("m1.txt");
	SparseMatrix m2("m2.txt");
	//matrices
	outfile<<"m1"<<endl<<m1<<endl;
	outfile<<"m2"<<endl<<m2<<endl;
	//operator +
	m1 = m1 + m2;
	outfile<<"m1 + m2"<<endl<<m1<<endl;
	//operator -
	m1 = m1 - m2;
	outfile<<"m1 - m2"<<endl<<m1<<endl;
	m1 = -m1;
	outfile<<"-m1"<<endl<<m1<<endl;
	//operator *
	//m1 = m1 * m2;
	//outfile<<"m1 * m2"<<endl<<m1<<endl;
	//Transpose
	outfile<<"m2.transpose"<<endl<<m2.transpose()<<endl;
	//Assign
	m2 = m1;
	outfile<<"m2 = m1"<<endl<<m2<<endl;
	
	return 0;
}
