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

using namespace std;

vector<double> readImage(ifstream& file);
void wrightImage(string file,vector<double> face);




int main()
{
//Variables
vector< vector<double> > faces(1204, vector<double>(2880,0));
int q = 0;


/////////////////////////////////////////////////////////////////////
//INPUT
/////////////////////////////////////////////////////////////////////
DIR* dr = NULL;
char* dirn = new char[50];
struct dirent *drnt = NULL;

gets(dirn);
dr = opendir(dirn);
drnt = readdir(dr);
drnt = readdir(dr);
drnt = readdir(dr);

stringstream filename;
filename << dirn << "/" << drnt->d_name;
ifstream picture(filename.str().c_str());

for(int d = 0; d < 2; d++)
{
	faces.push_back(readImage(picture));
	q++;
}
closedir(dr);

/////////////////////////////////////////////////////////////////////
//OUTPUT
/////////////////////////////////////////////////////////////////////
for(int g = 0; g < 1204; g++)
{
        ostringstream filename;
        filename << "Test/" << g << ".pgm";
        wrightImage(filename.str(), faces[g]);
}
	




	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//ReadImage
/////////////////////////////////////////////////////////////////////////////////////////
vector<double> readImage(ifstream& file)
{
 	vector<double> values;
    std::string line;
    getline(file, line); // Skip P2 line
    getline(file, line); // Skip width line
    getline(file, line); // Skip height line
    getline(file, line); // Skip max value line

    int val;
    while(file >> val)
    {
        values.push_back(val);
    }
    return values;
}

/////////////////////////////////////////////////////////////////////////////////////////
//WriteImage
/////////////////////////////////////////////////////////////////////////////////////////
void wrightImage(string file,vector<double> face)
{
	stringstream filename;
	filename << file;
	ofstream image_file(filename.str().c_str());
	image_file << "P2" << endl << 48 << endl << 60 << endl << 255 << endl;

for (int i = 0; i < 2880; ++i)
    {
        int val = face[i];
        if (val < 0)
        {
            val = 0;
        }
        image_file << val << " ";
    }
    image_file.close();
}
