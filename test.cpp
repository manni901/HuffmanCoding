#include<iostream>
#include<bitset>
#include<vector>
#include<fstream>
using namespace std;

int main()
{
	ifstream is("sample_input_large.txt");
	is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
    cout<<length;
}
