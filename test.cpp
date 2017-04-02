#include<iostream>
#include<bitset>
#include<vector>
#include<fstream>
#include<climits>
using namespace std;

int main()
{
	char a = 1;
	char b = a>>2;
	bitset<CHAR_BIT> A(a);
	bitset<CHAR_BIT> B(b);
	bitset<CHAR_BIT> C(a << (CHAR_BIT - 2));
	cout<<A<<"\n"<<B<<"\n"<<C<<"\n";
	int f = 5;
	f = f<<2;
	cout<<f;
	//char a = 0;
	//cout<<bitset<CHAR_BIT>(a);
}

/*void vectorBitShift(vector<char>&A, vector<char> V, int shift)
{
	char prev = A.back();
	A.pop_back();
	for(int i=0;i<V.size();i++)
	{
		char temp = V[i];
		V[i] = V[i]>>shift;
		V[i] &= prev;
		A.push_back(V[i]);
		prev = temp << (CHAR_BIT - shift);
	}
}*/
