#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "radixTree.hpp"

using namespace std;
using namespace kick_ass;
#define MAX 100

void normalize (string &str)
{
    for (int i = 0; i < str.size(); i++) {
	if (str[i] >= 65 && str[i] <= 90)
	    str[i] += 32;
    }
    return;
}

void displayResult(string &qury, vector<string> &result)
{
    cout<<"****Following words in dictionary start with "<<qury<<"****"<<endl;
    for (int i = 0; i < result.size(); i++)
	cout<<"("<<i+1<<") "<<result[i]<<endl;
}

int main(int argc, char **argv)
{
    char line[MAX];
    ifstream inFile;
    radixTree rTree;
    inFile.open(argv[1]);
    while (!inFile.eof()) {
	memset(line, 0, sizeof(line));
	inFile.getline(line, MAX);
	if (!strlen(line))
	    continue;
	string word(line);
	normalize(word);
	rTree.insert(word);
    }
    cout<<"****done building corpus****"<<endl;
    char ch;
    do {
	cout<<endl;
	cout<<"words starting with: ";
	string qury;
	cin>>qury;
	normalize(qury);
	if (rTree.lookup(qury)) {
	    cout<<"Words with prefix "<<qury<<" are present in corpus."<<endl;
	    vector<string> &result = rTree.wordsStartingWith(qury);
	    displayResult(qury, result);
	} else
	    cout<<"Words with prefix "<<qury<<" are not present in corpus."<<endl;
	cout<<"Wanna continue?\t";
	cin>>ch;
    } while (ch != 'n' && ch != 'N');

    return 0;
}
