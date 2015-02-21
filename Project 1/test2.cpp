///////////////////////////////////////////////////////////////////////////////
// Project 1
// CS479
// Nicholas Smith
// Brian Goga
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
struct point
{ 
 double x_value;
 double y_value;

 int mean;
 int category;

};
double randomeDouble()
{
	return ((double)rand()/(double)RAND_MAX);
}

void Gauss(double** array)
{
	
	array = new double* [10000];
	for(int i = 0; i < 10000; i++)
	{
		(array)[i] = new double [2];
	}


	double x1, x2, y1, y2 , w;

	for(int q = 0; q < 10000; q++)
	{
		do
		{
			x1 = 2.0 * randomeDouble() - 1.0;
			x2 = 2.0 * randomeDouble() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while (w >= 1.0 );

		w = sqrt( (-2.0 * log(w)) / w);
		array[q][0] = x1 * w;
		array[q][1] = x2 * w;
	}
}

void assignPoints(double** array, point* points, int percent)
{
	int i;

	for(i=0; i< 1000 * percent; i++)
	{
		points[i].x_value = array[i][0] +1.0;
		points[i].y_value = array[i][1] +1.0;

		points[i].mean = 1;
	}

	while(i<10000)
	{
		points[i].x_value = array[i][0] +4.0;
		points[i].y_value = array[i][1] +4.0;

		points[i].mean = 2;

		i++;
	}
}

int main()
{
	double mean1[2][1] = {{1.0}, {1.0}};
	double mean2[2][1] = {{4.0}, {4.0}};
	double** swag;

	point* data = new point[10000];

	double x1, x2, y1, y2 , w;

	Gauss(swag);
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1 A
////////////////////////////////////////	/////////////////////////////////////////////////////////////

	assignPoints(swag, data, 5);
	

	return 0;
}