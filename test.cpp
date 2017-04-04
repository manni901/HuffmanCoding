#include<iostream>
#include<bitset>
#include<vector>
#include<fstream>
#include<climits>
using namespace std;

int main()
{
	char a = 'a';
	bitset<CHAR_BIT> B(a);
	string c = B.to_string();
	cout<<c;
}
