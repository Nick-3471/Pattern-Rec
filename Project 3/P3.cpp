///////////////////////////
//Project 3
//Brian Goga & Nicholas Smith
//CS 479
//////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <cmath>
#include <list>
#include <vector>
#include <dirent.h>
#include "eigen/Eigen/Core"

using namespace std;

void ReadImage(ifstream& file, vector<double> &face);
void wrightImage(string file,vector<double> face);

int Counter = 0;

int main()
{
//Variables
vector< vector<double> > faces (1204, vector<double>(2880,0));
int** test;

/////////////////////////////////////////////////////////////////////
//INPUT
/////////////////////////////////////////////////////////////////////
DIR* dr = NULL;
char* dirn = new char[50];
struct dirent *drnt = NULL;

cout << "Enter File name to import pictures: ";

gets(dirn);
dr = opendir(dirn);
drnt = readdir(dr);
drnt = readdir(dr);

cout << endl << "Reading in pictures..." << endl;
while(drnt = readdir(dr))
{
    stringstream filename;

    filename << dirn << "/" << drnt->d_name;
    ifstream picture(filename.str().c_str());

    ReadImage(picture, faces[Counter]);
    
    Counter++;
}

//////////////////////////////////////////////////////////////////////
//Mean
/////////////////////////////////////////////////////////////////////
cout << "Calulating Mean..." << endl;
vector<double> mean;
for (int i = 0; i < 2880; ++i)
    {
    double sum = 0;
    for (int j = 0; j < 1204; ++j)
        {
        sum += faces[j][i];
        }
    mean.push_back(sum/1204);
    }
faces.push_back(mean);

//////////////////////////////////////////////////////////////////////
//Covariance
/////////////////////////////////////////////////////////////////////
cout << "Creating Covariance Matrix" << endl;
vector< vector<double> > Trans (0, vector<double>(2880,0));

for(int s = 0; s < 2880; s++)
{
    vector<double> row (0, 0);
    for(int t = 0; t < 1204; t++)
    {
        row.push_back(faces[t][s]);
    }
    Trans.push_back(row);
}


//////////////////////////////////////////////////////////////////////
//OUTPUT
/////////////////////////////////////////////////////////////////////
cout << "Writing out pictures..." << endl;
for(int g = 0; g < Counter +1; g++)
{
    ostringstream outfilename;
    outfilename << "Test/" << g << ".pgm";
    wrightImage(outfilename.str(), faces[g]);
}
	




	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//ReadImage
/////////////////////////////////////////////////////////////////////////////////////////
void ReadImage(ifstream& file, vector<double> &face)
{
 int i, j, M, N, Q;
 unsigned char *image;
 char header [100], *ptr;
 
 string list;
 getline(file, list);




 M = 48;
 N= 60;
 Q = 255;

 image = (unsigned char *) new unsigned char [M*N];

 file.read( reinterpret_cast<char *>(image), (M*N)*sizeof(unsigned char));

 file.close();

 //
 // Convert the unsigned characters to integers
 //

 for(i=0; i<N; i++)
   for(j=0; j<M; j++)
     face[i*M+j] = (double)image[i*M+j];
}

/////////////////////////////////////////////////////////////////////////////////////////
//WriteImage
/////////////////////////////////////////////////////////////////////////////////////////
void wrightImage(string file, vector<double> face)
{
stringstream filename;
filename << file;
ofstream picture(filename.str().c_str());

 int i, j, M, N, Q;
 unsigned char *image;
 char header [100], *ptr;

 M = 48;
 N= 60;
 Q = 255;

image = (unsigned char *) new unsigned char [M*N];

 for(i=0; i<N; i++)
   for(j=0; j<M; j++)
     image[i*M+j] = (unsigned char)face[i*M+j];

picture << "P5"  << endl;
picture << 48 << " " << 60 << endl;
picture << 255 << endl;

picture.write( reinterpret_cast<char *>(image), (M*N)*sizeof(unsigned char));

picture.close();
}
