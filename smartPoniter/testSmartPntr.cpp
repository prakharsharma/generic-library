#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "smartPntr.hpp"

using namespace std;

int main (int argc, char **argv)
{
    int *p = new int(3);
    int *q = new int[12];

    /*usage model*/
    {
	smartPntr<int> pInt; /*a smart pntr to an integer*/
	//smartPntr<int> scalar = new int(3);//NOT ALLOWED
	//smartPntr<int> vector = new int[13];//NOT ALLOWED
	pInt.attach(new int(2));
	smartPntr<int> qInt;//(p);
	qInt(p);
	qInt = pInt;/*assignment operator*/
	smartPntr<int, vectorResource<int> > vector;
	vector.attach(q);
    }
    cout<<"**************************"<<endl;

    return 0;
}
