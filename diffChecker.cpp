#include<iostream>
#include<fstream>
using namespace std;

int main()
{
	//ifstream f1("code_table.txt");
	//ifstream f2("code_table1.txt");
	
	/*long long int a,b;
	string A,B;
	int err = 0;
	int maxi = 0, maxi1 = 0;
	while((f1>>a && f1>>A) && (f2>>b && f2>>B))
	{
		if(A.length() > maxi) maxi = A.length();
		if(B.length() > maxi1) maxi1 = B.length();
	}
	cout<<maxi<<" "<<maxi1;*/
	
	ifstream f1("sample_input_large.txt");
	ifstream f2("decoded.txt");
	
	long long int a,b;
	int err = 0;
	while(f1>>a && f2>>b)
	{
		if(a != b)err++;
	}
	cout<<err;
}
	
