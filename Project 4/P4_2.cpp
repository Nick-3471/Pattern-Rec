///////////////////////////////////////////////////////////////////////////////
// Name 
// Date
// Project
// Other Info
///////////////////////////////////////////////////////////////////////////////

// Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>

// Other Includes
#include "config.h"
#include "libsvm/svm.cpp"

// Define

using namespace std;

// Function Headers
void MatrixInversion(double **A, int order, double **Y);
int GetMinor(double **src, double **dest, int row, int col, int order);
double CalcDeterminant( double **mat, int order);
double LogProb(double[69][30], int num, double** cov);

// Main
int main()
{
	//Varibles
	svm_problem Tr_1;
	svm_parameter param;
	svm_model *model;
	svm_node **node = new svm_node*[train_Size];
	svm_node **tests = new svm_node*[test_Size];
	string line;
	double* temp = new double[train_Size];

	ifstream X;
	X.open("genderdata/16_20/trPCA_01.txt");

	Tr_1.x = new svm_node*[train_Size];
	for(int q = 0; q < train_Size; q++)
	{
		node[q] = new svm_node[eigen_Size + 1];
		for(int i = 0; i < eigen_Size; i++)
		{
			node[q][i].index = i;
			//node[q][i].value = X.get();
			X >> node[q][i].value;
		}
		//Setting end of vector
		node[q][eigen_Size].index = -1;
		node[q][eigen_Size].value = 0.0;

		//gets rest of line
		getline(X, line);
	}
	Tr_1.x = node;
	X.close();
	X.open("genderdata/16_20/TtrPCA_01.txt");

	for(int i = 0; i < train_Size; i++)
	{	
		X >> temp[i];
	}
	Tr_1.y = temp;
	X.close();

	//Setting l
	Tr_1.l = train_Size;

	
	X.open("genderdata/16_20/tsPCA_01.txt");

	for(int q = 0; q < test_Size; q++)
	{
		tests[q] = new svm_node[eigen_Size + 1];
		for(int i = 0; i < eigen_Size; i++)
		{
			tests[q][i].index = i;
			X >> tests[q][i].value;
		}
		//Setting end of vector
		tests[q][eigen_Size].index = -1;
		tests[q][eigen_Size].value = 0.0;

		//gets rest of line
		getline(X, line);

	}	
	X.close();


	double Male [69][30];
	double Female [65][30];
	double **CovM = new double*[30];
	double **CovF = new double*[30];
	double **InM = new double*[30];
	double **InF = new double*[30];
	double MM [30];
	double MF [30];
	double meanM = 0;
	double meanF = 0;

	int counterM = 0;
	int counterF = 0;


	for (int i = 0; i < 69; ++i)
	{
		CovM[i] = new double[30];
		InM[i] = new double[30];
	}
	for (int i = 0; i < 65; ++i)
	{
		CovF[i] = new double[30];
		InF[i] = new double[30];
	}

	cout << "Generating Classes" << endl;
	for(int i = 0; i < train_Size; i++)
	{
		if(temp[i] == 1)
		{
			for(int q = 0; q < eigen_Size; q++)
				{
					
				Male[counterM][q] = node[i][q].value;				
				}
		counterM++;

		}
		if(temp[i] == 2)
		{
			for(int q = 0; q < eigen_Size; q++)
				{
					
				Female[counterF][q] = node[i][q].value;

				}
		counterF++;
		}

	}

	cout << "Calculateing Mean" << endl;
	for (int i = 0; i < 69; ++i)
	{
		
		for (int q = 0; q < eigen_Size; ++q)
		{
			MM[q]+= Male[i][q];
		}
	}
	for (int i = 0; i < 65; ++i)
	{
		for (int q = 0; q < eigen_Size; ++q)
		{
			MF[q]+= Female[i][q];
		}
	}

		for(int i = 0; i < 69; i++)
		{
			for (int q = 0; q < eigen_Size; ++q)
			{
				Male[i][q] -= MM[q];
			}
		}

		for(int i = 0; i < 65; i++)
		{
			for (int q = 0; q < eigen_Size; ++q)
			{
				Female[i][q] -= MF[q];
			}
		}


	cout << "Getting Covariance Matrix" << endl;
	for (int i = 0; i < 30; ++i)
	{
		for (int q = 0; q < 30; ++q)
		{
			CovM[i][q] = Male[i][q] * Male[i][q];
		}
	}

	for (int i = 0; i < 65; ++i)
	{
		for (int q = 0; q < 30; ++q)
		{
			CovF[i][q] = Female[i][q] * Female[i][q];
		}
	}
	MatrixInversion(CovM, 30, InM);
	MatrixInversion(CovF, 30, InF);

	cout << "Classifing Points" << endl;
	double p1, p2;
	int prob;
	for(int i = 0; i < train_Size; i++)
	{
		p1 = LogProb(Male, 30, InM);
		p2 = LogProb(Female, 30, InF);

		if(p1 > p2)
		{
			prob = 1;
		}
		else
		{
			prob = 2;
		}
			
		if(prob != Error[i])
		{
			counter++;
		}

	}
	cout << "Error: ";
	cout << counter << " -- " << (counter/test_Size)*100 << endl;




	return 0;
}

/////////////////////////////////////////////////////////////////////
//Function Calls
/////////////////////////////////////////////////////////////////////
void MatrixInversion(double **A, int order, double **Y)
{
    // get the determinant of a
    double det = 1.0/CalcDeterminant(A,order);
 
    // memory allocation
    double *temp = new double[(order-1)*(order-1)];
    double **minor = new double*[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = temp+(i*(order-1));
 
    for(int j=0;j<order;j++)
    {
        for(int i=0;i<order;i++)
        {
            // get the co-factor (matrix) of A(j,i)
            GetMinor(A,minor,j,i,order);
            Y[i][j] = det*CalcDeterminant(minor,order-1);
            if( (i+j)%2 == 1)
                Y[i][j] = -Y[i][j];
        }
    }
 
    // release memory
    //delete [] minor[0];
    delete [] temp;
    delete [] minor;
}
 
// calculate the cofactor of element (row,col)
int GetMinor(double **src, double **dest, int row, int col, int order)
{
    // indicate which col and row is being copied to dest
    int colCount=0,rowCount=0;
 
    for(int i = 0; i < order; i++ )
    {
        if( i != row )
        {
            colCount = 0;
            for(int j = 0; j < order; j++ )
            {
                // when j is not the element
                if( j != col )
                {
                    dest[rowCount][colCount] = src[i][j];
                    colCount++;
                }
            }
            rowCount++;
        }
    }
 
    return 1;
}
 
// Calculate the determinant recursively.
double CalcDeterminant( double **mat, int order)
{
    // order must be >= 0
    // stop the recursion when matrix is a single element
    if( order == 1 )
        return mat[0][0];
 
    // the determinant value
    double det = 0;
 
    // allocate the cofactor matrix
    double **minor;
    minor = new double*[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = new double[order-1];
 
    for(int i = 0; i < order; i++ )
    {
        // get minor of element (0,i)
        GetMinor( mat, minor, 0, i , order);
        // the recusion is here!
 
        det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,order-1);
        //det += pow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
    }
 
    // release memory
    for(int i=0;i<order-1;i++)
        delete [] minor[i];
    delete [] minor;
 
    return det;
}

double LogProb(double Male[69][30], int num, double** cov)
{
	double swag = 0;
	for(int g = 0; g < 69; g++)
	{
		for(int i = 0; i < 30; i++)
		{
		swag += ((Male[g][i] * cov[i][i]) + Male[g][i]);
		}
	}
	return swag;
}
