#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include "eigen.h"
#include "config.h"

using namespace std;

/** Methods to work with the data. **/

/*
Reads pgm file from the input file.
Returns the vector of pixel values.
*/
std::vector<double> read_pgm(std::ifstream& file) {
    std::vector<double> values;
    std::string line;
    getline(file, line);
    getline(file, line);
    getline(file, line);
    getline(file, line);


    int val;
    while(file >> val)
    {
        values.push_back(val);
    }
    return values;
}

/*
Writes pgm file, represented by a matrix (must be 1xM) to the output file.
*/
void write_pgm(std::string file, Matrix *image) {
    std::stringstream filename;
    filename << file;
    std::ofstream image_file(filename.str().c_str());
    image_file << "P2" << std::endl << Width << std::endl << Height << std::endl << MaxValue << std::endl;
    for (int i = 0; i < M; ++i)
    {
        int val = image->array[0][i];
        if (val < 0)
        {
            val = 0;
        }
        image_file << val << " ";
    }
    image_file.close();
}

/*
Reads the training data from the folder, specified in DataPath.
Samples are assumed to be in s{1}/{2}.pgm, where {1} is the number of a person, {2} is the number of a sample.
For each person only the mean image is calculated.
Returns the vector, s.t. vector[i][j] is the jth value of the ith image (mean image). Vector has N elements, each element has M elements.
*/
std::vector< std::vector<double> > read_training_data() {
    std::vector< std::vector<double> > array;

// Iteration over people.
    for (int face = 0; face < Faces; ++face)
    {
        // Iteration over photos.
            std::stringstream filename;
            filename << "Faces" << "/" << face << ".pgm";
            std::ifstream image(filename.str().c_str());

            array.push_back(read_pgm(image));

    }
    
    return array;
}

std::vector< std::vector<double> > read_training_data2() {
    std::vector< std::vector<double> > array;

// Iteration over people.
    for (int face = 0; face < Faces2; ++face)
    {
        // Iteration over photos.
            std::stringstream filename;
            filename << "TEST" << "/" << face << ".pgm";
            std::ifstream image(filename.str().c_str());

            array.push_back(read_pgm(image));

    }
    
    return array;
}
/****/

/** Methods to work with matrices **/

/*
Scales the elements of a matrix to the new range. Used when outputting the matrices to the PGM format.
Returns the new matrix.
*/
Matrix scale(Matrix m, double min = 0, double max = 255) {
    // Find the current minimum and maximum.
    double m_min = m.array[0][0];
    double m_max = m.array[0][0];
    for (int r = 0; r < m.rows; ++r) {
        for (int c = 0; c < m.columns; ++c) {
            if (m.array[r][c] < m_min) {
                m_min = m.array[r][c];
            }
            if (m.array[r][c] > m_max) {
                m_max = m.array[r][c];
            }
        }
    }
    double old_range = m_max - m_min;
    double new_range = max - min;

    // Create a new matrix with scaled elements.
    Matrix result;
    result.columns = m.columns;
    result.rows = m.rows;
    for (int r = 0; r < m.rows; ++r) {
        std::vector<double> row;
        for (int c = 0; c < m.columns; ++c) {
            row.push_back((m.array[r][c] - m_min)*new_range/old_range + min);
        }
        result.array.push_back(row);
    }
    return result;
}
/****/

/*
Recognizes the photo. See main() for explanation of X, B, U, W.
Returns the number of matched person in the training set.
*/
int recognize(Matrix X, Matrix B, Matrix U, Matrix W) {
    /* Subtract the mean image */
    for (int c = 0; c < M; ++c)
    {
        X.array[0][c] -= B.array[0][c];
        if (X.array[0][c] < 0)
        {
            X.array[0][c] = 0;
        }
    }

    /* Find weights */
    Matrix Wx = Matrix(Eigenfaces, 1);
    for (int r = 0; r < Eigenfaces; ++r)
    {
        Wx.array[r][0] = (U.getRow(r)*X.transpose()).array[0][0];
    }

    /* Find the closest face from the trainig set */
    double min_distance = 0;
    int image_number = 0;
    for (int image = 0; image < N; ++image)
    {
        double distance = 0;
        for (int eigenface = 0; eigenface < Eigenfaces; ++eigenface)
        {
            distance += fabs(W.array[eigenface][image] - Wx.array[eigenface][0]);
        }
        if (distance < min_distance || image == 0)
        {
            min_distance = distance;
            image_number = image;
        }
    }
    return image_number;
}


