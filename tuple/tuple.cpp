#include <iostream>
#include <fstream>
#include <sstream>
#include "tuple.hpp"

using namespace std;
using namespace kick_ass_lib;

int main (int argc, char **argv)
{
	tuple<int, int, double> myTuple(1, 2, 3.14);

	cout<<get<1>(myTuple)<<" "<<get<2>(myTuple)<<" "<<get<3>(myTuple)<<endl;

	return 0;
}
