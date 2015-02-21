///////////////////////////////////////////////////////////////////////////////
// Project 1
// CS479
// Nicholas Smith
// Brian Goga
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cmath>
#include <random>
using namespace std;




int main()
{
	
 const int nrolls=10000;  // number of experiments
  const int nstars=100;    // maximum number of stars to distribute

  default_random_engine generator;
  normal_distribution<double> distribution(5.0,2.0);

  int p[10]={};

  for (int i=0; i<nrolls; ++i) {
    double number = distribution(generator);
    if ((number>=0.0)&&(number<10.0)) ++p[int(number)];
  }

  cout << "normal_distribution (5.0,2.0):" << endl;

  for (int i=0; i<10; ++i) {
    cout << i << "-" << (i+1) << ": ";
    cout << std::string(p[i]*nstars/nrolls,'*') << endl };







	return 0;
}