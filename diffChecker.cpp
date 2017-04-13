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
	
	fstream f1("large.txt");
	ifstream f2("decoded.txt");
	
	long long int a,b;
	int err = 0;
	int cc = 0;
	int ab = 0;
	while(f1>>a && f2>>b)
	{
		ab++;
		while(a > 999999)f1>>a;
		//cout<<a<<"&&&&&"<<b<<"\n";
		if(a != b){ cout<<a<<" &&&&& "<<b<<"\n";err++;}
		cc++;
		if(cc%1000000 == 0)cout<<cc<<"----"<<err<<"\n";
	}
	cout<<"------------------------------"<<err;
}
	
