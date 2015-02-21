/* boxmuller.c           
Implements the Polar form of the Box-Muller
Transformation

(c) Copyright 1994, Everett F. Carter Jr.
Permission is granted by the author to use
this software for any application provided this
copyright notice is preserved.

*/

#include <cmath>
#include <cstdio>
#include <iostream>
#include <cstdlib>
using namespace std;

//extern float ranf();         /* ranf() is uniform in 0..1 */
float box_muller(float m, float s);

double swag()
{
	return (rand() / (RAND_MAX + 1.0));
}




int main(){


srand(time(NULL));

float test;

test = box_muller(1, 0);
printf("%.6f", test);
test = box_muller(1, 0);
printf("%.6f", test);
return 0;
}

float box_muller(float m, float s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * swag() - 1.0;
			x2 = 2.0 * swag() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}


