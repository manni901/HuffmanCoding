#include<iostream>
#include<bitset>
#include<vector>
using namespace std;

int main()
{
	vector<int> V;
	V.push_back(int(1));
	int *n = &V[V.size()-1];
	int *a = n;
	cout<<(*a);
}