int main(int argc, const char * argv[])
{
    /*
    A contains the images as rows. A is NxM, [A]i,j is the jth pixel value of the ith image.
    B contains the mean image. B is 1xM matrix, [B]0,j is the jth pixel value of the mean image.
    S is the covariance matrix (each pixel is a random variable). S is NxN, computed as AA^T.
    V contains eigenvectors of S as rows. V is NxN.
    U contains eigenfaces as rows. U is NxM.
    W contains the weights of the eigenfaces in the training images. W is NxN, [W]i,j is the weight of the ith eigenface in the jth image.
    X is a 1xM matrix of the image to recognize (normalized).
    Wx is a Nx1 matrix, [W]i,0 is the weight of the ith eigenface in the X.
    */

    Matrix A = Matrix(N, M, read_training_data());
    Matrix B = Matrix(1, M);


    /* Find the mean image */
    for (int c = 0; c < M; ++c)
    {
        double sum = 0;
        for (int r = 0; r < N; ++r)
        {
            sum += A.array[r][c];
        }
        B.array[0][c] = sum/N;
    }

    /* Output the mean image */
    write_pgm("output/meanimage.pgm", &B);

    /* Subtract the mean from each image */
    for (int r = 0; r < N; ++r)
    {
        for (int c = 0; c < M; ++c)
        {
            A.array[r][c] -= B.array[0][c];
            if (A.array[r][c] < 0)
            {
                A.array[r][c] = 0;
            }
        }
    }

    /* Output the normalized images */
    for (int i = 0; i < N; ++i)
    {
        Matrix image = A.getRow(i);
        std::ostringstream filename;
        filename << "output/normalized/" << i << ".pgm";
        write_pgm(filename.str(), &image);
    }

    /* Find the covariance matrix */
    Matrix S = A*A.transpose();

    /* Find eigenvectors of the covariance matrix */
    Matrix V = eigensystem(&S).second.transpose();

    /* Find eigenfaces */
    Matrix U = Matrix(Eigenfaces, M);
    for (int r = 0; r < Eigenfaces; ++r)
    {
        Matrix eigenface = V.getRow(r)*A;

        U.array[r] = eigenface.array[0];
        double norm = 0;
        for (int i = 0; i < U.columns; i++) {


            norm += pow(U.array[r][i], 2);
        }
        norm = sqrt(norm);
        for (int i = 0; i < U.columns; i++) {
            U.array[r][i] /= norm;
        }
        /* Output eigenface */
        eigenface = scale(U.getRow(r));
        std::ostringstream filename;
        filename << "output/eigenfaces/" << r << ".pgm";
        write_pgm(filename.str(), &eigenface);
    }

    /* Find weights */
    Matrix W = Matrix(Eigenfaces, N);
    for (int r = 0; r < Eigenfaces; ++r)
    {
        for (int c = 0; c < N; ++c)
        {
            W.array[r][c] = (U.getRow(r)*A.getRow(c).transpose()).array[0][0];
        }
    }

    double sumVal = 0;
    std::vector<double> EigenValues = eigensystem(&S).first;
    for(int i = 0; i < Eigenfaces; ++i)
    {
        sumVal += EigenValues[i];
    }

    double RunTot = 0;
    double thrsh;
    for(int j = 0; j < Eigenfaces; ++j)
    {
        RunTot += EigenValues[j];
        if((RunTot / sumVal) > .8)
        {
            thrsh = j;
            j = Eigenfaces;            
        } 
    }


    /*Testing*/
    Matrix D = Matrix(Faces2, M, read_training_data2());
    
    /* Subtract the mean from each image */
    for (int r = 0; r < N; ++r)
    {
        for (int c = 0; c < M; ++c)
        {
            D.array[r][c] -= B.array[0][c];
            if (D.array[r][c] < 0)
            {
                D.array[r][c] = 0;
            }
        }
    }

    /* Find eigenfaces */
    Matrix Z = Matrix(Eigenfaces, M);
    for (int r = 0; r < Eigenfaces; ++r)
    {
        Matrix eigenfaceB = V.getRow(r)*D;

        Z.array[r] = eigenfaceB.array[0];
        double norm = 0;
        for (int i = 0; i < Z.columns; i++) {


            norm += pow(Z.array[r][i], 2);
        }
        norm = sqrt(norm);
        for (int i = 0; i < Z.columns; i++) {
            Z.array[r][i] /= norm;
        }
        /* Output eigenface */
        eigenfaceB = scale(Z.getRow(r));
        std::ostringstream filename;
        filename << "output/testing/" << r << ".pgm";
        write_pgm(filename.str(), &eigenfaceB);
    }

    /* Find weights */
    Matrix E = Matrix(Eigenfaces, N);
    for (int r = 0; r < thrsh; ++r)
    {
        for (int c = 0; c < N; ++c)
        {
            E.array[r][c] = (Z.getRow(r)*B.getRow(c).transpose()).array[0][0];
        }
    }

    double Malldist
    for(int rr = 0;  rr < thrsh; ++rr)
    {
        for(int cc = 0; cc < N; ++cc)
        {
            Malldist = min(W.array[rr][cc] - E.array[rr][cc])
        }
    }

    return 0;
}

